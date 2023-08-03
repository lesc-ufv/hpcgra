#ifndef OPERATOR_H
#define OPERATOR_H

#include <vector>
#include <string>
#include <utility>
#include <assert.h>
#include <cstdlib>
#include <map>

class Operator
{

private:
    int m_id;
    int m_data_flow_id;
    int m_level;
    int m_type;
    int m_size;
    std::vector<unsigned short> m_val;
    std::vector<bool> m_is_end;
    std::string m_op_code;
    std::string m_label;
    std::map<int, unsigned short> m_constants;
    std::map<int, Operator *> m_inputs;
    std::map<int, std::vector<Operator *>> m_outputs;

public:
    Operator(int id, std::string op_code, int type, std::string label, int size);

    ~Operator();

    void print();

    int getId() const;

    void setId(int id);

    int getSize() const;

    void setSize(int size);

    std::string getOpCode() const;

    void setOpCode(std::string op_code);

    int getType() const;

    void setType(int type);

    short getVal(int idx) const;

    void setVal(int val, int idx);

    void addSrc(Operator *src, int port);

    void rmSrc(int port);

    Operator *getSrc(int port);

    void addDst(Operator *dst, int port);

    void rmDst(Operator *dst, int port);

    std::vector<Operator *> &getDst(int port);

    std::vector<int> getSrcPort(Operator *op);

    std::vector<int> getDstPort(Operator *op);

    std::map<int, std::vector<Operator *>> &getOutputs();

    std::map<int, Operator *> &getInputs();

    void setConst(int port, unsigned short value);

    unsigned short getConst(int port);

    std::map<int, unsigned short> &getConst();

    void setLevel(int level);

    int getLevel() const;

    void setDataFlowId(int data_flow_id);

    int getDataFlowId() const;

    const std::string &getLabel() const;

    virtual void compute() = 0;

    int getIsEnd(int idx) const;

    void setIsEnd(bool isEnd, int idx);
};

#endif // OPERATOR_H
