#ifndef __READ_ARCH_H
#define __READ_ARCH_H

#include <json/json.h>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

struct pe_t {
    int id;
    int type;
    int routes;
    bool acc;
    std::vector<int> neighbors;
    std::vector<int> elastic_queue;
    bool *isa;
    std::vector<int> inputs, outputs, basics;
};

bool read_arch(std::string &arch_file, std::vector<pe_t> &pe) {

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

    int count = map_type.size();

    try {
        int size_pe = data["pe"].size();
        int size_neighbors = 0;
        int size_elastic_queue = 0;
        int size_isa = 0;

        for (int i = 0; i < size_pe; ++i) {
            size_isa = data["pe"][i]["isa"].size();
            for (int j = 0; j < size_isa; ++j) {
                std::string type_name = data["pe"][i]["isa"][j].asString();
                if (map_type.count(type_name) == 0) {
                    map_type[type_name] = count++;
                }
            }
        }

        SIZE_TYPE = map_type.size();

        for (int i = 0; i < size_pe; ++i) {
            pe_t aux_pe;
            aux_pe.isa = new bool[SIZE_TYPE];
            aux_pe.id = data["pe"][i]["id"].asInt();
            
            aux_pe.type = -1;

            for (int j = 0; j < SIZE_TYPE; ++j)
                aux_pe.isa[j] = false;
            
            aux_pe.isa[0] = (data["pe"][i]["num_istream"].asInt() >= 1); // input
            aux_pe.isa[1] = (data["pe"][i]["num_ostream"].asInt() >= 1); // output

            if (aux_pe.isa[0] && aux_pe.isa[1])
                aux_pe.type = 2;
            else if (aux_pe.isa[0])
                aux_pe.type = 0;
            else if (aux_pe.isa[1])
                aux_pe.type = 1;

            /*aux_pe.isa[aux_pe.type] = true;
            if (aux_pe.type == 2) {
                aux_pe.isa[0] = true;
                aux_pe.isa[1] = true;
            }*/

            size_neighbors = data["pe"][i]["neighbors"].size();
            for (int j = 0; j < size_neighbors; ++j) {
                aux_pe.neighbors.push_back(data["pe"][i]["neighbors"][j].asInt());
            }

            aux_pe.routes = data["pe"][i]["routes"].asInt();

            size_elastic_queue = data["pe"][i]["elastic_queue"].size();
            for (int j = 0; j < size_elastic_queue; ++j) {
                aux_pe.elastic_queue.push_back(data["pe"][i]["elastic_queue"][j].asInt());
            }

            size_isa = data["pe"][i]["isa"].size();
            for (int j = 0; j < size_isa; ++j) {
                aux_pe.isa[map_type[data["pe"][i]["isa"][j].asString()]] = true;
            }

            pe.push_back(aux_pe);
        }
    } catch (std::exception& e) {
        std::cout << "Standard exception: " << e.what() << "\n";
        return false;
    }

    return true;
}

#endif
