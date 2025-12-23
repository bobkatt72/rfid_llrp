#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace llrp {

struct LlrpStatus {
    uint16_t param_type{0};       // 10-bit parameter type
    uint16_t param_reserved{0};   // 6-bit reserved
    uint16_t param_len{0};        // bytes including 4-byte param header
    uint16_t status_code{0};      // 0 = success, otherwise failure
    uint16_t reserved{0};         // reserved in LLRPStatus
};

bool parse_llrp_status(const std::vector<uint8_t>& payload, LlrpStatus& out);
std::string status_to_string(const LlrpStatus& s);

} // namespace llrp
