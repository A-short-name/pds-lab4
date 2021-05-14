#include <iostream>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <string>
#include "Result.hpp"

#define NUM_DELIMITERS 5
#define LOG_FILE "../log.txt"

template<class MapperInputT, class ResultT>
std::vector<ResultT> mapreduce_map(const MapperInputT& input);


template<typename ReducerInput, typename ResultT>
ResultT mapreduce_reduce(ReducerInput input);

template<class ReduceInputT, class ResultT>
ResultT mapreduce(const ReduceInputT &input);



int main() {
    //string that will contain lines of file
    std::string line;

    //open file
    auto myfile = std::ifstream(LOG_FILE);

    if (myfile.is_open()) {
        while ( getline (myfile,line) )
        {
            //chiamando la map devo risolvere i tipi template, che saranno "string" e "result" (che a sua volta è una classe template)
            auto risultato = mapreduce_map<std::string, Result<int>>(line);

            //stampa risultato map
            for(auto & p : risultato) {
                std::cout << "(" << p.getKey() << " " << p.getValue() << ") ";
            }
            std::cout << "\n";
        }
        myfile.close();
    }



    myfile.close();
    return 0;
}


/**
 *
 * @tparam MapperInputT, che nel nostro caso sarà std::string, deve implementare la funzione substr
 * @tparam ResultT nel nostro caso sarà Result<int>
 * @param input line letta da file
 *
 * @return vettore di ResultT (avrò tutti i campi ex: ip, data, error code ecc)
 */
template<typename MapperInputT, typename ResultT>
std::vector<ResultT>  mapreduce_map(const MapperInputT& input){
    std::vector<ResultT> res;
    MapperInputT chunk;
    MapperInputT next = input;
    std::vector<MapperInputT> fields;
    // delimitatori delle stringhe di log che ho nell'esempio
    std::string delimiters[NUM_DELIMITERS] = {
            " - - [",
            "] \"",
            "\" ",
            " ",
            "\n"
    };

    //per ogni delimitatore itero nella stringa e recupero ogni chunk
    for (auto &delimiter : delimiters) {
        //
        chunk = next.substr(0, next.find(delimiter));
        fields.push_back(chunk);
        next = next.substr(next.find(delimiter) + delimiter.length(),
                           next.length() - (next.find(delimiter) + delimiter.length()));

        res.push_back({ResultT(chunk,1)});
    }
    return res;
}

