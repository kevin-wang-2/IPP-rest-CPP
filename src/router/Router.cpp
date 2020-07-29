#include "Router.h"
#include <iostream>

#include "../../lib/CPPLogger/CPPLogger.h"
static CPPLogger& logger = CPPLogger::getLogger("server");

using namespace std;

Router& Router::getRouter() {
    static unique_ptr<Router> router = nullptr;
    if(!router) router = make_unique<Router>(Router());

    return *router;
}

void Router::operator()(const HTTPRequest_t &req, HTTPResponse_t &res) {
    logger.enter("ROUTER");
    IHTTPRequest ireq(req);
    IHTTPResponse ires(res);

    int cntCallable = 0;

    for(auto &callable : dqCallable) {
        cntCallable++;
        logger.enter("CALLABLE" + to_string(cntCallable) + callable->getIdentity());
        try {
            callable->operator()(ireq, ires);
            if (callable->finish())  {
                callable->reset();
                logger.exit();
                logger.exit();
                return;
            } else callable->reset();
        } catch(exception& e) {
            logger(ERROR)
                << e.what()
                << endl;
            res.header.status = 500;
            logger.exit();
            logger.exit();
            return;
        }
        logger.exit();
    }

    if(!ires.finished) {
        res.header.status = 404;
    }

    logger.exit();
}