#include "../include/read_arch.h"
#include <vector>

bool read_arch(std::string &arch_file, 
               std::vector<pe_t> &pe, 
               std::map<std::string, int> &map_type
            ) {

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

        std::map<int,std::vector<bool>> input_zone;
        int c = 0;
        for (auto in : data["input_zone"]) {
            input_zone[c] = std::vector<bool>(size_pe,false);
            for (auto v : in) {
                input_zone[c][v.asInt()] = true;
            }
            ++c;
        }

        std::map<int,std::vector<bool>> output_zone;
        c = 0;
        for (auto out : data["output_zone"]) {
            output_zone[c] = std::vector<bool>(size_pe,false);
            for (auto v : out) {
                output_zone[c][v.asInt()] = true;
            }
            ++c;
        }
        
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

        const unsigned int SIZE_TYPE = map_type.size();

        for (int i = 0; i < size_pe; ++i) {
            pe_t aux_pe;
            aux_pe.isa = new bool[SIZE_TYPE];
            aux_pe.id = data["pe"][i]["id"].asInt();
            
            aux_pe.type = -1;

            for (int j = 0; j < SIZE_TYPE; ++j)
                aux_pe.isa[j] = false;
            
            for (auto in : input_zone) {
                if (in.second[aux_pe.id]) {
                    aux_pe.isa[0] = true;
                    break;
                }
            }

            for (auto out : output_zone) {
                if (out.second[aux_pe.id]) {
                    aux_pe.isa[1] = true;
                    break;
                }
            }

            if (aux_pe.isa[0] && aux_pe.isa[1])
                aux_pe.type = 2;
            else if (aux_pe.isa[0])
                aux_pe.type = 0;
            else if (aux_pe.isa[1])
                aux_pe.type = 1;

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
