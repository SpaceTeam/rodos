#include "rodos.h"
#include "checksumes.h"

uint64_t toBeTested;

class ChecksumTester : public Thread {

  public:
    void run() {
        toBeTested = 1;
        char buff[80]; //Buffer for printf, a lot longer than needed

        CRC crc;

        for(int i = 1; i < 100; i++) {
            toBeTested *= i;
            toBeTested += i;
            SPRINTF(buff, " demo data %lld", toBeTested);
            PRINTF("Checksum of %d: %lld\n", i, toBeTested);

            int32_t computedCRC1 = RODOS::computeCrc((uint8_t*)&toBeTested, 8, 0);
            int32_t computedCRC2 = crc.computeCRC((uint8_t*)&toBeTested, 8, 0);
            if(computedCRC1 == computedCRC2)
                PRINTF("  OK %d\n", computedCRC1);
            else
                PRINTF(" Err %d != %d\n", computedCRC1, computedCRC2);
            PRINTF("  checksum   %d\n", RODOS::checkSum((uint8_t*)&toBeTested, 8));
            PRINTF("  chcksumxor %d\n", RODOS::checkSumXor32((uint32_t*)&toBeTested, 2));
            PRINTF("  hash; %d\n", hash(buff));
        }

        PRINTF("\nThis run (test) terminates now! (%s)\n", LOCATION);
        hwResetAndReboot();
    }
} checksumsTester;