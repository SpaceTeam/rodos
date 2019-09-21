

//--- generated file, DO NOT modify
//--- see bitFieldsSerializer
#pragma once


namespace CCSDS {
struct DownlinkTFHeader {
    static const int HEADER_SIZE = 6;
    uint32_t version             ; //  2  bits  set to 0
    uint32_t spaceCraftId        ; // 10  bits  
    uint32_t virtualChanId       ; //  3  bits  0: Real time, 1 history, 7 idle
    uint32_t opControlFlag       ; //  1  bits  1 -> at the end of the TF is a 32-bit CLCW command status word
    uint32_t masterChanFrameCnt  ; //  8  bits  
    uint32_t virtualChanFrameCnt ; //  8  bits  
    uint32_t secondHeaderFlag    ; //  1  bits  1-> there is a second. header (I recomend: NO secondary header)
    uint32_t synchFlag           ; //  1  bits  set to 0
    uint32_t packetOrderFlag     ; //  1  bits  set to 0
    uint32_t segmentLenId        ; //  2  bits  set to 11
    uint32_t firstHeaderPtr      ; // 11  bits  normaly set to 0 , for idle frames set to 11111111110 (0x7fe)

    int serialize(char * const buf) const;
    int deserialize(char const * const buf);
};


inline int DownlinkTFHeader::serialize(char* const b) const {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    unsigned char* buf = (unsigned char*)b;
    setBitField(buf,   0,  2, version);
    setBitField(buf,   2, 10, spaceCraftId);
    setBitField(buf,  12,  3, virtualChanId);
    setBitField(buf,  15,  1, opControlFlag);
    buf[2]      =             masterChanFrameCnt;
    buf[3]      =             virtualChanFrameCnt;
    setBitField(buf,  32,  1, secondHeaderFlag);
    setBitField(buf,  33,  1, synchFlag);
    setBitField(buf,  34,  1, packetOrderFlag);
    setBitField(buf,  35,  2, segmentLenId);
    setBitField(buf,  37, 11, firstHeaderPtr);

    return 6;
}
inline int DownlinkTFHeader::deserialize(char const * const b) {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    unsigned char* buf = (unsigned char*)b; // becouse deserialize is const, but getBitFiled not!
    version          = getBitField(buf,   0,  2);
    spaceCraftId     = getBitField(buf,   2, 10);
    virtualChanId    = getBitField(buf,  12,  3);
    opControlFlag    = getBitField(buf,  15,  1);
    masterChanFrameCnt = buf[2];
    virtualChanFrameCnt = buf[3];
    secondHeaderFlag = getBitField(buf,  32,  1);
    synchFlag        = getBitField(buf,  33,  1);
    packetOrderFlag  = getBitField(buf,  34,  1);
    segmentLenId     = getBitField(buf,  35,  2);
    firstHeaderPtr   = getBitField(buf,  37, 11);

    return 6;
}
} // namespace end

