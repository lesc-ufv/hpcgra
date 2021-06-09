#include <operator.h>

#include <utility>

Operator::Operator(int id, std::string op_code, int type, std::string label){

    m_id = id;
    m_data_flow_id = -1;
    m_op_code = std::move(op_code);
    m_level = -1;
    m_type = type;
    m_val = 0;
    m_is_end = false;
    m_label = std::move(label);
}

Operator::~Operator() {
    m_constants.clear();
    m_src.clear();
    m_dst.clear();
}

int Operator::getId() const {
    return m_id;
}

void Operator::setId(int id) {
    m_id = id;
}

std::string Operator::getOpCode() const {
    return m_op_code;
}

void Operator::setOpCode(std::string op_code) {
    m_op_code = op_code;
}

int Operator::getType() const {
    return m_type;
}

void Operator::setType(int type) {
    m_type = type;
}

void Operator::setVal(int val) {
    m_val = val;
}

short Operator::getVal() const {
    return m_val;
}

void Operator::setSrc(Operator *src, int port) {
    assert(port < 3);
    m_src[port] = src;
}

Operator * Operator::getSrc(int port){
   assert(port < 3);
   return m_src[port];
}

void Operator::addDst(Operator * op_dst, int port){
    m_dst.emplace_back(port,op_dst);
}

std::vector<std::pair<int,Operator*>> &Operator::getDst() {
    return Operator::m_dst;
}

void Operator::setConst(int port, unsigned short value) {
    assert(port < 3);
    m_constants[port] = value;
}

unsigned short Operator::getConst(int port){
    assert(port < 3);
    return m_constants[port];
}

std::map<int,unsigned short> &Operator::getConst() {
    return m_constants;
}

void Operator::setLevel(int level) {
    m_level = level;
}

int Operator::getLevel() const {
    return m_level;
}

void Operator::setDataFlowId(int dataFlowId) {
    m_data_flow_id = dataFlowId;
}

int Operator::getDataFlowId() const {
    return m_data_flow_id;
}

const std::string &Operator::getLabel() const {
    return m_label;
}

int Operator::getIsEnd() const {
    return m_is_end;
}

void Operator::setIsEnd(bool isEnd) {
    m_is_end = isEnd;
}

