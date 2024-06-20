#ifndef GENERIC_H
#define GENERIC_H

#include <data_flow_defs.h>
#include <operator.h>
#include <params.h>
#include <utility>

class Generic : public Operator {
public:
    explicit Generic(int id, std::string opcode, int size) : Operator(id, opcode, OP_GENERIC, opcode, size) {}

    static Operator *create(Params params) {
        return new Generic(params.id, params.opcode, params.size);
    }

    void compute() override {
    }
};

#endif //GENERIC_H
