#pragma once

#include <cstdint>
#include <string>
#include <vector>

class TcpClient
{
public:
    TcpClient();
    ~TcpClient();

    bool connect_to(const std::string& ip, uint16_t port);
    bool is_connected() const;

    bool send_bytes(const std::vector<uint8_t>& data);

    // Blocking receive:
    //  - reads exactly `nbytes`
    //  - returns empty vector on error / disconnect
    std::vector<uint8_t> recv_exact(size_t nbytes);

    void close();

private:
    int sock_fd_;
};
