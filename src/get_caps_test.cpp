#include "rfid_llrp/tcp_client.hpp"
#include "rfid_llrp/llrp_framing.hpp"
#include "rfid_llrp/llrp_encoder.hpp"
#include "rfid_llrp/llrp_types.hpp"
#include "rfid_llrp/llrp_status.hpp"


#include <iostream>

int main()
{
    const std::string reader_ip = "192.168.1.98"; // <-- CHANGE THIS
    const uint16_t reader_port = 5084;

    TcpClient client;
    if (!client.connect_to(reader_ip, reader_port)) {
        std::cerr << "Failed to connect to reader\n";
        return 1;
    }

    std::cout << "Connected to reader\n";

    const uint32_t msg_id = 1;

    auto msg = llrp::encode_get_reader_capabilities(msg_id);
    if (!client.send_bytes(msg)) {
        std::cerr << "Failed to send GET_READER_CAPABILITIES\n";
        return 1;
    }

    std::cout << "Sent GET_READER_CAPABILITIES\n";

    try {
        while (true) {
            auto frame = llrp::read_frame(client);

            std::cout << "Received frame: "
                    << llrp::header_to_string(frame.header)
                    << "\nPayload bytes: "
                    << frame.payload.size()
                    << std::endl;

            // Ignore the unsolicited vendor/notification frame(s)
            if (frame.header.message_type == llrp::GET_READER_CAPABILITIES_RESPONSE ||
                frame.header.message_type == 12 /* ERROR_MESSAGE */)
            {
                llrp::LlrpStatus st;
                if (llrp::parse_llrp_status(frame.payload, st)) {
                    std::cout << "LLRP STATUS:\n"
                            << "  " << llrp::status_to_string(st) << "\n";
                    if (st.status_code != 0) {
                        std::cout << "\nRequest failed (status_code != 0)\n";
                        return 1;
                    }
                } else {
                    std::cout << "No LLRPStatus found in payload (too short)\n";
                }

                if (frame.header.message_type == llrp::GET_READER_CAPABILITIES_RESPONSE) {
                    std::cout << "Got GET_READER_CAPABILITIES_RESPONSE ✔\n";
                    break;
                }

                if (frame.header.message_type == 12) {
                    std::cout << "Got ERROR_MESSAGE (type 12)\n";
                    return 1;
                }
            }

            std::cout << "Ignoring message type " << frame.header.message_type << "\n";
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
