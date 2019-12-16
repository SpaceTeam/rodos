//#include <string.h>

#include "rodos.h"
#include "support-libs.h"

#include "hw_udp.h"
#include "rodos-lwip.h"
#include "lwip/igmp.h"

namespace RODOS {

void udp_recv_func(void* arg, struct udp_pcb* pcb, struct pbuf* p,
                   ip_addr_t* addr, u16_t port) {

    UDPReceiver*   udpRecv = (UDPReceiver*)arg;
    GenericMsgRef* msg     = &udpRecv->recvDatRef[udpRecv->recvDatPtr];

    if(p->tot_len <= MAX_NETWORK_MESSAGE_LENGTH) {
        msg->msgLen = p->tot_len;
        pbuf_copy_partial(p, udpRecv->recvDat[udpRecv->recvDatPtr], p->tot_len,
                          0);
        msg->msgPtr = (char*)udpRecv->recvDat[udpRecv->recvDatPtr];

        if(udpRecv->recvDatPtr >= RECV_BUFFER_COUNT)
            udpRecv->recvDatPtr = 0;
        if(udpRecv->asyncTopic)
            udpRecv->asyncTopic->publishFromInterrupt(msg, sizeof(msg));
        ++udpRecv->recvDatPtr;
        if(udpRecv->recvDatPtr >= RECV_BUFFER_COUNT)
            udpRecv->recvDatPtr = 0;
        pbuf_free(p);
    }
}

uint8_t lastPointer;

UDPReceiver::UDPReceiver(const TUDPPortNr port) {
    pcb         = 0;
    asyncTopic  = 0;
    initialised = false;
    recvDatPtr  = 0;
    lastPointer = 0;
    reopen(port);
}

UDPReceiver::~UDPReceiver() {
}

void UDPReceiver::reopen(const TUDPPortNr port) {
    listenPort  = port;
    multiReader = false;
    if(port < 0) {
        listenPort  = -port;
        multiReader = true;
    }
    initialised = false;
    init();
}

bool UDPReceiver::init() {
    if(initialised)
        return true;

    if(IPStack::instance && IPStack::instance->ipstackInitialized) {
        if(pcb) {
            udp_remove(pcb);
            pcb = 0;
        }

        pcb = udp_new();
        if(multiReader) {
            pcb->so_options |= SOF_REUSEADDR;
        }
        udp_bind(pcb, IP_ADDR_ANY, listenPort);
        udp_recv(pcb, &udp_recv_func, this);
        initialised = true;
        return true;
    } else {
        return false;
    }
}

#ifdef MULTICAST
void UDPReceiver::joinMulticastGroup(unsigned long ipaddr) {
    ip_addr_t mAddr;
    mAddr.addr = htonl(ipaddr);
    ip_addr_t sAddr;
    IPAddr    cAddr = IPStack::instance->ip;
    IP4_ADDR(&sAddr, cAddr.a, cAddr.b, cAddr.c, cAddr.d);
    igmp_joingroup(&sAddr, &mAddr);
}
#endif

bool UDPReceiver::readyToGet() {
    return lastPointer != recvDatPtr;
}

void UDPReceiver::setAsync(Topic<GenericMsgRef>* associatedTopic) {
    init();
    asyncTopic = associatedTopic;
}

/**
 * TODO: Not yet supported on bare-metal, use asyncTopic (UDPReceiver::setAsync)
 * Receives data from interface. Writes data up to maxLen to userData.
 * @param[IN/OUT] userData pointer to input buffer
 * @param[IN] size of input buffer
 * @return length of message written to userData
 */
long UDPReceiver::get(void* userData, const unsigned int maxLen) {
    return get(userData, maxLen, 0);
}

/**
 * TODO: Not yet supported on bare-metal, use asyncTopic (UDPReceiver::setAsync)
 * Receives data from interface. Writes data up to maxLen to userData.
 * @param[IN/OUT] userData pointer to input buffer
 * @param[IN] size of input buffer
 * @return length of message written to userData
 */
long UDPReceiver::get(void* userData, int maxLen, unsigned long* ipaddr) {
    if(!init()) {
        return -1;
    }
    if(lastPointer == recvDatPtr)
        return -1;
    if(lastPointer >= RECV_BUFFER_COUNT)
        lastPointer = 0;
    return 0;
}

UDPTransmitter::UDPTransmitter(const TUDPPortNr _port, const char* host) {
    //Name resolution not yet supported on bare-metal
    if(_port < 0) {
        ip_addr_t _ip;
        _ip.addr = IPADDR_LOOPBACK;
        init(_port, _ip);
    } else {
        ip.addr         = 0;
        port            = 0;
        enableBroadCast = false;
    }
}

UDPTransmitter::UDPTransmitter(const long _portNr, unsigned long _ipAddr) {
    ip_addr_t _ip;
    _ip.addr = _ipAddr;
    init(_portNr, _ip);
}

UDPTransmitter::UDPTransmitter(const long _portNr, int ip0, int ip1, int ip2, int ip3) {
    ip_addr_t _ip;
    IP4_ADDR(&_ip, ip0, ip1, ip2, ip3);
    init(_portNr, _ip);
}

UDPTransmitter::~UDPTransmitter() {
}

void UDPTransmitter::openConnection(const TUDPPortNr port, const char* host) {
    // hostName only xx.xx.xx.xx

    // Ip Address
    Tokenizer tok;
    int       ip[4];
    char      hostName[32];

    strcpy(hostName, host);
    if(strcmp(hostName, "localhost") == 0) strcpy(hostName, "127.0.0.1");
    tok.setSeparators((char*)".,; ");
    tok.init(hostName);

    for(int i = 0; i < 4; i++) {
        char* token = tok.next();
        if(token) ip[i] = s2int(token);
    }

    ip_addr_t ipAdr;
    IP4_ADDR(&ipAdr, ip[0], ip[1], ip[2], ip[3]);

    init(port, ipAdr);
}


void UDPTransmitter::init(const TUDPPortNr _port, ip_addr_t ipaddr) {
    if(_port < 0) {
        enableBroadCast = true;
        ip.addr         = IPADDR_BROADCAST;
        port            = -_port;
    } else {
        enableBroadCast = false;
        ip              = ipaddr;
        port            = _port;
    }
    connected = false;
    pcb       = 0;
}

void UDPTransmitter::connect(ip_addr_t _ip, uint16_t prt) {
    if(!pcb) {
        pcb = udp_new();
        pcb->so_options |= SOF_BROADCAST;
    }
    if(connected) {
        udp_disconnect(pcb);
    }
    udp_connect(pcb, &_ip, prt);
    connected = true;
}

bool UDPTransmitter::send(const void* msg, const unsigned int len) {
    if(!IPStack::instance)
        return false;
    IPStack::instance->IPsem.enter();
    if(!connected) {
        connect(ip, port);
    }

    pbuf* pb = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_POOL);
    if(!pb) {
        IPStack::instance->IPsem.leave();
        return false;
    }

    pbuf_take(pb, msg, len);
    err_t result = udp_send(pcb, pb);
    pbuf_free(pb);
    IPStack::instance->IPsem.leave();
    return result == ERR_OK;
}

bool UDPTransmitter::sendTo(const void* userData, const int maxLen,
                            unsigned long _ipAddr) {
    if(!IPStack::instance)
        return false;
    IPStack::instance->IPsem.enter();

    ip_addr_t adr;
    adr.addr = _ipAddr;
    connect(adr, port);

    bool result = send(userData, maxLen);

    udp_disconnect(pcb);
    connected = false;
    IPStack::instance->IPsem.leave();
    return result;
}

} /* namespace RODOS */