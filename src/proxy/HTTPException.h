#ifndef CPP_REST_HTTPEXCEPTION_H
#define CPP_REST_HTTPEXCEPTION_H

class HTTPException : public std::exception {
public:
    int code;
    explicit HTTPException(int _code) : code(_code) {};
};

#endif //CPP_REST_HTTPEXCEPTION_H
