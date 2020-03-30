#include <stdint.h>
#include "stream-bytesex.h"
#include "macros_bits_bytes.h"

/** Bytesex Convertions
 *  byte string allwas in Big-endian (internet, ccsds, net, motorola) format
 *
 * Author    : Sergio Montenegro
 * Date      : 23.10.2008
 */

/************ From a big-endian byte stram  to internal representation ****/

#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif

// TODO: SWAPCHARS is already in macros.h
/// exchange values of char variables (a) and (b)
#define SWAPCHARS(_a, _b) {char _tempswap = _a;   _a = _b;   _b = _tempswap; }

int16_t swap16(int16_t sw_);
int32_t swap32(int32_t sw_);
int64_t swap64(int64_t sw_);
float   swapFloat(float   sw_); 
double  swapDouble(double  sw_);
int16_t int16ConvertHost2Net(int16_t sw_); 
int32_t int32ConvertHost2Net(int32_t sw_);
int64_t int64ConvertHost2Net(int64_t sw_); 
float   floatConvertHost2Net(float   sw_); 
double  doubleConvertHost2Net(double sw_); 

bool isHostBigEndian = false;

int16_t swap16(int16_t sw_) {
    int16_t sw = sw_;
    char *c = (char*)&sw;
    SWAPCHARS(c[0], c[1]);
    return sw;
}

int32_t swap32(int32_t lw_) {
    int32_t lw = lw_;
    char *c = (char*)&lw;
    SWAPCHARS(c[0], c[3]);
    SWAPCHARS(c[1], c[2]);
    return lw;
}

float swapFloat(float fw_) {
    float fw = fw_;
    char *c = (char*)&fw;
    SWAPCHARS(c[0], c[3]);
    SWAPCHARS(c[1], c[2]);
    return fw;
}


int64_t swap64(int64_t llw_) {
    int64_t  llw = llw_;
    char *c = (char*)&llw;
    SWAPCHARS(c[0], c[7]);
    SWAPCHARS(c[1], c[6]);
    SWAPCHARS(c[2], c[5]);
    SWAPCHARS(c[3], c[4]);
    return llw;
}

double swapDouble(double dw_) {
    double  dw = dw_;
    char *c = (char*)&dw;
    SWAPCHARS(c[0], c[7]);
    SWAPCHARS(c[1], c[6]);
    SWAPCHARS(c[2], c[5]);
    SWAPCHARS(c[3], c[4]);
    return dw;
}

int16_t int16ConvertHost2Net(int16_t sw) {
    if(isHostBigEndian) return sw;
    return swap16(sw); 
}

int32_t int32ConvertHost2Net(int32_t lw) {
    if(isHostBigEndian) return lw;
    return swap32(lw); 
}

int64_t int64ConvertHost2Net(int64_t llw) {
    if(isHostBigEndian) return llw;
    return swap64(llw); 
}

float floatConvertHost2Net(float fw) {
    if(isHostBigEndian) return fw;
    return swapFloat(fw); 
}

double doubleConvertHost2Net(double dw) {
    if(isHostBigEndian) return dw;
    return swapDouble(dw); 
}


/// DEPRECATED... 
short shortConvertHost2Net(short sw) { if(isHostBigEndian) return sw; return swap16(sw); } 
/// DEPRECATED... 
long  longConvertHost2Net(long lw) { if(isHostBigEndian) return lw; return swap32(lw); }
/// DEPRECATED... 
long long longlongConvertHost2Net(long long llw) { if(isHostBigEndian) return llw; return swap64(llw); } 


//__________________________________________________________________________________________________________

uint16_t bigEndianToUint16_t(const void* buff) {
    uint8_t* byteStream = (uint8_t*)buff;
    if (byteStream == 0) return 0;
    return   (((uint16_t)(byteStream[0])) << 8)
           |  ((uint16_t)(byteStream[1]));
}

