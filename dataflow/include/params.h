#include <utility>

#ifndef PARAMS_H
#define PARAMS_H

class Params {
public:
    int id;
    unsigned short constant0;
    unsigned short constant1;
    unsigned short constant2;
    unsigned short *data;
    int size;

    explicit Params(int id) : id(id),
                     data(nullptr),
                     size(0) {}

    Params(int id, unsigned short constant0) : id(id),
                                   constant0(constant0),
                                   data(nullptr),
                                   size(0) {}

    Params(int id, unsigned short constant0, unsigned short *data, int size) : id(id),
                                                        constant0(constant0),
                                                        data(data),
                                                        size(size) {}
                                                        
    Params(int id, unsigned short constant0,unsigned short constant1) : id(id),
                                   constant0(constant0),
                                   constant1(constant1),
                                   data(nullptr),
                                   size(0) {}

    Params(int id, unsigned short constant0,unsigned short constant1, unsigned short *data, int size) : id(id),
                                                        constant0(constant0),
                                                        constant1(constant1),
                                                        data(data),
                                                        size(size) {}
    Params(int id, unsigned short constant0,unsigned short constant1,unsigned short constant2) : id(id),
                                   constant0(constant0),
                                   constant1(constant1),
                                   constant2(constant2),
                                   data(nullptr),
                                   size(0) {}

    Params(int id, unsigned short constant0,unsigned short constant1,unsigned short constant2, unsigned short *data, int size) : id(id),
                                                        constant0(constant0),
                                                        constant1(constant1),
                                                        constant2(constant2),
                                                        data(data),
                                                        size(size) {}

    ~Params() = default;
};

#endif //PARAMS_H
