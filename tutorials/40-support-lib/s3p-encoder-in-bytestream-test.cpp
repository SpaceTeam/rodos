/*
 * S3P Encoder / decoder test
 * @Author: Lars Grütter, Sergio Montenegro
 */ 

#include "rodos.h"
#include "s3p-encoder.h"

static int const MAX_LEN = 100;

uint8_t intermediateBuffer[MAX_LEN]; // to store received message to send again
int16_t intermediateBufferCnt = 0;

//__________________________________________________________________________________
// this shall write to a UART or Radio-Link Modulator or so.
class S3pSender : public S3pEncoder { // 
    void putByte(uint8_t c) { 
        intermediateBuffer[intermediateBufferCnt++] = c; 
        PRINTF("   Write in UART[%d] 0x%02x\n", intermediateBufferCnt, c);
    }
} s3pSender;


//__________________________________________________________________________________
// this allows you to receive data from UART, Radio-Link or so reacting to interrupts
class S3pReceiverFromInterrupt : public S3pEncoder {

    // This both shall be used in when using interrupts to get data
    void appendByte(uint8_t c) { PRINTF(" 0x%02x-", c); }
    void endOfMessage(int len) { PRINTF(": total %d bytes\n", len); }

} s3pReceiverFromInterrupt;


//__________________________________________________________________________________
// this allows you to receive data from UART, Radio-Link or so by polling
class S3pReceiverPolling : public S3pEncoder {
    int pos = -1;

    // this shall be a polling read from UART or Radio-Link or so.
    uint8_t getByte() { 
        if(pos >= MAX_LEN-1) pos = 0;
        return intermediateBuffer[pos++]; 
    }
} s3pReceiverPolling;







//__________________________________________________________________________________
//__________________________________________________________________________________
//__________________________________________________________________________________
// Test all sender and both receivers


class S3P_Test : public StaticThread<> {
    uint8_t dataToSend[MAX_LEN];
    uint8_t dataToReceive[MAX_LEN];

    void run() {

        // Simulate a Sender (to UART or so)
        PRINTF("_________________________________ Simulate Sender\n");
        for(uint8_t i = 0; i < 20; i++) dataToSend[i] = (0xf0u + i) & 0xFFu;

        PRINTF("Sending trash before BOM\n"); 
        for(uint8_t i = 0; i < 10; i++) s3pSender.putByteEncoded(i);

        PRINTF("Sending data: ");
        for(int i = 0; i < 20; i++) PRINTF("0x%02x-", dataToSend[i]);
        PRINTF("\n");
        s3pSender.sendMsg(20, dataToSend);

        PRINTF("Sending trash after EOM\n"); 
        for(uint8_t i = 0; i < 5; i++) s3pSender.putByteEncoded(i);
        PRINTF("send again\n");
        s3pSender.sendMsg(20, dataToSend);

        
        //Simulate a Receiver using Interrupts (from UART or so)
        PRINTF("_________________________________ Simulate Receiver from Interrupt (shall get 2 identical messages)\n");
        for(int i = 0; i < intermediateBufferCnt; i++) s3pReceiverFromInterrupt.upcallOnInputChar(intermediateBuffer[i]);  
        
        //Simulate a Receiver by polling (from UART or so)
        PRINTF("_________________________________ Simulate Receiver by polling (shall get many identical messages)\n");
        int len;
        for(int j = 0; j < 6; j++) { 
            len = s3pReceiverPolling.getMsgByPolling(MAX_LEN, dataToReceive);
            PRINTF("Got %d bytes: ", len);
            for(int i = 0; i < len; i++) PRINTF("0x%02x-", dataToReceive[i]);
            PRINTF("\n");
        }

        PRINTF("\n\n DONE, ^C to end\n");

    }
} s3pTest; 
