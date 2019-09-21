
/**
* @file hw_udp.h
* @date 2008/07/15 16:35
* @author Sergio Montenegro
*
*/

#include "rodos.h"

#pragma once

namespace RODOS {

/**
*  UDP interface for Linux platform, using sockets
*/


/**
 *    UDP input port for linux
 */
class UDPReceiver {

private:

public:
    UDPReceiver(const int port) { }
    ~UDPReceiver()              { }

    void reopen(const int port) { }
 
    void setAsync(Topic<GenericMsgRef>* associatedTopic) { }

    long get(void* userData, const unsigned int maxLen=1300)    { return 0; }
    long get(void* userData, int maxLen, unsigned long *ipaddr) { return 0; }
    bool readyToGet()                                           { return false;}
    bool isInitialised()                                        { return true; } 
};


class UDPTransmitter {
private:

public:

    /** init udp communication, can be re-called at any time
     ** WARNING: Negative port number means broadcast **/

    UDPTransmitter(const int port,  const char *host = "localhost")         { }
    UDPTransmitter(const long _portNr, unsigned long _ipAddr)               { }
    UDPTransmitter(const long _portNr, int ip0, int ip1, int ip2, int ip3)  { }
    virtual ~UDPTransmitter()                                               { }

    void openConnection(const int port,  const char *host)                  { }

    /** Send it as datagram containing "userdata", default length = all bytes
     * @param data pointer to datafield
     * @param length of data field
     */
    virtual bool send(const void*msg, const unsigned int len)               { return false; }

    /** Send datagram to a specified host different to the address used during initialisation
     * @param data pointer to datafield
     * @param length of data field
     * @param ipAddr of receiving host
     */
    bool sendTo(const void* userData, const int maxLen, unsigned long ipAddr) { return false; }
    bool isInitialised()                                                      { return true; } 
};

} // namespace

