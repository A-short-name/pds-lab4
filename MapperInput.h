//
// Created by Mattia Riola on 14/05/21.
//

#ifndef PDS_LAB4_MAPPERINPUT_H
#define PDS_LAB4_MAPPERINPUT_H


#include <string>

class MapperInput{
    std::string line;
public:
    MapperInput(std::string line):line(std::move(line)){}
    std::string getLine() const {return line;}
};


#endif //PDS_LAB4_MAPPERINPUT_H
