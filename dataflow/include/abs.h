#ifndef ABS_H
#define ABS_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>

class Abs : public Operator {
public:

    explicit Abs(int id) : Operator(id, "abs", OP_BASIC, "abs",1) {}

    static Operator *create(Params params) {
        return new Abs(params.id);
    }
    void compute() override {
        if(Operator::getSrc(0)){
            auto v = abs(Operator::getSrc(0)->getVal(0));
            Operator::setVal(v,0);
        }
    }
};

#endif //ABS_H
