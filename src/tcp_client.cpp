#include "rfid_llrp/tcp_client.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

TcpClient::TcpClient()
: sock_fd_(-1)
{
}

TcpClient::~TcpClient()
{
    close();
}

bool TcpClient::connect_to(const std::string& ip, uint16_t port)
{
    close();

    sock_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd_ < 0) {
        perror("socket");
        return false;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) != 1) {
        std::cerr << "Invalid IP address: " << ip << std::endl;
        close();
        return false;
    }

    if (connect(sock_fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        perror("connect");
        close();
        return false;
    }

    return true;
}

bool TcpClient::is_connected() const
{
    return sock_fd_ >= 0;
}

bool TcpClient::send_bytes(const std::vector<uint8_t>& data)
{
    if (sock_fd_ < 0) {
        return false;
    }

    size_t total_sent = 0;
    while (total_sent < data.size()) {
        ssize_t sent = send(
            sock_fd_,
            data.data() + total_sent,
            data.size() - total_sent,
            0);

        if (sent <= 0) {
            perror("send");
            close();
            return false;
        }

        total_sent += static_cast<size_t>(sent);
    }

    return true;
}

std::vector<uint8_t> TcpClient::recv_exact(size_t nbytes)
{
    std::vector<uint8_t> buffer(nbytes);
    size_t total_read = 0;

    while (total_read < nbytes) {
        ssize_t n = recv(
            sock_fd_,
            buffer.data() + total_read,
            nbytes - total_read,
            0);

        if (n <= 0) {
            // 0 = orderly shutdown, <0 = error
            perror("recv");
            close();
            return {};
        }

        total_read += static_cast<size_t>(n);
    }

    return buffer;
}

void TcpClient::close()
{
    if (sock_fd_ >= 0) {
        ::close(sock_fd_);
        sock_fd_ = -1;
    }
}
