#include "rfid_llrp/llrp_encoder.hpp"
#include "rfid_llrp/llrp_types.hpp"

namespace llrp
{

std::vector<uint8_t> encode_get_reader_capabilities(uint32_t message_id)
{
    std::vector<uint8_t> buf;

    // ===== LLRP MESSAGE HEADER (10 bytes) =====
    // Version = 1, Type = GET_READER_CAPABILITIES (1)
    uint16_t ver_type =
        static_cast<uint16_t>((1u << 13) | (GET_READER_CAPABILITIES << 3));

    buf.push_back((ver_type >> 8) & 0xFF);
    buf.push_back(ver_type & 0xFF);

    // Placeholder for message length
    buf.push_back(0);
    buf.push_back(0);
    buf.push_back(0);
    buf.push_back(0);

    // Message ID
    buf.push_back((message_id >> 24) & 0xFF);
    buf.push_back((message_id >> 16) & 0xFF);
    buf.push_back((message_id >> 8) & 0xFF);
    buf.push_back(message_id & 0xFF);

    // ===== REQUESTED_DATA PARAMETER (8 bytes TOTAL) =====
    // Parameter Type = 117 (RequestedData)
    // Parameter Length = 8 (including this header)

    buf.push_back(0x00);
    buf.push_back(0x75);   // 117
    buf.push_back(0x00);
    buf.push_back(0x08);   // length = 8

    // RequestedData = 0 (ALL)
    buf.push_back(0x00);
    buf.push_back(0x00);
    buf.push_back(0x00);
    buf.push_back(0x00);

    // ===== FIX MESSAGE LENGTH =====
    uint32_t len = static_cast<uint32_t>(buf.size());
    buf[2] = (len >> 24) & 0xFF;
    buf[3] = (len >> 16) & 0xFF;
    buf[4] = (len >> 8) & 0xFF;
    buf[5] = len & 0xFF;

    return buf;
}

} // namespace llrp
