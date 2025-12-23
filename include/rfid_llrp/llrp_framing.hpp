#pragma once

#include <cstdint>
#include <string>
#include <vector>

class TcpClient;

namespace llrp
{
struct Header
{
    uint8_t  version{0};
    uint16_t message_type{0};   // 10-bit in spec, stored in 16-bit
    uint32_t length{0};         // total bytes including 10-byte header
    uint32_t message_id{0};
    uint8_t  reserved{0};       // low 3 bits of the first 16-bit field
};

struct Frame
{
    Header header;
    std::vector<uint8_t> payload;  // length - 10 bytes
};

// Read one full LLRP frame from the TCP stream (blocking).
// Returns { } on disconnect/error.
// Throws std::runtime_error on invalid framing (e.g., insane length).
Frame read_frame(TcpClient& client, uint32_t max_frame_bytes = 1024 * 1024);

// Convenience: stringify header for logging/debugging.
std::string header_to_string(const Header& h);

} // namespace llrp
