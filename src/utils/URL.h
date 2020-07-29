#ifndef CPP_REST_URL_H
#define CPP_REST_URL_H

#include <string>
#include <cassert>

unsigned char ToHex(unsigned char x);

unsigned char FromHex(unsigned char x);

std::string UrlEncode(const std::string& str);

std::string UrlDecode(const std::string& str);

#endif //CPP_REST_URL_H
