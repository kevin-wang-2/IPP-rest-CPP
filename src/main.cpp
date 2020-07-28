#include "router/Router.h"

int main() {
    HTTPServer server;

    Router& router = Router::getRouter();

    router.use(std::make_shared<FunctionalRouterCallable>(FunctionalRouterCallable([](IHTTPRequest &req, IHTTPResponse &res) {
        std::cout << "Client "
                  << req.request.header.client.IP[0]
                  << "."
                  << req.request.header.client.IP[1]
                  << "."
                  << req.request.header.client.IP[2]
                  << "."
                  << req.request.header.client.IP[3]
                  << ":"
                  << req.request.header.client.port
                  << " is visiting "
                  << req.request.header.path
                  << std::endl;
        if(req.request.header.path == "/1234") return false;
        res.end(req.request.header.path);
        return true;
    })));

    server.listen(8000);

    server.serve_forever();

    return 0;
}