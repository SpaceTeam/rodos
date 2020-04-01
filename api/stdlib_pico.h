#pragma once

#include <stdint.h>

namespace RODOS {

int isspace(char c);
int isdigit(char c);
int isalpha(char c);
int isupper(char c);

int32_t strtol (const char * nptr, const char ** endptr, uint8_t base);

}  // namespace


