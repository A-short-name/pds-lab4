//
// Created by Mattia Riola on 22/05/21.
//

#ifndef PDS_LAB4_REDUCERINPUT_H
#define PDS_LAB4_REDUCERINPUT_H

#include "Result.hpp"
#include <string>

template<typename T>
class ReducerInput {
    Result<T> mapResult;
    T accum;
public:
    ReducerInput(Result<T> mapResult, T acc): mapResult(mapResult), accum(acc){}


    const Result<T> &getMapResult() const {
        return mapResult;
    }


    T getAccum() const {
        return accum;
    }

};


#endif //PDS_LAB4_REDUCERINPUT_H
