

/**
* @file rodos-debug.cpp
* @date 2008/06/17 10:48
* @author Lutz Dittrich & Sergio Montenegro,
*   adapted in 2010 by Henrik Theiling (AbsInt Angewandte Informatik GmbH)
*   SNPRINTF added in 2019 by Sebastian Rückerl (TUM
*
* @brief debug and printf functions
*
*/

#include "rodos.h"

namespace RODOS {

Semaphore printfProtector;

void PRINTF(const char* fmt, ...) {
    if(printfMask == 0) return;
    Yprintf yprintf;
    va_start(yprintf.ap, fmt);
    if (rodosErrorCounter != 0) xprintf("prev-ERR(%ld) -- ", rodosErrorCounter);
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
    if (rodosErrorCounter != 0) xprintf("prev-ERR(%ld) -- ", rodosErrorCounter);
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


void RODOS_ERROR(const char* text) {
  rodosErrorMsg = text;
  rodosErrorCounter++;
  PRINTF("!! Programming-ERROR %s\n",text);
  errorLog.addRaw(text);
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

class SNYprintf : public Yprintf {
public:
    char* dest;
    size_t max_size;
    size_t cur_size;

    SNYprintf(char* _dest, size_t _size)
        : dest(_dest)
        , max_size(_size)
        , cur_size(0)      {}

    ~SNYprintf() {
        if (max_size != 0 ) { 
            *dest = '\0'; 
        }
    }

    void yputc(char c) {  
        if (cur_size + 1 < max_size) {
            *dest = c;
            dest++;
            cur_size++;
        }
    }
};

int vsnprintf(char*dest, size_t size, const char* fmt, va_list args) {
    SNYprintf yprintf(dest, size);
    va_copy(yprintf.ap, args);
    if (!isSchedulerRunning()) {
        yprintf.vaprintf(fmt);
    } else {
        printfProtector.enter();
            yprintf.vaprintf(fmt);
        printfProtector.leave();
    }
    return yprintf.cur_size;
}

int SNPRINTF(char * dest, size_t size, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int written_size = vsnprintf(dest, size, fmt, args);
    va_end(args);
    return written_size;
}

void SPRINTF(char* dest, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsnprintf(dest, SPRINTF_MAX_SIZE, fmt, args);
    va_end(args);
}



}


