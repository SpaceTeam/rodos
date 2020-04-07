

//--- generated file, DO NOT modify
//--- see bitFieldsSerializer
#pragma once


namespace CCSDS {
struct UplinkTFTrailer {
    static const int HEADER_SIZE = 2;
    uint16_t crc                 ; // 16  bits  cyclic redundancy code in CCSDS called "FRAME ERROR CONTROL FIELD"

    int serialize(uint8_t * buf) const;
    int deserialize(const uint8_t * buf);
};


inline int UplinkTFTrailer::serialize(uint8_t* buf) const {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    uint16_tToBigEndian(buf+0,  crc);

    return 2;
}
inline int UplinkTFTrailer::deserialize(const uint8_t * buf) {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    crc              = bigEndianToUint16_t(buf+0);

    return 2;
}
} // namespace end

