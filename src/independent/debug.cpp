

/**
* @file debug.cpp
* @date 2008/06/17 10:48
* @author Lutz Dittrich & Sergio Montenegro,
*         adapted in 2010 by Henrik Theiling (AbsInt Angewandte Informatik GmbH)
*
*
* @brief debug functions
*
*/

#include "rodos.h"

namespace RODOS {

Semaphore printfProtector;

void PRINTF(const char* fmt, ...) {
    if(printfMask == 0) return;
    Yprintf yprintf;
    va_start(yprintf.ap, fmt);
    if (errorCounter != 0) xprintf("prev-ERR(%ld) -- ", errorCounter);
    if (!isSchedulerRunning()) {
        yprintf.vaprintf(fmt);
    } else {
        printfProtector.enter();
            yprintf.vaprintf(fmt);
        printfProtector.leave();
    }
    FFLUSH();
}

void PRINTF_CONDITIONAL(uint32_t id, const char* fmt, ...) {
    if(id != 0xffffffff && (id & printfMask) == 0) return;
    Yprintf yprintf;
    va_start(yprintf.ap, fmt);
    if (errorCounter != 0) xprintf("prev-ERR(%ld) -- ", errorCounter);
    if (!isSchedulerRunning()) {
        yprintf.vaprintf(fmt);
    } else {
        printfProtector.enter();
            yprintf.vaprintf(fmt);
        printfProtector.leave();
    }
    FFLUSH();
}

//_________________________________________________________________-

class SYprintf : public Yprintf {
public:
    char* dest;
    SYprintf(char* _dest) {  dest = _dest; }
    ~SYprintf()           { *dest = 0; }
    void yputc(char c)    { *dest = c; dest++; }
};



void SPRINTF(char* dest, const char* fmt, ...) {
    SYprintf yprintf(dest);
    va_start(yprintf.ap, fmt);
    if (!isSchedulerRunning()) {
        yprintf.vaprintf(fmt);
    } else {
        printfProtector.enter();
            yprintf.vaprintf(fmt);
        printfProtector.leave();
    }
}


void ERROR(const char* text) {
  // errorLog.add(text); WARNING: Crashes in called in constcutros whitout thread
  errorMsg = text;
  PRINTF("!! Programming-ERROR %s\n",text);
  errorCounter++;
}


//void printBin(unsigned char val) {
//	PRINTF(" ");
//	for(int i = 0; i < 8; i++) {
//		PRINTF("%c", (val & 0x80)? '1' : '0');
//		val = val << 1;
//	}
//}

void printCharsHex(const char* titel, void* ptr, unsigned int len) {
	unsigned char* vals = (unsigned char*)ptr;
	PRINTF("%s", titel);
	for(unsigned int i = 0; i < len; i++) PRINTF("%02x ", vals[i]);
	PRINTF("\n");
}

void printCharsHex_Conditional(uint32_t id, const char* titel, void* ptr, unsigned int len) {
        if((id & printfMask) == 0 ) return;
        printCharsHex(titel,  ptr, len); 
}

char* formatBinary(long val, int len, char* outputBuf) {
	val <<= 32 - len;
	for(int i = 0; i < len; i++) {
		outputBuf[i] = ( (val & 0x80000000)? '1' : '0');
		val <<= 1;
	}
	outputBuf[len] = 0;
	return outputBuf;
}


}


