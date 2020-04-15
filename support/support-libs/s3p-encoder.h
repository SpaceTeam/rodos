
#pragma once

/*
 * S3P Message framing, 
 * @author: Sergio Montenegro, Sept 2019
 */

#include <stdint.h>
#include "thread.h"

namespace RODOS {

class S3pEncoder {

  public:
    // ___________________________________________________ You shall override these methods
    virtual void putByte([[gnu::unused]] uint8_t c) {} // override to send or store encoded messages (maybe you shall proctedt with semaphore?)
    // __________________________________________________________

    bool mayPutBytes = true; // your putByte() shall wait to send until this is true, see STOP/CONTINUE from receiver

    static const uint16_t BOM      = 0xFE02; // independent of endinaty we send MSF (Most Significant First)
    static const uint16_t EOM      = 0xFE03;
    static const uint16_t STUFF    = 0xFE7E;
    static const uint16_t STOP     = 0xFE13;
    static const uint16_t CONTINUE = 0xFE11;
    static const uint16_t SYNC     = 0xFE16;
    static const uint16_t COMMAND  = 0xFE00;
    static const uint8_t  MARK     = 0xFE;

    inline bool isCommand(uint16_t c) { return (c & 0xff00) == COMMAND; }
    inline bool isMark(uint16_t c) { return (c & 0x00ff) == MARK; }
    inline bool hasMark(uint16_t c) { return isCommand(c) || isMark(c); }

    //_____________________________________ To encode messages

    inline void putDualByte(uint16_t c) {
        PRIORITY_CEILER_IN_SCOPE();
        putByte(static_cast<uint8_t>(c >> 8));
        putByte(static_cast<uint8_t>(c & 0xff));
    } // bigendian, MSF

    inline void putByteEncoded(uint8_t c) {
        if(isMark(c))
            putDualByte(STUFF);
        else
            putByte(c);
    }

    inline void sendMsg(int len, uint8_t* s) {
        putDualByte(BOM);
        for(int i = 0; i < len; i++) putByteEncoded(*s++);
        putDualByte(EOM);
    }

    inline void sendSync() { putDualByte(SYNC); }

    // ___________________________________________________ You shall override these methods
    virtual void syncTime() {} // override to sync time will be called from getMsgByWaiting() and/or upcallOnInputChar()
   
    //_____________________________________ To decode messages Alternative 1 : busy waiting, polling

    virtual uint8_t getByte() { return 0; } //override to allow me to read characters. Waits until data is ready

    int getMsgByPolling(int maxlen, uint8_t* s); // uses your getByte() and suspend/waits (depending on getByte())

    //_____________________________________ To decode messages Alternative 2: as interrupt server

    virtual void appendByte([[gnu::unused]] uint8_t c) {} // upcallOnInputChar()  will call this method, each time It gives you a byte for your message
    virtual void endOfMessage([[gnu::unused]] int len) {} // upcallOnInputChar()  will call this method when one message is ready. It gives you the length

    void upcallOnInputChar(uint8_t inputChar); // the interrupt server shall call this for each arrived byte

    virtual ~S3pEncoder() = default;
};

} // namespace RODOS
