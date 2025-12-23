#pragma once

#include <cstdint>
#include <vector>

namespace llrp
{

std::vector<uint8_t> encode_get_reader_capabilities(uint32_t message_id)
{
    std::vector<uint8_t> buf;
    buf.reserve(18);

    // ---- LLRP Message Header ----
    // Version = 1, MessageType = GET_READER_CAPABILITIES (1)
    uint16_t ver_type =
        static_cast<uint16_t>((1u << 13) | (GET_READER_CAPABILITIES << 3));

    write_be_u16(buf, ver_type);

    // Placeholder for length
    write_be_u32(buf, 0);

    // Message ID
    write_be_u32(buf, message_id);

    // ---- RequestedData PARAMETER ----
    // Parameter Type = 4
    // Parameter Length = 8
    write_be_u16(buf, 4);   // RequestedData param
    write_be_u16(buf, 8);   // length

    // RequestedData = All (0)
    buf.push_back(0x00);

    // Reserved (3 bytes)
    buf.push_back(0x00);
    buf.push_back(0x00);
    buf.push_back(0x00);

    // ---- Fix up message length ----
    uint32_t length = static_cast<uint32_t>(buf.size());
    buf[2] = static_cast<uint8_t>((length >> 24) & 0xFF);
    buf[3] = static_cast<uint8_t>((length >> 16) & 0xFF);
    buf[4] = static_cast<uint8_t>((length >> 8) & 0xFF);
    buf[5] = static_cast<uint8_t>((length >> 0) & 0xFF);

    return buf;
}


}
