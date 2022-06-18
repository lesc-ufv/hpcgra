#include <operator.h>

#include <utility>

Operator::Operator(int id, std::string op_code, int type, std::string label, int size){

    m_id = id;
    m_data_flow_id = -1;
    m_op_code = std::move(op_code);
    m_level = -1;
    m_type = type;
    m_label = std::move(label);
    m_size = size;
    for(int i=0;i < size;i++){
        m_val.emplace_back(0);
        m_is_end.emplace_back(false);
    }
}

Operator::~Operator() {
    m_constants.clear();
    m_inputs.clear();
    m_outputs.clear();
    m_val.clear();
    m_is_end.clear();
}

int Operator::getId() const {
    return m_id;
}

void Operator::setId(int id) {
    m_id = id;
}

int Operator::getSize() const {
    return m_size;
}

void Operator::setSize(int size) {
    m_size = size;
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

void Operator::setVal(int val, int idx) {
    m_val[idx] = val;
}

short Operator::getVal(int idx) const {
    return m_val[idx];
}

void Operator::addSrc(Operator *src, int port) {
    m_inputs[port] = src;
}

Operator * Operator::getSrc(int port){
    for (auto it = m_inputs.begin(); it != m_inputs.end(); ++it){
        if (it->first == port){
            return it->second;
        }
    }
    assert((1) && "Source operator not found!");
    return nullptr;
}

int Operator::getSrcPort(Operator * op){
    int port = -1;
    for (auto it = m_inputs.begin(); it != m_inputs.end(); ++it){
        if (it->second->getId() == op->getId()){
            port = it->first;
            break;
        }
    }
    assert((port >= 0) && "Source port not found!");
    return port;
}

int Operator::getDstPort(Operator * op){
    int port = -1;
    for (auto it = m_outputs.begin(); it != m_outputs.end(); ++it) {
        for(auto out : it->second) {
            if (out->getId() == op->getId()){
                port = it->first;
                break;
            }
        }
    }
    assert((port >= 0) && "Source port not found!");
    return port;
}

void Operator::addDst(Operator * op_dst, int dstPort){
    m_outputs[dstPort].push_back(op_dst);
}

std::map<int, std::vector<Operator*>> &Operator::getDst() {
    return m_outputs;
}

std::map<int, Operator*> &Operator::getAllSrc() {
    return m_inputs;
}

void Operator::setConst(int port, unsigned short value) {
    m_constants[port] = value;
}

unsigned short Operator::getConst(int port){
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

int Operator::getIsEnd(int idx) const {
    return m_is_end[idx];
}

void Operator::setIsEnd(bool isEnd,int idx) {
    m_is_end[idx] = isEnd;
}

