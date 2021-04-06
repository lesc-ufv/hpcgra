#include <utility>

#ifndef PARAMS_H
#define PARAMS_H

class Params {
public:
    int id;
    std::vector<int*> constants;
    short *data;
    int size;

    explicit Params(int id) : id(id),
                     data(nullptr),
                     size(0) {}

    Params(int id, std::vector<int*> constants) : id(id),
                                   constants(std::move(constants)),
                                   data(nullptr),
                                   size(0) {}

    Params(int id, std::vector<int*> constants, short *data, int size) : id(id),
                                                        constants(std::move(constants)),
                                                        data(data),
                                                        size(size) {}

    ~Params() = default;
};

#endif //PARAMS_H
