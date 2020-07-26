#include <iostream>
#include "network/SocketServer.h"
#include "proxy/HTTPProxyHandler.h"

int main() {
    SocketServer<proxyMsgLoop<HTTPProxyHandler>> server;
    server.listen(8000);

    server.serve_forever();

    return 0;
}