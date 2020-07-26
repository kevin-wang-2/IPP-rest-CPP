#ifndef CPPWEBLOGGER_PROXYMESSAGELOOP_H
#define CPPWEBLOGGER_PROXYMESSAGELOOP_H

#include "../network/SocketServer.h"
#include <iostream>

class ProxyTemplate {
public:
    bool terminateFlag = false;

    virtual bool validate(const std::string &) { return true; };

    virtual std::string parse(std::string input, IP ip) {
        std::cout << ip.IP[0] << "."
                  << ip.IP[1] << "."
                  << ip.IP[2] << "."
                  << ip.IP[3] << ":"
                  << ip.port << " - "
                  << input
                  << std::endl;
        return input;
    }
};

template<class Handler = ProxyTemplate>
class proxyMsgLoop : public messageLoop {
    SocketServer<proxyMsgLoop> &parent;
    Handler proxyHandler;
public:
    explicit proxyMsgLoop(SocketServer<proxyMsgLoop> &ss) : parent(ss) {}

    void operator()() override;

    bool validateProxy(const std::string &input);

    std::string parseProxy(std::string input, IP ip);
};

#include "proxyMessageLoop.hpp"

#endif //CPPWEBLOGGER_PROXYMESSAGELOOP_H
