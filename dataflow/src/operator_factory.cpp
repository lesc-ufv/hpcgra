#include <operator_factory.h>

OperatorFactory::OperatorFactory() {

    Register("abs", &Abs::create);
    Register("add", &Add::create);
    Register("addi", &Addi::create);
    Register("sub", &Sub::create);
    Register("subi", &Subi::create);
    Register("and", &And::create);
    Register("andi", &Andi::create);
    Register("seq", &Seq::create);
    Register("seqi", &Seqi::create);
    Register("sne", &Sne::create);
    Register("snei", &Snei::create);
    Register("input", &InputStream::create);
    Register("output", &OutputStream::create);
    Register("max", &Max::create);
    Register("maxi", &Maxi::create);
    Register("min", &Min::create);
    Register("mini", &Mini::create);
    Register("mul", &Mul::create);
    Register("muli", &Muli::create);
    Register("mux", &Mux::create);
    Register("muxi", &Muxi::create);
    Register("not", &Not::create);
    Register("or", &Or::create);
    Register("ori", &Ori::create);
    Register("xor", &Xor::create);
    Register("xori", &Xori::create);
    Register("sgt", &Sgt::create);
    Register("sgti", &Sgti::create);
    Register("shl", &Shl::create);
    Register("shli", &Shli::create);
    Register("shr", &Shr::create);
    Register("shri", &Shri::create);
    Register("slt", &Slt::create);
    Register("slti", &Slti::create);
    Register("const", &Const::create);
    Register("reg", &Reg::create);
}

void OperatorFactory::Register(const std::string &operatorName, pfnCreate_t pfnCreate) {
    m_FactoryMap[operatorName] = pfnCreate;
}

Operator *OperatorFactory::CreateOperator(const std::string &operatorName, Params &params) {
    auto it = m_FactoryMap.find(operatorName);
    if (it != m_FactoryMap.end())
        return it->second(params);
    return nullptr;
}