uint32_t bigEndianToUint32_t(const void* buff) {
    uint8_t* byteStream = (uint8_t*)buff;
    if (byteStream == 0) return 0;
    return    (((uint32_t)(byteStream[0])) << 24)
            | (((uint32_t)(byteStream[1])) << 16)
            | (((uint32_t)(byteStream[2])) <<  8)
            |  ((uint32_t)(byteStream[3]));
}


uint64_t bigEndianToUint64_t(const void* buff) {
    uint8_t* byteStream = (uint8_t*)buff;
    if (byteStream == 0) return 0;
    return   (((uint64_t)(byteStream[0])) << 56)
           | (((uint64_t)(byteStream[1])) << 48)
           | (((uint64_t)(byteStream[2])) << 40)
           | (((uint64_t)(byteStream[3])) << 32)
           | (((uint64_t)(byteStream[4])) << 24)
           | (((uint64_t)(byteStream[5])) << 16)
           | (((uint64_t)(byteStream[6])) <<  8)
           |  ((uint64_t)(byteStream[7]));
}


float bigEndianToFloat(const void* buff) {
    uint8_t* byteStream = (uint8_t*)buff;
    union {
        float value;
        uint32_t lvalue;
    } value_union;
    value_union.lvalue = bigEndianToInt32_t(byteStream);
    return value_union.value;
}

double bigEndianToDouble(const void* buff) {
    uint8_t* byteStream = (uint8_t*)buff;
    union {
        double value;
        uint64_t llvalue;
    } value_union;
    value_union.llvalue = bigEndianToInt64_t(byteStream);
    return value_union.value;
}

/************ From internal representation to big-endian  byte stram  ****/

void uint16_tToBigEndian(void* buff, uint16_t value) {
    uint8_t* byteStream = (uint8_t*)buff;
    if (byteStream == 0) return;
    byteStream[0] = (uint8_t) ((value >> 8) & 0xFF);
    byteStream[1] = (uint8_t) ((value >> 0) & 0xFF);
}

void uint32_tToBigEndian(void* buff, uint32_t value) {
    uint8_t* byteStream = (uint8_t*)buff;
    if (byteStream == 0) return;
    byteStream[0] = (uint8_t) ((value >> 24) & 0xFF);
    byteStream[1] = (uint8_t) ((value >> 16) & 0xFF);
    byteStream[2] = (uint8_t) ((value >>  8) & 0xFF);
    byteStream[3] = (uint8_t) ((value >>  0) & 0xFF);
}


void uint64_tToBigEndian(void* buff, uint64_t value) {
    uint8_t* byteStream = (uint8_t*)buff;
    if (byteStream == 0) return;
    byteStream[0] = (uint8_t) ((value >> 56) & 0xFF);
    byteStream[1] = (uint8_t) ((value >> 48) & 0xFF);
    byteStream[2] = (uint8_t) ((value >> 40) & 0xFF);
    byteStream[3] = (uint8_t) ((value >> 32) & 0xFF);
    byteStream[4] = (uint8_t) ((value >> 24) & 0xFF);
    byteStream[5] = (uint8_t) ((value >> 16) & 0xFF);
    byteStream[6] = (uint8_t) ((value >>  8) & 0xFF);
    byteStream[7] = (uint8_t) ((value >>  0) & 0xFF);
}

int16_t bigEndianToInt16_t (const void* byteStream) { return static_cast<int16_t>( bigEndianToUint16_t(byteStream) ); }
int32_t bigEndianToInt32_t (const void* byteStream) { return static_cast<int32_t>( bigEndianToUint32_t(byteStream) ); }
int64_t bigEndianToInt64_t (const void* byteStream) { return static_cast<int64_t>( bigEndianToUint64_t(byteStream) ); }
void    int16_tToBigEndian (void* byteStream, int16_t value) { uint16_tToBigEndian(byteStream, static_cast<uint16_t>(value)); }
void    int32_tToBigEndian (void* byteStream, int32_t value) { uint32_tToBigEndian(byteStream, static_cast<uint32_t>(value)); }
void    int64_tToBigEndian (void* byteStream, int64_t value) { uint64_tToBigEndian(byteStream, static_cast<uint64_t>(value)); }

