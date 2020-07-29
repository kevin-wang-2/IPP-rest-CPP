#ifndef CPP_REST_STL_HELPER_H
#define CPP_REST_STL_HELPER_H

#include <string>
#include <map>
#include <utility>

template <class T1, class T2>
class map_value_finder {
    T2 _cmp;
public:
    explicit map_value_finder(const T2&  _) : _cmp(_) {}
    bool operator()(const typename std::map<T1, T2>::value_type& pair) {
        return pair.second == _cmp;
    }
};

#endif //CPP_REST_STL_HELPER_H
