#include "rfid_llrp/llrp_framing.hpp"
#include "rfid_llrp/tcp_client.hpp"

#include <sstream>
#include <stdexcept>

// ---- Big-endian helpers (LLRP fields are network byte order) ----
static uint16_t read_be_u16(const uint8_t* p)
{
    return static_cast<uint16_t>((static_cast<uint16_t>(p[0]) << 8) |
                                 (static_cast<uint16_t>(p[1]) << 0));
}

static uint32_t read_be_u32(const uint8_t* p)
{
    return (static_cast<uint32_t>(p[0]) << 24) |
           (static_cast<uint32_t>(p[1]) << 16) |
           (static_cast<uint32_t>(p[2]) << 8)  |
           (static_cast<uint32_t>(p[3]) << 0);
}

namespace llrp
{
    Frame read_frame(TcpClient& client, uint32_t max_frame_bytes)
    {
        // LLRP header is always 10 bytes:
        //  0..1 : Version (3 bits), MessageType (10 bits), Reserved (3 bits)
        //  2..5 : Message Length (uint32)
        //  6..9 : Message ID (uint32)
        constexpr size_t HEADER_LEN = 10;

        auto hdr_bytes = client.recv_exact(HEADER_LEN);
        if (hdr_bytes.empty()) {
            return {}; // socket closed / error
        }
        if (hdr_bytes.size() != HEADER_LEN) {
            return {};
        }

        const uint16_t ver_type_reserved = read_be_u16(hdr_bytes.data() + 0);
        const uint32_t length            = read_be_u32(hdr_bytes.data() + 2);
        const uint32_t message_id        = read_be_u32(hdr_bytes.data() + 6);

        Header h;
        h.version      = static_cast<uint8_t>((ver_type_reserved >> 13) & 0x7);   // top 3 bits
        h.message_type = static_cast<uint16_t>((ver_type_reserved >> 3) & 0x3FF); // next 10 bits
        h.reserved     = static_cast<uint8_t>(ver_type_reserved & 0x7);           // low 3 bits
        h.length       = length;
        h.message_id   = message_id;

        // Basic validation
        if (h.length < HEADER_LEN) {
            throw std::runtime_error("LLRP frame length < 10 (invalid)");
        }
        if (h.length > max_frame_bytes) {
            throw std::runtime_error("LLRP frame length exceeds max_frame_bytes (likely desync)");
        }

        const size_t payload_len = static_cast<size_t>(h.length - HEADER_LEN);

        Frame f;
        f.header = h;
        if (payload_len > 0) {
            f.payload = client.recv_exact(payload_len);
            if (f.payload.size() != payload_len) {
                return {}; // disconnected mid-frame
            }
        }

        return f;
    }

    std::string header_to_string(const Header& h)
    {
        std::ostringstream oss;
        oss << "LLRP{ver=" << static_cast<int>(h.version)
            << ", type=" << h.message_type
            << ", len=" << h.length
            << ", msg_id=" << h.message_id
            << ", rsv=" << static_cast<int>(h.reserved)
            << "}";
        return oss.str();
    }

} // namespace llrp
