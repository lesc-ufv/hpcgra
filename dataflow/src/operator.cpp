#include <operator.h>

#include <utility>
#include <iostream>

Operator::Operator(int id, std::string op_code, int type, std::string label, int size)
{

    m_id = id;
    m_data_flow_id = -1;
    m_op_code = std::move(op_code);
    m_level = -1;
    m_type = type;
    m_label = std::move(label);
    m_size = size;
    for (int i = 0; i < size; i++)
    {
        m_val.emplace_back(0);
        m_is_end.emplace_back(false);
    }
}

Operator::~Operator()
{
    m_constants.clear();
    m_inputs.clear();
    m_outputs.clear();
    m_val.clear();
    m_is_end.clear();
}

int Operator::getId() const
{
    return m_id;
}

void Operator::setId(int id)
{
    m_id = id;
}

int Operator::getSize() const
{
    return m_size;
}

void Operator::setSize(int size)
{
    m_size = size;
}

std::string Operator::getOpCode() const
{
    return m_op_code;
}

void Operator::setOpCode(std::string op_code)
{
    m_op_code = op_code;
}

int Operator::getType() const
{
    return m_type;
}

void Operator::setType(int type)
{
    m_type = type;
}

void Operator::setVal(int val, int idx)
{
    m_val[idx] = val;
}

short Operator::getVal(int idx) const
{
    return m_val[idx];
}

void Operator::addSrc(Operator *src, int port)
{
    m_inputs[port] = src;
}

Operator *Operator::getSrc(int port)
{
    if (m_inputs.find(port) != m_inputs.end())
    {
        return m_inputs[port];
    }
    assert((1) && "Source operator not found!");
}

void Operator::addDst(Operator *dst, int port)
{
    m_outputs[port].push_back(dst);
}

std::vector<Operator *> &Operator::getDst(int port)
{

    if (m_outputs.find(port) != m_outputs.end())
    {
        return m_outputs[port];
    }
    assert((1) && "Dst operator not found!");
}

std::vector<int> Operator::getSrcPort(Operator *op)
{
    std::vector<int> ports;
    for (auto it = m_inputs.begin(); it != m_inputs.end(); ++it)
    {
        if (it->second->getId() == op->getId())
        {
            ports.emplace_back(it->first);
        }
    }
    return ports;
}

std::vector<int> Operator::getDstPort(Operator *op)
{
    std::vector<int> ports;
    for (auto it = m_outputs.begin(); it != m_outputs.end(); ++it)
    {
        for (auto out : it->second)
        {
            if (out->getId() == op->getId())
            {
                ports.emplace_back(it->first);
            }
        }
    }
    return ports;
}

std::map<int, std::vector<Operator *>> &Operator::getOutputs()
{
    return m_outputs;
}

std::map<int, Operator *> &Operator::getInputs()
{
    return m_inputs;
}

void Operator::setConst(int port, unsigned short value)
{
    m_constants[port] = value;
}

unsigned short Operator::getConst(int port)
{
    return m_constants[port];
}

std::map<int, unsigned short> &Operator::getConst()
{
    return m_constants;
}

void Operator::setLevel(int level)
{
    m_level = level;
}

int Operator::getLevel() const
{
    return m_level;
}

void Operator::setDataFlowId(int dataFlowId)
{
    m_data_flow_id = dataFlowId;
}

int Operator::getDataFlowId() const
{
    return m_data_flow_id;
}

const std::string &Operator::getLabel() const
{
    return m_label;
}

int Operator::getIsEnd(int idx) const
{
    return m_is_end[idx];
}

void Operator::setIsEnd(bool isEnd, int idx)
{
    m_is_end[idx] = isEnd;
}

void Operator::print()
{
    std::cout << "Op. ID: " << m_id << std::endl;
    std::cout << "Op. DF. ID: " << m_data_flow_id << std::endl;
    std::cout << "Op. Level: " << m_level << std::endl;
    std::cout << "Op. Type: " << m_type << std::endl;
    std::cout << "Op. Size: " << m_size << std::endl;
    std::cout << "Op. Values: ";
    for (auto v : m_val)
        std::cout << v << " ";
    std::cout << std::endl;
    std::cout << "Op. Is End: ";
    for (auto v : m_is_end)
        std::cout << v << " ";
    std::cout << std::endl;
    std::cout << "Op. Opcde:" << m_op_code << std::endl;
    std::cout << "Op. Label:" << m_label << std::endl;
    std::cout << "Op. Constants(port:value):";
    for (auto v : m_constants)
        std::cout << v.first << " : " << v.second;
    std::cout << std::endl;

    std::cout << "Op. Inputs(port:id): ";
    for (auto v : m_inputs)
        std::cout << v.first << " : " << v.second->getId() << " ";
    std::cout << std::endl;

    std::cout << "Op. Outputs(port:[id ...]):";
    for (auto v : m_outputs)
    {
        std::cout << v.first << " : [";
        for (auto i : v.second)
            std::cout << i->getId() << " ";
    }
    std::cout << "]" << std::endl;
}