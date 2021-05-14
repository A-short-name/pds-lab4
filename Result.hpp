//
// Created by Mattia Riola on 14/05/21.
//

#ifndef PDS_LAB4_RESULT_HPP
#define PDS_LAB4_RESULT_HPP

#include <string>

template <typename T>
class Result {
    std::string key;
    T value;
public:
    const std::string &getKey() const {
        return key;
    }

    void setKey(const std::string &key) {
        Result::key = key;
    }

    T getValue() const {
        return value;
    }

    void setValue(T value) {
        Result::value = value;
    }
    Result(std::string s, T value): key(std::move(s)), value(std::move(value)) {};
};

#endif //PDS_LAB4_RESULT_HPP
