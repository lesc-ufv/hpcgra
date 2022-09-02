#include "../include/split.h"

std::vector<std::string> split(std::string data, std::string token) {
    std::vector<std::string> output;
    size_t pos = std::string::npos; // size_t to avoid improbable overflow
    do {
        pos = data.find(token);
        output.push_back(data.substr(0, pos));
        if (std::string::npos != pos)
            data = data.substr(pos + token.size());
    } while (std::string::npos != pos);
    return output;
}