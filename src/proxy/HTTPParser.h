#ifndef CPP_REST_HTTPPARSER_H
#define CPP_REST_HTTPPARSER_H

#include <map>
#include "../network/SocketServer.h"
#include "HTTPException.h"

///// HTTP头结构 /////

// TODO: 完善其它方法
enum HTTPMethod_t {
    GET,
    POST,
    PUT,
    PATCH,
    DELETE,
    HEAD,
    OPTIONS,
    TRACE
};

enum HTTPVersion_t {
    HTTP_1,
    HTTP_1_1,
    HTTP_2
};

enum MIMEMainType_t {
    MIME_TEXT,
    MIME_MULTIPART,
    MIME_APPLICATION,
    MIME_MESSAGE,
    MIME_IMAGE,
    MIME_AUDIO,
    MIME_VIDEO,
    MIME_DRAWING,
    MIME_JAVA,
    MIME_MODEL,
    MIME_ANY
};

struct MIME_t {
    MIMEMainType_t mainType;
    std::string subType;
};

struct weightedMIME_t {
    MIMEMainType_t mainType;
    std::string subType;
    double weight = 1;
};

// TODO: 完善其它头信息
struct HTTPRequestHeader_t {
    HTTPMethod_t method = GET;
    std::string path = "";
    HTTPVersion_t version = HTTP_1_1;
    IP client = {{0, 0, 0, 0}, 80};
    IP host = {{0, 0, 0, 0}, 80};
    std::string referer;
    std::vector<weightedMIME_t> acceptedMIME {};
    bool keepAlive = true;
    std::string UA;
    std::map<std::string, std::string> other;
};

///// HTTP解释器 /////

namespace HTTPParser {
    HTTPRequestHeader_t parseHeader(const std::string& input);

    void parseHead(const std::string& head, HTTPRequestHeader_t& result);

    void parseHost(const std::string& host, HTTPRequestHeader_t& result);

    void parseReferer(const std::string &refString, HTTPRequestHeader_t& result);

    void parseConnection(const std::string& host, HTTPRequestHeader_t& result);

    void parseUA(const std::string& UA, HTTPRequestHeader_t& result);

    void parseMIME(const std::string& MIMES, HTTPRequestHeader_t& result);
}

///// HTTP解释器异常 /////
class InvalidHeader : public std::exception {};

#endif //CPP_REST_HTTPPARSER_H
