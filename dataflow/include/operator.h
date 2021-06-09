#ifndef OPERATOR_H
#define OPERATOR_H

#include <vector>
#include <string>
#include <utility>
#include <assert.h>
#include <cstdlib>
#include<map>

class Operator {

private:
    int m_id;
    int m_data_flow_id;
    int m_level;
    int m_type;
    unsigned short m_val;
    bool m_is_end;
    std::string m_op_code;
    std::string m_label;
    std::map<int,unsigned short> m_constants;
    std::map<int, Operator*> m_src;
    std::vector<std::pair<int, Operator*>> m_dst;
    
public:
    Operator(int id, std::string op_code, int type, std::string label);

    ~Operator();

    int getId() const;

    void setId(int id);

    std::string getOpCode() const;

    void setOpCode(std::string op_code);

    int getType() const;

    void setType(int type);

    short getVal() const;

    void setVal(int val);
    
    void setSrc(Operator *src, int port);
    
    Operator *getSrc(int port);
    
    void addDst(Operator * op_dst, int port);
    
    std::vector<std::pair<int, Operator*>> &getDst();

    void setConst(int  port, unsigned short value);

    unsigned short getConst(int port);
    
    std::map<int, unsigned short> &getConst();
    
    void setLevel(int level);

    int getLevel() const;

    void setDataFlowId(int data_flow_id);

    int getDataFlowId() const;

    const std::string &getLabel() const;

    virtual void compute() = 0;

    int getIsEnd() const;

    void setIsEnd(bool isEnd);
};

#endif //OPERATOR_H
