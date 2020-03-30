

//--- generated file, DO NOT modify
//--- see bitFieldsSerializer
#pragma once


namespace CCSDS {
struct DownlinkSPHeader {
    static const int HEADER_SIZE = 16;
    uint32_t version             ; //  3  bits  just ignore! set to 0
    uint32_t typeId              ; //  1  bits  shall be 0!
    uint32_t secHeaderFlag       ; //  1  bits  1 -> secondary header follows
    uint32_t applicationId       ; // 11  bits  normaly called APID, for idle set to 11111111111 = 0x7ff
    uint32_t groupingFlags       ; //  2  bits  00: cont packet, 01: first one, 10: last one, 11: no groupping (11 Recommended!)
    uint32_t sourceSeqCnt        ; // 14  bits  please count only not idle packeges
    uint32_t dataPackLen         ; // 16  bits  Warning! = len(secondaryheader + userdata) - 1 , in our case here 10 + userdatalen - 1
    uint32_t secondaryFlag       ; //  1  bits  shall be 0
    uint32_t pusVersion          ; //  3  bits  ignore! set to 1
    uint32_t spare               ; //  4  bits  shall be 0
    uint32_t service             ; //  8  bits  3  = normal bus telemetry
    uint32_t subservice          ; //  8  bits  25 = Automatic HKD
    uint32_t destination         ; //  8  bits  0 for Groundstation (normal case) ; WARNING: Some expect here source (not dest)
    uint32_t timeStampSeconds    ; // 32  bits  UTC in seconds since 1.1.2000  0:00, set by Housekeeper
    uint32_t timeStampFraction   ; // 16  bits  (1/2^16) of the second

    int serialize(uint8_t* buf) const;
    int deserialize(const uint8_t* buf);
};


inline int DownlinkSPHeader::serialize(uint8_t* buf) const {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    setBitField(buf,   0,  3, version);
    setBitField(buf,   3,  1, typeId);
    setBitField(buf,   4,  1, secHeaderFlag);
    setBitField(buf,   5, 11, applicationId);
    setBitField(buf,  16,  2, groupingFlags);
    setBitField(buf,  18, 14, sourceSeqCnt);
    uint16_tToBigEndian(buf+4,  dataPackLen);
    setBitField(buf,  48,  1, secondaryFlag);
    setBitField(buf,  49,  3, pusVersion);
    setBitField(buf,  52,  4, spare);
    buf[7]      =             service;
    buf[8]      =             subservice;
    buf[9]      =             destination;
    uint32_tToBigEndian(buf+10,  timeStampSeconds);
    uint16_tToBigEndian(buf+14,  timeStampFraction);

    return 16;
}
inline int DownlinkSPHeader::deserialize(const uint8_t * buf) {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    version          = getBitField(buf,   0,  3);
    typeId           = getBitField(buf,   3,  1);
    secHeaderFlag    = getBitField(buf,   4,  1);
    applicationId    = getBitField(buf,   5, 11);
    groupingFlags    = getBitField(buf,  16,  2);
    sourceSeqCnt     = getBitField(buf,  18, 14);
    dataPackLen      = bigEndianToUint16_t(buf+4);
    secondaryFlag    = getBitField(buf,  48,  1);
    pusVersion       = getBitField(buf,  49,  3);
    spare            = getBitField(buf,  52,  4);
    service          = buf[7];
    subservice       = buf[8];
    destination      = buf[9];
    timeStampSeconds = bigEndianToUint32_t(buf+10);
    timeStampFraction = bigEndianToUint16_t(buf+14);

    return 16;
}
} // namespace end