void floatToBigEndian(void* buff, float value_) {
    uint8_t* byteStream = (uint8_t*)buff;
    union {
        float value;
        unsigned lvalue;
    } value_union;
    value_union.value = value_;
    int32_tToBigEndian(byteStream, value_union.lvalue);
}

void doubleToBigEndian   (void* buff, double value_) {
    uint8_t* byteStream = (uint8_t*)buff;
    union {
        double value;
        uint64_t llvalue;
    } value_union;
    value_union.value = value_;
    int64_tToBigEndian(byteStream, value_union.llvalue);
}


/*************************************************/

void setBitInByteStream (void *byteStream, int bitIndex, bool value) {
    int byteIndex = bitIndex / 8;
    int bitPos =    bitIndex % 8;
    unsigned char* bytes = (unsigned char*)byteStream;

    SET_BIT_IN_BYTE(bytes[byteIndex], bitPos, (value? 1 : 0));
}


int getBitFromByteStream(const void *byteStream, int bitIndex) {

    unsigned char* bytes = (unsigned char*)byteStream;

    unsigned char selectedByte = bytes[bitIndex/8];
    return GET_BIT_FROM_BYTE(selectedByte, (bitIndex % 8));

}


/***********************************************************/


/** Sets a bit field form 0 to 16 bits for CCSDS bitd fields 
  * Warning: CCSDS -> Bit 0 = most significant bit!
  **/

void setBitField(void* buffer, int bitPos, int numOfBits, uint32_t val) {
    unsigned char* buf = (unsigned char*) buffer;
    int byteIndex = bitPos / 8;
    bitPos        = bitPos % 8;
    int shifts    = 24 - (bitPos + numOfBits);
    int32_t mask     = ONES(numOfBits) << shifts;

    val = val << shifts;

    // get the word as big-endian (CPU independent)
    uint32_t word    = (buf[byteIndex] << 16);
	if(bitPos+numOfBits > 8 ) word |=  buf[byteIndex+1] << 8; //Do not read byte not required to avoid reading beyond the buffer
	if(bitPos+numOfBits > 16) word |=  buf[byteIndex+2];

    word  &= ~ mask;        // Clear bit field
    word  |=  mask & val;   // set the correspondig bits

    // store the word as big-endian (CPU independent)
    buf[byteIndex]                             = (word >> 16) & 0xff; // Most Significant first
    if(bitPos+numOfBits > 8 ) buf[byteIndex+1] = (word >> 8 ) & 0xff;
    if(bitPos+numOfBits > 16) buf[byteIndex+2] =  word        & 0xff;
}


/** gets a bit field form 0 to 16 bits for CCSDS bitd fields 
  * Warning: CCSDS -> Bit 0 = most significant bit!
  **/

uint32_t getBitField(const void* buffer, int bitPos, int numOfBits) {
    const uint8_t* buf = (const uint8_t*) buffer;
    int byteIndex = bitPos / 8;
    bitPos        = bitPos % 8;
    int shifts    = 24 - (bitPos + numOfBits);
    int32_t mask     = ONES(numOfBits);  // so many bits set to 1 from leastst significant


    // get the word as big-endian (CPU independent)
    uint32_t word    = (buf[byteIndex] << 16);
    if(bitPos+numOfBits > 8 ) word |=  buf[byteIndex+1] << 8; //Do not read byte not required to avoid reading beyond the buffer
    if(bitPos+numOfBits > 16) word |=  buf[byteIndex+2];

    return  (word >> shifts) &  mask;
}

// counts number of bits set to 1
uint32_t getSetBits(const uint32_t &value) {

    // reference: Hacker's Delight, p. 66, Figure 5-2
    uint32_t x = value;
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x + (x >> 4)) & 0x0F0F0F0F;
    x = x + (x >> 8);
    x = x + (x >> 16);
    return x & 0x0000003F;
}

#ifndef NO_RODOS_NAMESPACE
}
#endif


