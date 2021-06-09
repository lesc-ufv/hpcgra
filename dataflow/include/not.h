#ifndef NOT_H
#define NOT_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>

class Not : public Operator {
public:
    explicit Not(int id) : Operator(id, "not", OP_BASIC, "not") {}

    static Operator *create(Params params) {
        return new Not(params.id);
    }

    void compute() override {
        if (Operator::getSrc(0)) {
            auto v = ~Operator::getSrc(0)->getVal();
            Operator::setVal(v);
        }
    }
};


#endif //NOT_H
