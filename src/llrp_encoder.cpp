#include "rfid_llrp/llrp_encoder.hpp"
#include "rfid_llrp/llrp_types.hpp"

namespace
{
    void write_be_u16(std::vector<uint8_t>& buf, uint16_t v)
    {
        buf.push_back(static_cast<uint8_t>((v >> 8) & 0xFF));
        buf.push_back(static_cast<uint8_t>((v >> 0) & 0xFF));
    }

    void write_be_u32(std::vector<uint8_t>& buf, uint32_t v)
    {
        buf.push_back(static_cast<uint8_t>((v >> 24) & 0xFF));
        buf.push_back(static_cast<uint8_t>((v >> 16) & 0xFF));
        buf.push_back(static_cast<uint8_t>((v >> 8) & 0xFF));
        buf.push_back(static_cast<uint8_t>((v >> 0) & 0xFF));
    }
}

namespace llrp
{

std::vector<uint8_t> encode_get_reader_capabilities(uint32_t message_id)
{
    std::vector<uint8_t> buf;

    // ---- LLRP Header (10 bytes) ----
    uint16_t ver_type = static_cast<uint16_t>((1u << 13) | (GET_READER_CAPABILITIES << 3));
    buf.push_back(static_cast<uint8_t>(ver_type >> 8));
    buf.push_back(static_cast<uint8_t>(ver_type & 0xFF));

    // Placeholder for length
    buf.insert(buf.end(), {0, 0, 0, 0});

    // Message ID
    buf.push_back(static_cast<uint8_t>((message_id >> 24) & 0xFF));
    buf.push_back(static_cast<uint8_t>((message_id >> 16) & 0xFF));
    buf.push_back(static_cast<uint8_t>((message_id >> 8) & 0xFF));
    buf.push_back(static_cast<uint8_t>((message_id >> 0) & 0xFF));

    // ---- RequestedData TLV (8 bytes) ----
    // Parameter header: type=117, length=8
    buf.push_back(0x00);
    buf.push_back(0x75);  // ParamType (117)
    buf.push_back(0x00);
    buf.push_back(0x08);  // ParamLength (8)

    // Field: RequestedData + 3 reserved
    buf.push_back(REQUESTED_DATA_ALL);
    buf.push_back(0x00);
    buf.push_back(0x00);
    buf.push_back(0x00);

    // ---- Fix up length ----
    uint32_t length = static_cast<uint32_t>(buf.size());
    buf[2] = static_cast<uint8_t>((length >> 24) & 0xFF);
    buf[3] = static_cast<uint8_t>((length >> 16) & 0xFF);
    buf[4] = static_cast<uint8_t>((length >> 8) & 0xFF);
    buf[5] = static_cast<uint8_t>((length >> 0) & 0xFF);

    return buf;
}

} // namespace llrp
