#ifndef CPPWEBLOGGER_SOCKETSERVER_H
#define CPPWEBLOGGER_SOCKETSERVER_H

#include "SocketObject.h"
#include "messageLoop.h"
#include <vector>
#include <queue>
#include <string>
#include <thread>
#include <mutex>
#include <memory>

#include <iostream>

struct ClientMessage {
    std::vector<SocketObject *>::const_iterator sckClient;
    std::string msg;
};

template<class MsgLoop>
class SocketServer {
protected:
    SocketObject sckServer;
    std::vector<SocketObject *> arrClient{};
    std::queue<ClientMessage> msgQueue{};
    std::mutex baseMutex{};
    std::thread *thMsgLoop = nullptr,
            *thAccLoop = nullptr;

    bool terminate = false;

    int _port;

    class accLoop {
        SocketServer<MsgLoop> &parent;
    public:
        explicit accLoop(SocketServer<MsgLoop> &ss) : parent(ss) {}

        void operator()();
    };

    friend accLoop;
    friend MsgLoop;
public:
    SocketServer();

    ~SocketServer();

    void listen(int port);

    void serve_forever(bool async = false);
};


#include "SocketServer.hpp"


#endif //CPPWEBLOGGER_SOCKETSERVER_H
