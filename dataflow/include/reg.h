#ifndef REG_H
#define REG_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>

class Reg : public Operator {
public:

    explicit Reg(int id) : Operator(id, "reg", OP_BASIC, "reg",1) {}

    static Operator *create(Params params) {
        return new Reg(params.id);
    }
    void compute() override {
        if(Operator::getSrc(0)){
            auto v = Operator::getSrc(0)->getVal(0);
            Operator::setVal(v,0);
        }
    }
};

#endif //REG_H
