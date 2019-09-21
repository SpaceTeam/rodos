

//--- generated file, DO NOT modify
//--- see bitFieldsSerializer
#pragma once


namespace CCSDS {
struct UplinkTFTrailer {
    static const int HEADER_SIZE = 2;
    uint32_t crc                 ; // 16  bits  cyclic redundancy code in CCSDS called "FRAME ERROR CONTROL FIELD"

    int serialize(char * const buf) const;
    int deserialize(char const * const buf);
};


inline int UplinkTFTrailer::serialize(char* const b) const {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    unsigned char* buf = (unsigned char*)b;
    uint16_tToBigEndian(buf+0,  crc);

    return 2;
}
inline int UplinkTFTrailer::deserialize(char const * const b) {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    unsigned char* buf = (unsigned char*)b; // becouse deserialize is const, but getBitFiled not!
    crc              = bigEndianToUint16_t(buf+0);

    return 2;
}
} // namespace end

