
/*
 * S3P Message framing, 
 * @author: Sergio Montenegro, Sept 2019
 */

#include "s3p-encoder.h"

namespace RODOS {

//_____________________________________ To decode messages (Receiver from UART, Radio-Link or so)

int S3pEncoder::getMsgByPolling(int maxLen, uint8_t* msg) {

    static uint16_t dualByte =  0; // it will deliver an unexpected 0x00XX but only for the first call
    int             len      = -1;

    while(1) {
        if(len >= maxLen) return maxLen;
        dualByte = static_cast<uint16_t>((dualByte << 8) | getByte());

        switch(dualByte) {
            case BOM:
                len = 0;
                break;

            case EOM:
                if(len >= 0) return len; //<<--------------------------------------
                break;

            case STOP:
                mayPutBytes = false;
                break;

            case CONTINUE:
                mayPutBytes = true;
                break;

            case SYNC:
                syncTime();
                break;

            case STUFF:
                if(len >= 0) msg[len++] = MARK;
                break;

            default:
                if(len >= 0 && !isCommand(dualByte) && !isMark(dualByte)) {
                    msg[len++] = static_cast<uint8_t>(dualByte & 0xff);
                }
        }
    }
}


//_______________________________________ to decode messages as reaction to interrupt from UART, Radio link or so
void S3pEncoder::upcallOnInputChar(uint8_t inputByte) {

    static int16_t  len      = -1;
    static uint16_t dualByte = 0;

    dualByte = static_cast<uint16_t>((dualByte << 8u) | inputByte);

    switch(dualByte) {
        case BOM:
            len = 0;
            break;

        case EOM:
            if(len >= 0) endOfMessage(len);
            len = -1;
            break;

        case STOP:
            mayPutBytes = false;
            break;

        case CONTINUE:
            mayPutBytes = true;
            break;

        case SYNC:
            syncTime();
            break;

        case STUFF:
            if(len >= 0) { appendByte(MARK); len++; }
            break;

        default:
            if(len >= 0 && !isCommand(dualByte) && inputByte != MARK) {
                appendByte(inputByte);
                len++;
            }
    }
}

}  // namespace RODOS

