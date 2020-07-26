template<class MsgLoop>
SocketServer<MsgLoop>::SocketServer() : sckServer(I_AF_INET, I_SOCK_STREAM, 0) {
}

template<class MsgLoop>
SocketServer<MsgLoop>::~SocketServer() {
    sckServer.close();
    if(thMsgLoop) {
        terminate = true;
        if(thMsgLoop->joinable()) thMsgLoop->join();
        delete thMsgLoop;
    }
    if(thAccLoop) {
        terminate = true;
        if(thAccLoop->joinable()) thAccLoop->join();
        delete thAccLoop;
    }
    if(!arrClient.empty()) {
        for(auto &item : arrClient) item->close();
    }
}

template<class MsgLoop>
void SocketServer<MsgLoop>::listen(int port) {
    sckServer.bind(port);
    sckServer.listen();
}

#include <iostream>

template <class MsgLoop>
void SocketServer<MsgLoop>::serve_forever(bool async) {
    std::cout << "Serving ..." << std::endl;
    if(async) {
        thMsgLoop = new ::std::thread(MsgLoop(*this));
        thAccLoop = new ::std::thread(accLoop(*this));
    } else {
        thAccLoop = new ::std::thread(accLoop(*this));
        MsgLoop(*this)();
    }
}

template<class MsgLoop>
void SocketServer<MsgLoop>::accLoop::operator()() {
    std::vector<std::thread*> thPool;
    while(!parent.terminate) {
        try {
            SocketObject client = parent.sckServer.accept(true);
            while(!parent.baseMutex.try_lock());
            parent.arrClient.push_back(new SocketObject(client));
            auto id = parent.arrClient.end() - 1;
            try {
                std::string msg;
                client >> msg;
                parent.msgQueue.push({id, msg});
            } catch (SocketObject::TimeOut &e) {
                (**id).close();
                delete *id;
                parent.arrClient.erase(id);
            }
            parent.baseMutex.unlock();
        } catch(SocketObject::FailToAccept &e) {}
    }
}
