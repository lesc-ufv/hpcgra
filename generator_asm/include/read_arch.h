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

bool read_arch(std::string &arch_file, 
               std::vector<pe_t> &pe, 
               std::map<std::string, int> &map_type
            );

#endif
