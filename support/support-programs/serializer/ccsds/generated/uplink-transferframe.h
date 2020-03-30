

//--- generated file, DO NOT modify
//--- see bitFieldsSerializer
#pragma once


namespace CCSDS {
struct UplinkTFHeader {
    static const int HEADER_SIZE = 6;
    uint32_t version             ; //  2  bits  set to 0
    uint32_t bypassFlag          ; //  1  bits  0->A: normal (check SeqNr),  1->B: Bypass, accept all cmds
    uint32_t controllCommandFlag ; //  1  bits  0->D (data upload protocoll) 1 -> C (Command protocoll)
    uint32_t spare1              ; //  2  bits  set to 0
    uint32_t spacecraftID        ; // 10  bits  
    uint32_t virtualChannelID    ; //  6  bits  
    uint32_t frameLength         ; // 10  bits  frameLength = real_len - 1
    uint32_t frameSequenceNr     ; //  8  bits  
    uint32_t sequenceFlags       ; //  2  bits  set to 0x3 
    uint32_t multiplexAceessPoint; //  6  bits  set to 0x3 , eg to select target computer

    int serialize(uint8_t * buf) const;
    int deserialize(const uint8_t * buf);
};


inline int UplinkTFHeader::serialize(uint8_t* buf) const {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    setBitField(buf,   0,  2, version);
    setBitField(buf,   2,  1, bypassFlag);
    setBitField(buf,   3,  1, controllCommandFlag);
    setBitField(buf,   4,  2, spare1);
    setBitField(buf,   6, 10, spacecraftID);
    setBitField(buf,  16,  6, virtualChannelID);
    setBitField(buf,  22, 10, frameLength);
    buf[4]      =             frameSequenceNr;
    setBitField(buf,  40,  2, sequenceFlags);
    setBitField(buf,  42,  6, multiplexAceessPoint);

    return 6;
}
inline int UplinkTFHeader::deserialize(const uint8_t * buf) {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    version          = getBitField(buf,   0,  2);
    bypassFlag       = getBitField(buf,   2,  1);
    controllCommandFlag = getBitField(buf,   3,  1);
    spare1           = getBitField(buf,   4,  2);
    spacecraftID     = getBitField(buf,   6, 10);
    virtualChannelID = getBitField(buf,  16,  6);
    frameLength      = getBitField(buf,  22, 10);
    frameSequenceNr  = buf[4];
    sequenceFlags    = getBitField(buf,  40,  2);
    multiplexAceessPoint = getBitField(buf,  42,  6);

    return 6;
}
} // namespace end

