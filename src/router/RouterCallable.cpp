#include "RouterCallable.h"

void RouterCallable::operator()(IHTTPRequest &req, IHTTPResponse &res) {
    run(req, res, [&]{
        isFinished = true;
    });
}
