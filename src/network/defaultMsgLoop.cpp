#include "SocketServer.h"

void defaultMsgLoop::operator()() {
    while (!parent.terminate) {
        while(!parent.msgQueue.empty()) {
            auto curMsg = parent.msgQueue.front();
            parent.msgQueue.pop();
            try {
                **(curMsg.sckClient) << curMsg.msg;
                (*curMsg.sckClient)->close();
            } catch(SocketObject::TimeOut &e) {
                (*curMsg.sckClient)->close();
            }
            parent.arrClient.erase(curMsg.sckClient);
            delete *curMsg.sckClient;
        }
    }
}