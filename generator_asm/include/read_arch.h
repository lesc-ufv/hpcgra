#ifndef __READ_ARCH_H
#define __READ_ARCH_H

#include <json/json.h>
#include <fstream>
#include <iostream>
#include <map>

using namespace std;

typedef struct pe_t {
    int id;
    int type;
    int routes;
    bool acc;
    vector<int> neighbors;
    vector<int> elastic_queue;
    bool isa[SIZE_TYPE];
    vector<int> inputs, outputs, basics;
} pe_t;

bool read_arch(string &arch_file, vector<pe_t> &pe) {

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

            for (int j = 0; j < SIZE_TYPE; ++j)
                aux_pe.isa[j] = false;

            aux_pe.isa[aux_pe.type] = true;

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
                aux_pe.isa[map_type[data["pe"][i]["isa"][j].asString()]] = true;
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
