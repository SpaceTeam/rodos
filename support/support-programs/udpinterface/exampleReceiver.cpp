#include <stdio.h>
#include "udp_common.h"

/*************************************************/

UDPInLx udpIn(7022);


int main([[gnu::unused]] int argc, [[gnu::unused]] char *argv[]) {
    char inString[100];
    while(1) {
        udpIn.get(inString);
        printf("got %s\n", inString);
    }
    return 0;
}
