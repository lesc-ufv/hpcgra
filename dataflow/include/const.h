#ifndef CONST_H
#define CONST_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>

class Const : public Operator
{
public:
    Const(int id, unsigned short constant, std::string label) : Operator(id, "const", OP_IMMEDIATE, label, 1)
    {
        setConst(1, constant);
    }

    static Operator *create(Params params)
    {
        return new Const(params.id, params.constant0, params.label);
    }

    void compute() override
    {
        if (Operator::getSrc(0))
        {
            auto v = Operator::getConst(0);
            Operator::setVal(v, 0);
        }
    }
};

#endif // CONST_H
