#pragma once

//#include <stddef.h>

namespace RODOS {

int isspace(int c);
int isdigit(int c);
int isalpha(int c);
int isupper(int c);

long int strtol (const char * nptr, char ** endptr, int base);

}  // namespace


