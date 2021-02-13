#pragma once

/*
 * S3P Message framing, 
 * @author: Sergio Montenegro, Sept 2019, 2021
 */

/** please read s3p-encoder-in-byte stream.pdf **/

#include <stdint.h>
#include "s3p-code.h"

namespace RODOS {

/*
 * Triggered from Application, Data Flow from Application to UART/RadioLink (Send/put)
 */

/*
 * To encode an send a message. 
 * Case 1: Synchronous sending. The calling function (upper SW layers) has the control
 * the sending channel is always ready to get data, e.g. from memory to memory,
 * or else busy waiting  until the sending channel (UART/Radiolink) is ready to get data.
 * The Upcall/Virtual function is on the side of the UART/Radiolink (lower SW layers)
 */


// TODO: Return and propagate Error codes
class S3pSenderSyncrhonous : public S3pCode {

  private:
    // _________________________ UPCALL _________________ You shall override this method to write to the UART/Radiolink
    virtual void putByte([[gnu::unused]] uint8_t c) {} // override to send or store encoded messages, (busy waiting) Wait until data is written!

  public:
    // Calls from the application side

    inline void putDataByte(const uint8_t c)  {   
        if(c == MARK) putCommand(STUFF);
        else putByte(c);
    }
    inline void putCommand(const uint16_t c) { // eg SYNC, BOM, EOM, STOP.... or ... normal data byte
        if(c == NODATA) return;
        putByte(msb(c)); // upcall to your virtual putByte
        putByte(lsb(c));
    }
    inline void sendMsg(const int len, const uint8_t* s) {
        putCommand(BOM);
        for(int i = 0; i < len; i++) putDataByte(*s++);
        putCommand(EOM);
    }
};


/*
 * To receive messages from UART/Radiolink and decode.
 * Case 1: Synchronous receiving. The calling function (upper SW layers) has the control
 * the receiving channel is always ready to provide data, e.g. from memory to memory,
 * or else busy waiting  until the receiving channel (UART/Radiolink) is ready to provide data.
 * The Upcall/Virtual function is on the side of the UART/Radiolink (lower SW layers)
 */

/*
 * Triggered from Application, Data Flow from UART/RadioLink to Application (get)
 */

class S3pReceiverSyncrhonous : S3pCode {
  private:
    // _________________________ UPCALL _________________ You shall override this method to read messages from the UART/Radiolink
    virtual uint8_t getByte() { return 0; } // override to read from UART/Radiolink encoded messages. (busy waiting) Wait until data is ready
    //___________________________________________________

  public:
    // Calls from the application side

    inline uint16_t getDualByte() {
        uint8_t c = getByte();
        if(c != MARK) return (uint16_t)c;                 // normal data, 0x00XX
        return (uint16_t)(COMMAND | (uint16_t)getByte()); // we get most significant first now least significant
    }

    int getMsg(int maxLen, uint8_t* msg) {
        int len = -1;
        while(1) {
            if(len >= maxLen) return maxLen;
            uint16_t dualByte = getDualByte();  // Warning: it suspends until data arrives

            switch(dualByte) {
                case NODATA:                                 break;
                case STOP:     mayPutBytes = false;          break;
                case CONTINUE: mayPutBytes = true;           break;
                case SYNC:     syncTime();                   break;
                case BOM:      len = 0;                      break;
                case STUFF:    if(len>=0) msg[len++] = MARK; break;                     
                case EOM:      if(len>=0) return len;        break; //<<--- return here

                default: // shall be normal data
                if(isDataByte(dualByte) && len>=0) msg[len++] = dataByte(dualByte);
            } // switch
        }     // loop
    }
};

} // namespace RODOS

