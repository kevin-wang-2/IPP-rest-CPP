#ifndef CPP_REST_HTTPPARSER_H
#define CPP_REST_HTTPPARSER_H

#include "../network/SocketServer.h"

///// HTTP头结构 /////

// TODO: 完善其它方法
enum HTTPMethod_t {
    GET,
    POST,
    PUT,
    PATCH,
    DELETE,
    OTHER
};

enum HTTPVersion_t {
    HTTP_1_1,
    HTTP_2
};

// TODO: 完善其它头信息
struct HTTPHeader_t {
    HTTPMethod_t method;
    std::string path;
    HTTPVersion_t version;
    IP host;
    bool keepAlive;
    std::vector<std::string> UA;
    std::string body;
};

///// HTTP解释器 /////

namespace HTTPParser {
    HTTPHeader_t parseHeader(const std::string& input);

    void parseHead(const std::string& head, HTTPHeader_t& result);
}

///// HTTP解释器异常 /////
class InvalidHeader : public std::exception {};

#endif //CPP_REST_HTTPPARSER_H
