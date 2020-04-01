

//--- generated file, DO NOT modify
//--- see bitFieldsSerializer
#pragma once


namespace CCSDS {
struct UplinkSPHeader {
    static const uint32_t HEADER_SIZE = 10;
    uint32_t version             ; //  3  bits  set to 0
    uint32_t type                ; //  1  bits  1 -> telecommand
    uint32_t secondaryHeaderFlag ; //  1  bits  set to 1 (We have a PUS header)
    uint32_t applicationId       ; // 11  bits  
    uint32_t sequenceFlags       ; //  2  bits  set 3 for standalone
    uint32_t sequenceCounter     ; // 14  bits  
    uint32_t length              ; // 16  bits  real_len - 1
    uint32_t pusSecondaryHeaderFlag; //  1  bits  set to 0
    uint32_t pusVersion          ; //  3  bits  set to 1
    uint32_t ackType             ; //  4  bits  4x Bool: (completion, execution, start, acceptance)
    uint32_t serviceType         ; //  8  bits  
    uint32_t serviceSubtype      ; //  8  bits  
    uint32_t sourceID            ; //  8  bits  set to  0

    int serialize(uint8_t * buf) const;
    int deserialize(const uint8_t * buf);
};


inline int UplinkSPHeader::serialize(uint8_t* buf) const {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    setBitField(buf,   0,  3, version);
    setBitField(buf,   3,  1, type);
    setBitField(buf,   4,  1, secondaryHeaderFlag);
    setBitField(buf,   5, 11, applicationId);
    setBitField(buf,  16,  2, sequenceFlags);
    setBitField(buf,  18, 14, sequenceCounter);
    uint16_tToBigEndian(buf+4,  length);
    setBitField(buf,  48,  1, pusSecondaryHeaderFlag);
    setBitField(buf,  49,  3, pusVersion);
    setBitField(buf,  52,  4, ackType);
    buf[7]      =             serviceType;
    buf[8]      =             serviceSubtype;
    buf[9]      =             sourceID;

    return 10;
}
inline int UplinkSPHeader::deserialize(const uint8_t* buf) {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    version          = getBitField(buf,   0,  3);
    type             = getBitField(buf,   3,  1);
    secondaryHeaderFlag = getBitField(buf,   4,  1);
    applicationId    = getBitField(buf,   5, 11);
    sequenceFlags    = getBitField(buf,  16,  2);
    sequenceCounter  = getBitField(buf,  18, 14);
    length           = bigEndianToUint16_t(buf+4);
    pusSecondaryHeaderFlag = getBitField(buf,  48,  1);
    pusVersion       = getBitField(buf,  49,  3);
    ackType          = getBitField(buf,  52,  4);
    serviceType      = buf[7];
    serviceSubtype   = buf[8];
    sourceID         = buf[9];

    return 10;
}
} // namespace end

