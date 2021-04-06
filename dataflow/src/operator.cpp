#include <operator.h>
#include <utility>

Operator::Operator(int id, std::string op_code, int type, std::string label) :
        id(id),
        opCode(std::move(op_code)),
        type(type),
        srcA(nullptr),
        srcB(nullptr),
        branchIn(nullptr),
        level(0),
        dataFlowId(-1),
        val(0),
        isEnd(false),
        label(std::move(label)) {

}

Operator::Operator(int id, std::string op_code, int type, std::string label, std::vector<int*> constants) :
        id(id),
        opCode(std::move(op_code)),
        type(type),
        srcA(nullptr),
        srcB(nullptr),
        branchIn(nullptr),
        constants(std::move(constants)),
        level(0),
        dataFlowId(-1),
        label(std::move(label)),
        val(0),
        isEnd(false) {

}

Operator::~Operator() {
    Operator::dst.clear();
}

int Operator::getId() const {
    return Operator::id;
}

void Operator::setId(int id) {
    Operator::id = id;
}

std::string Operator::getOpCode() const {
    return Operator::opCode;
}

void Operator::setOpCode(std::string op_code) {
    Operator::opCode = op_code;
}

int Operator::getType() const {
    return Operator::type;
}

void Operator::setType(int type) {
    Operator::type = type;
}

short Operator::getVal() const {
    return Operator::val;
}

void Operator::setVal(int val) {
    Operator::val = val;
}

std::vector<Operator *> &Operator::getDst() {
    return Operator::dst;
}

Operator *Operator::getSrcA() const {
    return Operator::srcA;
}

void Operator::setSrcA(Operator *srcA) {
    Operator::srcA = srcA;
}

Operator *Operator::getSrcB() const {
    return Operator::srcB;
}

void Operator::setSrcB(Operator *srcB) {
    Operator::srcB = srcB;
}

std::vector<int *> &Operator::getConst(){
    return constants;
}

void Operator::setConst(int port, int value) {
    auto c = new int[2];
    c[0] = port;
    c[1] = value;
    Operator::constants.emplace_back(c);
}

void Operator::setLevel(int level) {
    Operator::level = level;
}

int Operator::getLevel() const {
    return Operator::level;
}

Operator *Operator::getBranchIn() const {
    return Operator::branchIn;
}

void Operator::setBranchIn(Operator *branchIn) {
    Operator::branchIn = branchIn;
}

void Operator::setDataFlowId(int dataFlowId) {
    Operator::dataFlowId = dataFlowId;
}

int Operator::getDataFlowId() const {
    return Operator::dataFlowId;
}

const std::string &Operator::getLabel() const {
    return Operator::label;
}

int Operator::getIsEnd() const {
    return Operator::isEnd;
}

void Operator::setIsEnd(bool isEnd) {
    Operator::isEnd = isEnd;
}

