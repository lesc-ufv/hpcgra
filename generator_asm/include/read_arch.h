#ifndef __READ_ARCH_H
#define __READ_ARCH_H

#include <json/json.h>
#include <fstream>
#include <iostream>
#include <map>

using namespace std;

/* 
    "input" = 0
    "output" = 1
    "inout" = 2
    "basic" = 3
*/
/*
    'NDEF': 0
    'add': 1
    'sub': 2
    'mul': 3
    'or': 4    
    'xor': 5
    'and': 6
    'not': 7
    'abs': 8
    'pass': 9
    'muladd': 10 
    'mulsub': 11
    'addadd': 12
    'subsub': 13
    'addsub': 14
    'mux': 15
    'slt': 16
    'sgt': 17
    'seq': 18
    'sne': 19
    'shl': 20
    'shr': 21
    'max': 22
    'min': 23

    
*/

typedef struct pe_t {
    int id;
    int type;
    vector<int> neighbors;
    int routes;
    vector<int> elastic_queue;
    bool acc;
    vector<int> isa;
} pe_t;

bool read_arch(string &arch_file, vector<pe_t> &pe) {

    // map to 
    map<string, int> map_type = {{"input",  0},
                                 {"output", 1},
                                 {"inout",  2},
                                 {"basic",  3}};

    // map to isa
    std::map<std::string, int> map_isa = {{"add", 1},
                                            {"sub", 2},
                                            {"mul", 3},
                                            {"or", 4},    
                                            {"xor", 5},
                                            {"and", 6},
                                            {"not", 7},
                                            {"abs", 8},
                                            {"pass", 9},
                                            {"muladd", 10}, 
                                            {"mulsub", 11},
                                            {"addadd", 12},
                                            {"subsub", 13},
                                            {"addsub", 14},
                                            {"mux", 15},
                                            {"slt", 16},
                                            {"sgt", 17},
                                            {"seq", 18},
                                            {"sne", 19},
                                            {"shl", 20},
                                            {"shr", 21},
                                            {"max", 22},
                                            {"min", 23}};

    Json::Value data;
    std::ifstream ifs;
    ifs.open(arch_file);
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;
    if (!parseFromStream(builder, ifs, &data, &errs)) {
        std::cout << errs << std::endl;
        return false;
    }
    ifs.close();

    try {
        int size_pe = data["pe"].size();
        int size_neighbors = 0;
        int size_elastic_queue = 0;
        int size_isa = 0;

        for (int i = 0; i < size_pe; ++i) {
            pe_t aux_pe;
            aux_pe.id = data["pe"][i]["id"].asInt();
            aux_pe.type = map_type[data["pe"][i]["type"].asString()];

            size_neighbors = data["pe"][i]["neighbors"].size();
            for (int j = 0; j < size_neighbors; ++j) {
                aux_pe.neighbors.push_back(data["pe"][i]["neighbors"][j].asInt());
            }

            aux_pe.routes = data["pe"][i]["routes"].asInt();

            size_elastic_queue = data["pe"][i]["elastic_queue"].size();
            for (int j = 0; j < size_elastic_queue; ++j) {
                aux_pe.elastic_queue.push_back(data["pe"][i]["elastic_queue"][j].asInt());
            }

            aux_pe.acc = data["pe"][i]["acc"].asBool();

            size_isa = data["pe"][i]["isa"].size();
            for (int j = 0; j < size_isa; ++j) {
                aux_pe.isa.push_back(map_isa[data["pe"][i]["isa"][j].asString()]);
            }
            pe.push_back(aux_pe);
        }
    } catch (exception& e) {
        cout << "Standard exception: " << e.what() << endl;
        return false;
    }

    return true;
}

#endif
