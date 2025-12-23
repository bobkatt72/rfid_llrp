#pragma once

#include <cstdint>
#include <vector>

namespace llrp
{
std::vector<uint8_t> encode_get_reader_capabilities(uint32_t message_id);
}
