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

        // ----- LLRP Header -----
        uint16_t ver_type =
            static_cast<uint16_t>((1u << 13) | (GET_READER_CAPABILITIES << 3));

        write_be_u16(buf, ver_type);
        write_be_u32(buf, 0);              // length placeholder
        write_be_u32(buf, message_id);

        // ----- RequestedData PARAMETER (TLV!) -----
        // Type = 256, Length = 5, Value = 0 (All)

        write_be_u16(buf, 256);             // ParamType: RequestedData
        write_be_u16(buf, 5);               // ParamLength
        buf.push_back(REQUESTED_DATA_ALL);  // Value

        // ----- Fix length -----
        uint32_t length = static_cast<uint32_t>(buf.size());
        buf[2] = (length >> 24) & 0xFF;
        buf[3] = (length >> 16) & 0xFF;
        buf[4] = (length >> 8) & 0xFF;
        buf[5] = (length >> 0) & 0xFF;

        return buf;
    }

} // namespace llrp
