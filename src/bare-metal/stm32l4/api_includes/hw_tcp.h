#pragma once

// DUMMY!

namespace RODOS {

class HW_TCPServer {
public:
    ~HW_TCPServer();

    bool listen(const long portNr) { return false; };
    bool acceptNewConnection() { return false; };
    int sendData(void* buf, int len) { return 0; };
    int getData(void* buf, int maxLen) { return 0; };
    int getErrorCode() const { return -1; };
};

class HW_TCPClient {
public:
    ~HW_TCPClient();

    bool reopen(const long portNr, const char* hostname = "localhost") { return false; };
    int sendData(void* buf, int len) { return 0; };
    int getData(void* buf, int maxLen) { return 0; };
    int getErrorCode() const { return -1; };
};

} // namespace
