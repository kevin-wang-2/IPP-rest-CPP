#include "network/SocketServer.h"
#include "proxy/Router.h"

int main() {
    SocketServer<proxyMsgLoop<HTTPProxyHandler>> server;

    Router& router = Router::getRouter();

    router.use(std::make_shared<FunctionalRouterCallable>(FunctionalRouterCallable([](IHTTPRequest &req, IHTTPResponse &res) {
        res.end(req.request.header.path);
        return true;
    })));

    server.listen(8000);

    server.serve_forever();

    return 0;
}