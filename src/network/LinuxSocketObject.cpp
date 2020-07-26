#include "../utils/platforms.h"
#ifdef LINUX

#include "SocketObject.h"
#include <cstring>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

SocketObject::SocketObject(netType_t net, sockType_t sock, int protocol = 0) {
    if(net == 0) {
        domain = AF_INET;
    } else {
        domain = AF_INET6;
    }
    if(sock == 0) {
        type = ::SOCK_STREAM;
    } else {
        type = ::SOCK_DGRAM;
    }

    socketHandle = socket(domain, type, protocol);
    timeval tm = {2, 0};
    ::setsockopt(socketHandle, SOL_SOCKET, SO_RCVTIMEO, &tm, sizeof(timeval));
    ::setsockopt(socketHandle, SOL_SOCKET, SO_SNDTIMEO, &tm, sizeof(timeval));
}

SocketObject::SocketObject(const SocketObject &server, int clientHandle, const void* _addr) {
    domain = server.domain;
    type = server.type;

    const sockaddr_in *addr = (sockaddr_in *)_addr;
    clientIP[0] = (addr->sin_addr.s_addr & 0xff);
    clientIP[1] = (addr->sin_addr.s_addr >> 8) & 0xff;
    clientIP[2] = (addr->sin_addr.s_addr >> 16) & 0xff;
    clientIP[3] = (addr->sin_addr.s_addr >> 24) & 0xff;

    clientPort = ntohs(addr->sin_port);
    domain = addr->sin_family;
    type = server.type;

    socketHandle = clientHandle;
    connected = true;
}

void SocketObject::bind(int port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(0);
    if(::bind(socketHandle, (sockaddr*)&addr, sizeof(addr)) == -1) throw FailToBind();
}

void SocketObject::listen(int queue) {
    if(::listen(socketHandle, queue)) throw FailToListen();
}

void SocketObject::connect(const int *IP, int port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = in_addr_t((unsigned long)(IP[0]) || (unsigned long)(IP[1]) << 8 || (unsigned long)(IP[2]) << 16 || (unsigned long)(IP[3]) << 32);
    if(::connect(socketHandle, (sockaddr*)&addr, sizeof(addr)) == -1) throw FailToConnect();\
    connected = true;
}

SocketObject SocketObject::accept(bool async) {
    sockaddr_in addr{};
    socklen_t length = sizeof(addr);

    int clientHandle;

    if((clientHandle = ::accept(socketHandle, (sockaddr*)&addr, &length)) == -1) {
        if(async) throw FailToAccept();
    } else {
        return  {*this, clientHandle, &addr};
    }

    while((clientHandle = ::accept(socketHandle, (sockaddr*)&addr, &length)) == -1);

    return  {*this, clientHandle, &addr};
}

SocketObject& SocketObject::operator<<(const std::string &str) {
    if(!connected) throw NotConnected();
    ::send(socketHandle, str.c_str(), str.length(), 0);

    return *this;
}

SocketObject& SocketObject::operator<<(const char *buffer) {
    if(!connected) throw NotConnected();
    ::send(socketHandle, buffer, strlen(buffer), 0);

    return *this;
}

SocketObject& SocketObject::operator<<(const SocketBuffer &buffer) {
    if(!connected) throw NotConnected();
    ::send(socketHandle, buffer.content, buffer.len, 0);

    return *this;
}

SocketObject& SocketObject::operator>>(std::string &target) {
    if(!connected) throw NotConnected();
    char buffer[1024]{};

    if(::recv(socketHandle, buffer, 1024, 0) == -1 && errno == EAGAIN) throw TimeOut();

    target = buffer;

    return *this;
}

SocketObject& SocketObject::operator>>(char *buffer) {
    if(!connected) throw NotConnected();

    if(::recv(socketHandle, buffer, 1024, 0) == -1 && errno == EAGAIN) throw TimeOut();

    return *this;
}

SocketObject& SocketObject::operator>>(SocketBuffer &buffer) {
    if(!connected) throw NotConnected();

    if(::recv(socketHandle, buffer.content, buffer.len, 0) == -1 && errno == EAGAIN) throw TimeOut();

    return *this;
}

void SocketObject::close() {
    ::close(socketHandle);
    closed = true;
}

#endif