#include "rfid_llrp/llrp_encoder.hpp"
#include "rfid_llrp/llrp_types.hpp"

namespace llrp
{

static void write_be_u16(std::vector<uint8_t>& buf, uint16_t v)
{
    buf.push_back(static_cast<uint8_t>((v >> 8) & 0xFF));
    buf.push_back(static_cast<uint8_t>(v & 0xFF));
}

static void write_be_u32(std::vector<uint8_t>& buf, uint32_t v)
{
    buf.push_back(static_cast<uint8_t>((v >> 24) & 0xFF));
    buf.push_back(static_cast<uint8_t>((v >> 16) & 0xFF));
    buf.push_back(static_cast<uint8_t>((v >> 8) & 0xFF));
    buf.push_back(static_cast<uint8_t>(v & 0xFF));
}

std::vector<uint8_t> encode_get_reader_capabilities(uint32_t message_id)
{
    std::vector<uint8_t> buf;
    buf.reserve(14);

    // ── LLRP Header ─────────────────────────────
    // Version = 1, MessageType = 1, Reserved = 0
    uint16_t ver_type =
        static_cast<uint16_t>((1u << 13) | (GET_READER_CAPABILITIES << 3));

    write_be_u16(buf, ver_type);

    // Placeholder for message length
    write_be_u32(buf, 0);

    // Message ID
    write_be_u32(buf, message_id);

    // ── Message Body (NOT TLV!) ─────────────────
    // RequestedData = All (0)
    buf.push_back(0x00);

    // Reserved (3 bytes)
    buf.push_back(0x00);
    buf.push_back(0x00);
    buf.push_back(0x00);

    // ── Fix up length ───────────────────────────
    uint32_t length = static_cast<uint32_t>(buf.size());
    buf[2] = static_cast<uint8_t>((length >> 24) & 0xFF);
    buf[3] = static_cast<uint8_t>((length >> 16) & 0xFF);
    buf[4] = static_cast<uint8_t>((length >> 8) & 0xFF);
    buf[5] = static_cast<uint8_t>(length & 0xFF);

    return buf;
}

} // namespace llrp
