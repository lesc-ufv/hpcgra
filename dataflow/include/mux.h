#ifndef MUX_H
#define MUX_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>

class Mux : public Operator {
public:
    explicit Mux(int id) : Operator(id, "mux", OP_BASIC, "mux") {}

    static Operator *create(Params params) {
        return new Mux(params.id);
    }

    void compute() override {
        if (Operator::getSrcA() && Operator::getSrcB() && Operator::getBranchIn()) {
            auto v = Operator::getBranchIn()->getVal() ? Operator::getSrcA()->getVal()
                                                       : Operator::getSrcB()->getVal();
            Operator::setVal(v);
        }
    }
};

class Muxi : public Operator {
public:
    Muxi(int id, int constant1, int constant2) : Operator(id, "mux", OP_IMMEDIATE, "muxi") {
        setConst(1,constant1);
        setConst(2,constant2);
    }

    static Operator *create(Params params) {
        return new Muxi(params.id, params.constants[0][1],params.constants[1][1]);
    }

    void compute() override {
        if (Operator::getSrcA() && Operator::getBranchIn()) {
            auto v = Operator::getBranchIn()->getVal() ? Operator::getSrcA()->getVal() : Operator::getConst()[0][1];
            Operator::setVal(v);
        }
    }
};

#endif //MAIN_MUX_H
