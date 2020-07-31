#include "router/Router.h"
#include "router/HTMLRouter.h"
#include "../lib/CPPLogger/CPPLogger.h"
#include "middleware/StaticResource.h"

static CPPLogger& logger = CPPLogger::getLogger("server");

int main() {
    logger.enter("MAIN");
    HTTPServer server;

    Router& router = Router::getRouter();

    router
    .use(FunctionalRouterCallable([](IHTTPRequest &req, IHTTPResponse &res) {
        logger(LOG) << "Client "
                  << to_string(req.request.header.client.IP[0])
                  << "."
                  << to_string(req.request.header.client.IP[1])
                  << "."
                  << to_string(req.request.header.client.IP[2])
                  << "."
                  << to_string(req.request.header.client.IP[3])
                  << ":"
                  << to_string(req.request.header.client.port)
                  << " is visiting "
                  << req.request.header.path
                  << endl;
        return false;
    }))
    .use(StaticResource("static", ""))
    .use(HTMLRouter<GET>("/test", [](IHTTPRequest &req, IHTTPResponse &res) {
        if(!req.params.empty())
            res.write("Param:")
            .write(req.params[0])
            .write("<br>");
        res.end("Capture OK");
        //res.end(); // Cause Error
        return true;
    }));

    server.listen(8000);

    server.serve_forever();

    logger.exit();
    return 0;
}