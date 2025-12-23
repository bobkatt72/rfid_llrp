#pragma once

#include <cstdint>

namespace llrp
{
constexpr uint16_t GET_READER_CAPABILITIES            = 1;
constexpr uint16_t GET_READER_CAPABILITIES_RESPONSE   = 11;

// RequestedData enum (LLRP spec)
constexpr uint8_t REQUESTED_DATA_ALL = 0; // "All"

constexpr uint16_t PARAM_REQUESTED_DATA = 0x0001;

}
