#pragma once

namespace RODOS {

inline uint8_t trailing_zeros(uint32_t word){
    return static_cast<uint8_t>(__builtin_ctzl(word));
}

}
