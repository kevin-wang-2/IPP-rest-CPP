#include "proxyMessageLoop.h"

template<class Handler>
void proxyMsgLoop<Handler>::operator()() {
    while (!parent.terminate) {
        while(!parent.msgQueue.empty()) {
            while(!parent.baseMutex.try_lock());
            auto curMsg = parent.msgQueue.front();
            parent.msgQueue.pop();
            if(!validateProxy(curMsg.msg)) {
            } else try {
                IP fromIP{{(*curMsg.sckClient)->clientIP[0], (*curMsg.sckClient)->clientIP[1], (*curMsg.sckClient)->clientIP[2], (*curMsg.sckClient)->clientIP[3]}, (*curMsg.sckClient)->clientPort};
                **(curMsg.sckClient) << parseProxy(curMsg.msg, fromIP);
            } catch(SocketObject::TimeOut &e) {}
            (*curMsg.sckClient)->close();
            parent.arrClient.erase(curMsg.sckClient);
            delete *curMsg.sckClient;
            parent.terminate = proxyHandler.terminateFlag;
            parent.baseMutex.unlock();
        }
    }
}

template<class Handler>
bool proxyMsgLoop<Handler>::validateProxy(const std::string& input) {
    return proxyHandler.validate(input);
}

template<class Handler>
std::string proxyMsgLoop<Handler>::parseProxy(std::string input, IP ip) {
    return proxyHandler.parse(input, ip);
}