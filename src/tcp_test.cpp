#include "rfid_llrp/tcp_client.hpp"
#include <iostream>

int main()
{
    TcpClient client;

    if (!client.connect_to("192.168.1.98", 5084)) {
        std::cerr << "Failed to connect\n";
        return 1;
    }

    std::cout << "Connected to reader\n";
    return 0;
}
