#ifndef CPPWEBLOGGER_MESSAGELOOP_H
#define CPPWEBLOGGER_MESSAGELOOP_H

#include "SocketObject.h"

class defaultMsgLoop;

template<class T>
class SocketServer;

class messageLoop {
public:
    virtual void operator()() = 0;
};

class defaultMsgLoop : messageLoop {
    SocketServer<defaultMsgLoop> &parent;
public:
    explicit defaultMsgLoop(SocketServer<defaultMsgLoop> &ss) : parent(ss) {}

    void operator()() override;
};

#endif //CPPWEBLOGGER_MESSAGELOOP_H
