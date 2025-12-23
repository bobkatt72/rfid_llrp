#include "rfid_llrp/llrp_status.hpp"
#include <sstream>

static uint16_t read_be_u16(const uint8_t* p)
{
    return static_cast<uint16_t>((static_cast<uint16_t>(p[0]) << 8) |
                                 (static_cast<uint16_t>(p[1]) << 0));
}

namespace llrp {

bool parse_llrp_status(const std::vector<uint8_t>& payload, LlrpStatus& out)
{
    // LLRPStatus parameter is at least 8 bytes:
    //  0..1: ParamType(10) + Reserved(6)
    //  2..3: ParamLen
    //  4..5: StatusCode
    //  6..7: Reserved
    if (payload.size() < 8) return false;

    const uint16_t type_and_reserved = read_be_u16(payload.data() + 0);

    out.param_reserved = static_cast<uint16_t>((type_and_reserved >> 10) & 0x003F);
    out.param_type     = static_cast<uint16_t>(type_and_reserved & 0x03FF);
    out.param_len      = read_be_u16(payload.data() + 2);
    out.status_code    = read_be_u16(payload.data() + 4);
    out.reserved       = read_be_u16(payload.data() + 6);

    return true;
}

std::string status_to_string(const LlrpStatus& s)
{
    std::ostringstream oss;
    oss << "LLRPStatus{param_type=" << s.param_type
        << ", param_reserved=" << s.param_reserved
        << ", param_len=" << s.param_len
        << ", status_code=" << s.status_code
        << ", reserved=" << s.reserved
        << "}";
    return oss.str();
}

} // namespace llrp
