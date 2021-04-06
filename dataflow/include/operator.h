#ifndef OPERATOR_H
#define OPERATOR_H

#include <vector>
#include <string>

class Operator {

private:
    int id;
    int level;
    std::string opCode;
    int type;
    short val;
    std::vector<int *> constants;
    Operator *srcA;
    Operator *srcB;
    Operator *branchIn;
    std::vector<Operator *> dst;
    int dataFlowId;
    std::string label;
    bool isEnd;

public:
    Operator(int id, std::string op_code, int type, std::string label);

    Operator(int id, std::string op_code, int type, std::string label, std::vector<int *> constant);

    ~Operator();

    int getId() const;

    void setId(int id);

    std::string getOpCode() const;

    void setOpCode(std::string op_code);

    int getType() const;

    void setType(int type);

    short getVal() const;

    void setVal(int val);

    Operator *getSrcA() const;

    void setSrcA(Operator *srcA);

    Operator *getSrcB() const;

    void setSrcB(Operator *srcB);

    Operator *getBranchIn() const;

    void setBranchIn(Operator *branchIn);

    std::vector<Operator *> &getDst();

    std::vector<int *> &getConst();

    void setConst(int port, int value);

    void setLevel(int level);

    int getLevel() const;

    void setDataFlowId(int dataFlowId);

    int getDataFlowId() const;

    const std::string &getLabel() const;

    virtual void compute() = 0;

    int getIsEnd() const;

    void setIsEnd(bool isEnd);
};

#endif //OPERATOR_H
