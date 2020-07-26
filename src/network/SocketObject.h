#ifndef CPPWEBLOGGER_SOCKETOBJECT_H
#define CPPWEBLOGGER_SOCKETOBJECT_H

#include <string>

enum netType_t {
    I_AF_INET,
    I_AF_INET6
};

enum sockType_t {
    I_SOCK_STREAM,
    I_SOCK_DGRAM
};

struct SocketBuffer {
    int len;
    char *content;
};

struct IP {
    unsigned int IP[4];
    unsigned int port;
};

/**
 * 封装Socket库
 */
class SocketObject {
protected:
    bool connected = false;

    bool closed = false;

    int domain, type;

    int socketHandle;

    SocketObject(const SocketObject &server, int clientHandle, const void *addr);

public:

    unsigned int clientIP[4]{};

    unsigned int clientPort = 0;

    class NetworkError : public std::exception {
    };

    class FailToBind : public NetworkError {
    };

    class FailToListen : public NetworkError {
    };

    class FailToConnect : public NetworkError {
    };

    class FailToAccept : public NetworkError {
    };

    class NotConnected : public NetworkError {
    };

    class TimeOut : public std::exception {
    };

    SocketObject(netType_t net, sockType_t sock, int protocol);

    SocketObject(const SocketObject &so) {
        connected = so.connected;
        closed = so.closed;
        domain = so.domain;
        type = so.type;
        socketHandle = so.socketHandle;
        clientIP[0] = so.clientIP[0];
        clientIP[1] = so.clientIP[1];
        clientIP[2] = so.clientIP[2];
        clientIP[3] = so.clientIP[3];
        clientPort = so.clientPort;
    };

    void bind(int port);

    void listen(int queue = 20);

    void connect(const int IP[4], int port);

    SocketObject accept(bool async = false);

    SocketObject &operator<<(const std::string &str);

    SocketObject &operator<<(const char *buffer);

    SocketObject &operator<<(const SocketBuffer &buffer);

    SocketObject &operator>>(std::string &target);

    SocketObject &operator>>(char *buffer);

    SocketObject &operator>>(SocketBuffer &buffer);

    void close();

    bool isClosed() { return closed; }
};

#endif //CPPWEBLOGGER_SOCKETOBJECT_H
