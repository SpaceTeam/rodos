#include "hw_tcp.h"

#include "rodos-lwip.h"
#include "lwip/inet.h"

#include <rodos-debug.h>

err_t tcp_recv_func(void* arg, struct tcp_pcb* tpcb, struct pbuf* p, err_t) {
    TCPBase* tcp = static_cast<TCPBase*>(arg);

    if(!p) { // connection closed
        tcp->errorCode = 1;
        return ERR_OK;
    }

    if(tcp->recvBuf != nullptr || p->tot_len > MAX_NETWORK_MESSAGE_LENGTH) {
        return ERR_BUF;
    } else {
        tcp->recvBuf = p;
    }

    return ERR_OK;
}

err_t tcp_accept_func(void* arg, struct tcp_pcb* newpcb, err_t err) {
    HW_TCPServer* tcp = static_cast<HW_TCPServer*>(arg);

    if(tcp->pcb) tcp_abort(tcp->pcb);

    tcp->pcb = newpcb;
    tcp_arg(tcp->pcb, static_cast<TCPBase*>(arg));
    tcp_recv(tcp->pcb, &tcp_recv_func);
    tcp_accepted(tcp->listenPcb);

    return ERR_OK;
}

err_t tcp_connected_func(void* arg, struct tcp_pcb* tpcb, err_t err) {
    TCPBase* tcp = static_cast<TCPBase*>(arg);

    if(err == ERR_OK) tcp->errorCode = 0;

    return ERR_OK;
}


namespace RODOS {

//_______________________________________________________________________________

bool HW_TCPServer::listen(const long portNr) {
    if(!IPStack::instance) {
        RODOS_ERROR("IPStack not instantiated");
        return false;
    }
    IPStack::instance->ensureInit();

    long port = portNr;

    listenPcb = tcp_new();

    if(port < 0) {
        port = -port;
        listenPcb->so_options |= SOF_REUSEADDR;
    }

    tcp_arg(listenPcb, this);
    tcp_bind(listenPcb, IP_ADDR_ANY, portNr);
    listenPcb = tcp_listen(listenPcb);
    tcp_accept(listenPcb, &tcp_accept_func);

    errorCode = 0;
    return true;
}

HW_TCPServer::~HW_TCPServer() {
    tcp_abort(listenPcb);
}

bool HW_TCPServer::acceptNewConnection() {
    return pcb;
}

int HW_TCPServer::sendData(void* buf, int len) {
    return TCPBase::sendData(buf, len);
}

int HW_TCPServer::getData(void* buf, int maxLen) {
    return TCPBase::getData(buf, maxLen);
}

int HW_TCPServer::getErrorCode() const {
    return errorCode;
}

//_______________________________________________________________________________

HW_TCPClient::~HW_TCPClient() { tcp_abort(pcb); }

bool HW_TCPClient::reopen(const long portNr, const char* hostname) {
    if(!IPStack::instance) {
        RODOS_ERROR("IPStack not instantiated");
        return false;
    }
    IPStack::instance->ensureInit();

    long port = portNr;

    if(pcb) {
        tcp_abort(pcb);
        pcb = 0;
    }

    struct ip_addr ip;
    inet_aton(hostname, &ip);

    pcb = tcp_new();
    if(port < 0) {
        port = -port;
        pcb->so_options |= SOF_REUSEADDR;
    }
    tcp_bind(pcb, IP_ADDR_ANY, 8888);
    tcp_arg(pcb, dynamic_cast<TCPBase*>(this));
    tcp_recv(pcb, &tcp_recv_func);
    tcp_connect(pcb, &ip, port, &tcp_connected_func);

    return true;
}

int HW_TCPClient::sendData(void* buf, int len) {
    return TCPBase::sendData(buf, len);
}

int HW_TCPClient::getData(void* buf, int maxLen) {
    return TCPBase::getData(buf, maxLen);
}

int TCPBase::sendData(void* buf, int len) {
    int sendLen = MIN(tcp_sndbuf(pcb), len);
    tcp_write(pcb, buf, sendLen, 0);
    tcp_output(pcb);
    return sendLen;
}

int TCPBase::getData(void* buf, int maxLen) {
    if(!recvBuf) return 0;

    int copyLen = MIN(recvBuf->tot_len - readPos, maxLen);
    pbuf_copy_partial(recvBuf, buf, copyLen, readPos);

    if(recvBuf->tot_len - readPos <= copyLen) {
        pbuf_free(recvBuf);
        recvBuf = nullptr;
        readPos = 0;
    }

    tcp_recved(pcb, copyLen);

    return copyLen;
}

int HW_TCPClient::getErrorCode() const {
    return errorCode;
}

} /* namespace RODOS */
