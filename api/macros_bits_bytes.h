
#pragma once

#include <stdint.h>

/**
* @file Macros.h
* @date 2008/11/21 11:46
* @author Sergio Montenegro
*
*
*/


/* WARNING:
 * This file is not included in rodos.h or rodos.h
 * some drivers from chip provide has different meaning for SET_BIT and GET_BIT! (eg. a mask)
 * to avoid conflicts you shall include this file by your self.
 */

namespace RODOS {

constexpr uint8_t ZERO_IN_SENSE_OF_UNDEF_OR_ERR = 0; ///< this is more clear than just writing 0

template <typename T>
constexpr T min(const T a, const T b) {
    return (a < b) ? a : b;
}

constexpr uint32_t uint32_tOnes(uint8_t n) {
    return (static_cast<uint32_t>(1)  << n) - static_cast<uint32_t>(1);
}


}  // namespace

