#ifndef ABS_H
#define ABS_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>

class Abs : public Operator {
public:

    explicit Abs(int id) : Operator(id, "abs", OP_BASIC, "abs") {}

    static Operator *create(Params params) {
        return new Abs(params.id);
    }
    void compute() override {
        if (Operator::getSrcA()) {
            auto v = abs(Operator::getSrcA()->getVal());
            Operator::setVal(v);
        } else if (Operator::getSrcB()) {
            auto v = abs(Operator::getSrcB()->getVal());
            Operator::setVal(v);
        }
    }
};

#endif //ABS_H
