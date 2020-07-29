#ifndef CPP_REST_IHTTPRESPONSE_H
#define CPP_REST_IHTTPRESPONSE_H


#include "../proxy/HTTPProxyHandler.h"

class RequestEnd : public std::exception {
    const char* what() const noexcept override{
        return "Request has been ended";
    }
};

class Router;

class IHTTPResponse {
    HTTPResponse_t &response;
    bool finished = false;

    void validate() { if (finished) throw RequestEnd(); }

    explicit IHTTPResponse(HTTPResponse_t &res) : response(res), const_response(res) {}

public:
    const HTTPResponse_t &const_response;

    void status(int status) { response.header.status = status; }

    IHTTPResponse& write(const std::string &s) {
        validate();
        response.body += s;
        return *this;
    }

    void end(const std::string &s = "") {
        validate();
        response.body += s;
        finished = true;
    }

    void render(const std::string &fn);

    friend Router;
};


#endif //CPP_REST_IHTTPRESPONSE_H
