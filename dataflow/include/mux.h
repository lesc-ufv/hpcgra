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
        if (Operator::getSrc(0) && Operator::getSrc(1) && Operator::getSrc(2)) {
            auto v = Operator::getSrc(0)->getVal() ? Operator::getSrc(1)->getVal()
                                                       : Operator::getSrc(2)->getVal();
            Operator::setVal(v);
        }
    }
};

class Muxi : public Operator {
public:
    Muxi(int id, int constant) : Operator(id, "mux", OP_IMMEDIATE, "muxi") {
        setConst(2,constant);
    }

    static Operator *create(Params params) {
        return new Muxi(params.id, params.constant0);
    }

    void compute() override {
        if (Operator::getSrc(0) && Operator::getSrc(1)) {
            auto v = Operator::getSrc(0)->getVal() ? Operator::getSrc(1)->getVal() : Operator::getConst(2);
            Operator::setVal(v);
        }
    }
};

class Muxii : public Operator {
public:
    Muxii(int id, unsigned short constant1, unsigned short constant2) : Operator(id, "mux", OP_IMMEDIATE, "muxii") {
        setConst(1,constant1);
        setConst(2,constant2);
    }

    static Operator *create(Params params) {
        return new Muxii(params.id, params.constant0,params.constant1);
    }

    void compute() override {
        if (Operator::getSrc(0)) {
            auto v = Operator::getSrc(0)->getVal() ? Operator::getConst(1) : Operator::getConst(2);
            Operator::setVal(v);
        }
    }
};
#endif //MAIN_MUX_H
