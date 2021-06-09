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
        if(Operator::getSrc(0)){
            auto v = abs(Operator::getSrc(0)->getVal());
            Operator::setVal(v);
        }
    }
};

#endif //ABS_H
