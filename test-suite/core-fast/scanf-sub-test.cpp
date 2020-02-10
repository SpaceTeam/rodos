#include "rodos.h"

#include "scanf-substitue.h"

#define FAIL {PRINTF("FAILED at line %d in file %s\n", __LINE__, __FILE__); failed++;};


class ScanfSubTests: public Thread {
    void run() {
        int failed = 0;
    
        Tokenizer tok;
        tok.separators = (char*)"abc";
        char strintToParse[] = "123c456a789b";
        tok.init(strintToParse);
        
        if (tok.isSeparator('A'))  FAIL;
        if (!tok.isSeparator('a')) FAIL;
        if (!tok.isSeparator('b')) FAIL;
        if (!tok.isSeparator('c')) FAIL;
        
        PRINTF("string to parse : %s\n", tok.ptr);
        char* subStr;
        while((subStr = tok.next()) != 0)  PRINTF ("substring  %s\n", subStr);
        
        hwResetAndReboot();
    }
} sst;
 
 
