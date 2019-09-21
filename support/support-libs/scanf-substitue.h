
#pragma once
#include "stdint.h"

//  substitut for scanf , simple scanf funcions

// breaking a stream of text up into words separated by blanc or coma
struct Tokenizer {
    char* ptr;
    char* separators;

    Tokenizer() { separators = (char*)" ,;\n";}
    bool isSeparator(char c);
    void init(char* s) { ptr = s; };
    void setSeparators(char *separatorsP) { separators = separatorsP; }
    bool valid() { return ptr != 0 && *ptr != 0; }
    char* next(); // pointer to next tocken, will  be terminated by 0
    void  getAll(char* token[], int maxTokens) { for(int i = 0; i < maxTokens; i++) token[i] = next(); }
};

//terminated by 0, eg tockes from Tocknizer
#define atoi s2int
extern int64_t s2int(char *s);

extern char* strCpyUntilChar(char* dest, char* source, char terminator);

