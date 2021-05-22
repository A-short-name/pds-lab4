#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <regex>
#include "MapperInput.h"
#include "Result.hpp"
#include "ReducerInput.h"
#define LOG_FILE "../log.txt"
/**
 *
 * @tparam MapperInputT, che nel nostro caso sarà std::string, deve implementare la funzione substr
 * @tparam ResultT nel nostro caso sarà Result<int>
 * @param input line letta da file
 *
 * @return vettore di ResultT (avrò tutti i campi ex: ip, data, error code ecc)
 */
template<class MapperInputT, class ResultT>
std::vector<ResultT> mapreduce_map(const MapperInputT& input);

/**
 *
 * @tparam ReducerInputT che è il singolo risultato della map (che produce un vettore di reducerInput)
 * @tparam ResultT coppia chiave valore della mappa (stringa , accumulatore)
 * @param input il singolo risultato della map (che produce un vettore di reducerInput)
 * @return coppia chiave valore della mappa (stringa , accumulatore)
 */
template<typename ReducerInputT, typename ResultT>
ResultT mapreduce_reduce(ReducerInputT input);
/**
 *
 * @tparam T Valore della map (nel nostro caso un intero che è l'accumulatore)
 * @tparam R Reduce function
 * @tparam M Map function
 * @param file_input input dove verrà letta la linea e dove verranno applicate le map e reduce
 * @param mapfun Map function
 * @param redfun Reduce function
 * @return mappa di stringa 
 */
template<typename T,typename R, typename M>
std::map<std::string, T> mapreduce(std::ifstream& file_input,M mapfun, R redfun);




int main() {
    //string that will contain lines of file
    std::string line;

    //open file
    auto myfile = std::ifstream(LOG_FILE);


    if (myfile.is_open()) {
        //mapreduce
        std::map<std::string, int> accs = mapreduce<int>(myfile, mapreduce_map<MapperInput,Result<int>>,mapreduce_reduce<ReducerInput<int>,Result<int>>);

        for ( auto & p : accs){
            std::cout << "(" << p.first << " " << p.second << ") \n";
        }
    }



    myfile.close();
    return 0;
}



template<typename MapperInputT, typename ResultT>
std::vector<ResultT> mapreduce_map(const MapperInputT& input){
    std::vector<ResultT> res;
    std::string line = input.getLine();

    //PRELEVO INDIRIZZO IP
    std::smatch ip_match;
    std::regex_search(line, ip_match, std::regex("^[^ ]+"));
    if (! ip_match.empty())
        res.push_back({ResultT(ip_match[0], 1)});

    //PRELEVO ORA
    std::smatch time_match;
    std::regex_search(line, time_match, std::regex(":[0-9]{2}:"));

    std::string oraString = std::string(time_match[0]).substr(1,2);
    int ora = std::stoi(oraString);

    //PRELEVO FUSO ORARIO E LO SOMMO A ORA
    std::smatch fuso_match;
    std::regex_search(line, fuso_match, std::regex("[(\\+|\\-)]{1}[0-9]{4}"));
    if (! fuso_match.empty()) {
        std::string fusoStr = std::string(fuso_match[0]).substr(0,3);
        int fuso = std::stoi(fusoStr);
        res.push_back({ResultT(std::to_string(ora+fuso), 1)});
    }

    //PRELEVO PATH
    std::smatch path_match;
    std::regex_search(line, path_match, std::regex(" \\/[^ ]*"));
    if (! path_match.empty()) {
        std::string path = std::string(path_match[0]).substr(1, std::string(path_match[0]).length());
        res.push_back({ResultT(path, 1)});
    }
    //PRELEVO CODICE REQ
    std::smatch code_match;
    std::regex_search(line, code_match, std::regex(" [1-5][0-9]{2} "));
    if (! code_match.empty()) {
        std::string code = std::string(code_match[0]).substr(1, 3) + "-" + std::string(ip_match[0]);
        res.push_back({ResultT(code, 1)});
    }
    return res;
}


template<typename ReducerInputT, typename ResultT>
ResultT mapreduce_reduce(ReducerInputT input){
    auto new_accs = input.getAccum()+input.getMapResult().getValue();
    return ResultT(input.getMapResult().getKey(),new_accs);
}


template<typename T,typename R, typename M>
std::map<std::string, T> mapreduce(std::ifstream& file_input,M mapfun, R redfun) {
    std::map<std::string, T> accs{};
    std::string line;
    while (getline(file_input, line)) {
        //chiamando la map devo risolvere i tipi template, che saranno "string" e "result" (che a sua volta è una classe template)
        auto mapResults = mapfun(MapperInput(line));
        //stampa mapResults map
        for (auto &p : mapResults) {
            std::cout << "(" << p.getKey() << " " << p.getValue() << ") ";
            auto reduceResult = redfun(ReducerInput(p, accs[p.getKey()]));
            accs[reduceResult.getKey()] = reduceResult.getValue();
        }
    }
    return accs;
}

