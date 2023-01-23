#include <data_flow.h>

DataFlow::DataFlow(int id, std::string name) : id(id),
                                               name(std::move(name)),
                                               num_op_in(0),
                                               num_op_out(0),
                                               num_op(0),
                                               num_level(0) {}

DataFlow::~DataFlow()
{
    for (auto op : op_array)
    {
        delete op.second;
    }
    DataFlow::op_array.clear();
    for (auto g : DataFlow::graph)
    {
        g.second.clear();
    }
    DataFlow::graph.clear();
}

void DataFlow::addOperator(Operator *op)
{
    if (op->getDataFlowId() == -1)
    {
        op->setDataFlowId(DataFlow::id);
        DataFlow::op_array[op->getId()] = op;
        DataFlow::num_op++;
        if (op->getType() == OP_IN)
        {
            DataFlow::num_op_in++;
            DataFlow::input_op_ids.push_back(op->getId());
        }
        if (op->getType() == OP_OUT)
        {
            DataFlow::num_op_out++;
            DataFlow::output_op_ids.push_back(op->getId());
        }
    }
}

Operator *DataFlow::removeOperator(int op_id)
{
    Operator *r = DataFlow::op_array[op_id];
    DataFlow::op_array.erase(op_id);
    r->setDataFlowId(-1);
    return r;
}

void DataFlow::compute()
{

    auto n = DataFlow::getNumLevel();
    unsigned int allIsEnd = 0;
    while (allIsEnd != DataFlow::getNumOpIn())
    {
        allIsEnd = 0;
        for (int i = 0; i <= n; ++i)
        {
            for (auto item : DataFlow::getOpArray())
            {
                auto op = item.second;
                if (op->getLevel() == i)
                {
                    op->compute();
                    int end = 0;
                    for (int k = 0; k < op->getSize(); k++)
                    {
                        if (op->getType() == OP_IN && op->getIsEnd(k))
                        {
                            end = 1;
                        }
                        else
                        {
                            end = 0;
                            break;
                        }
                    }
                    allIsEnd += end;
                }
            }
            if (allIsEnd == DataFlow::getNumOpIn())
            {
                break;
            }
        }
    }
}

const std::map<int, Operator *> &DataFlow::getOpArray() const
{
    return op_array;
}

Operator *DataFlow::getOp(int id)
{
    if (DataFlow::op_array.find(id) != DataFlow::op_array.end())
    {
        return DataFlow::op_array[id];
    }
    return nullptr;
}

void DataFlow::toDOT(const std::string &fileNamePath)
{
    std::ofstream myfile;
    myfile.open(fileNamePath);
    myfile << "digraph " << DataFlow::name << "{" << std::endl;
    for (auto op : DataFlow::op_array)
    {

        if (op.second->getType() == OP_IN)
        {
            myfile << " " << op.first << " [ label = input" << op.second->getId() << " ]" << std::endl;
        }
        else if (op.second->getType() == OP_OUT)
        {
            myfile << " " << op.first << " [ label = output" << op.second->getId() << " ]" << std::endl;
        }
        else if (op.second->getType() == OP_IMMEDIATE)
        {
            myfile << " " << op.first;
            myfile << " [ label = " << op.second->getOpCode() << "i";
            myfile << ", value = \"[";
            auto v = op.second->getConst();
            int i = 0;
            for (auto c : v)
            {
                if (i < v.size() - 1)
                    myfile << "[" << c.first << "," << c.second << "],";
                else
                    myfile << "[" << c.first << "," << c.second << "]";
                i++;
            }
            myfile << "]\"]" << std::endl;

            for (auto c : v)
            {
                myfile << " \"" << op.first << "." << c.second << "\"[ label = " << c.second << " ]" << std::endl;
            }
        }
        else
        {
            myfile << " " << op.first << " [ label = " << op.second->getOpCode() << "]" << std::endl;
        }
    }
    for (auto op : DataFlow::op_array)
    {
        if (op.second->getType() == OP_IMMEDIATE)
        {
            for (auto c : op.second->getConst())
            {
                myfile << " \"" << op.first << "." << c.second << "\" -> " << op.first << std::endl;
            }
        }
        for (auto op_dst_arr : op.second->getOutputs())
        {
            for (auto op_dst : op_dst_arr.second)
            {
                myfile << " " << op.first << " -> " << op_dst->getId() << std::endl;
            }
        }
    }
    myfile << "}" << std::endl;
    myfile.close();
}

void DataFlow::toJSON(const std::string &fileNamePath)
{
    Json::Value df;
    Json::Value node;
    Json::Value edge;
    std::map<std::tuple<int, int, int, int>, bool> map_port;

    for (auto const item : op_array)
    {
        auto op = item.second;
        node["id"] = std::to_string(op->getId());
        node["opcode"] = op->getOpCode();
        node["label"] = op->getLabel();
        node["size"] = std::to_string(op->getSize());
        if (op->getType() == OP_IMMEDIATE)
        {
            auto v = op->getConst();
            for (auto c : v)
            {
                Json::Value j;
                j.append(std::to_string(c.first));
                j.append(std::to_string(c.second));
                node["const"].append(j);
            }
        }

        df["nodes"].append(node);
        node.clear();
    }
    for (auto item : DataFlow::op_array)
    {
        auto op = item.second;
        for (auto neighbor_arr : op->getOutputs())
        {
            for (auto nei : neighbor_arr.second)
            {
                edge["source"] = std::to_string(op->getId());
                edge["target"] = std::to_string(nei->getId());
                auto exitfor = false;
                for (auto dp : op->getDstPort(nei))
                {
                    for (auto sp : nei->getSrcPort(op))
                    {
                        auto key = std::tuple<int, int, int, int>(op->getId(), nei->getId(), sp, dp);

                        if (map_port.find(key) == map_port.end())
                        {
                            edge["source-port"] = std::to_string(dp);
                            edge["target-port"] = std::to_string(sp);
                            map_port[key] = true;
                            exitfor = true;
                            break;
                        }
                    }
                    if (exitfor)
                        break;
                }
                df["edges"].append(edge);
                edge.clear();
            }
        }
    }

    std::ofstream myfile;
    myfile.open(fileNamePath);
    Json::StreamWriterBuilder builder;
    const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(df, &myfile);
}

// TODO: Need to be FIXED
void DataFlow::toJsonOperator(const std::string &fileNamePath)
{
    Json::Value json;
    std::map<std::tuple<int, int, int, int>, bool> map_port;

    json["opcode"] = this->name;

    for (auto item : this->op_array)
    {
        int id = item.first;
        Operator *op = item.second;

        if (typeid(*op) == typeid(InputStream))
        {
            // do nothing
            json["inputs"].append(op->getOpCode() + std::to_string(op->getId()));
        }
        else if (typeid(*op) == typeid(OutputStream))
        {
            Operator *from = op->getSrc(0);
            std::string name = from->getOpCode() + std::to_string(from->getId());
            // if (from->getType() != OP_IN)
            //     name += ".out" + std::to_string(from->getDstPort(op));
            auto exitfor = false;
            for (auto dp : from->getDstPort(op))
            {
                for (auto sp : op->getSrcPort(from))
                {
                    auto key = std::tuple<int, int, int, int>(op->getId(), from->getId(), sp, dp);

                    if (map_port.find(key) == map_port.end())
                    {
                        name += ".out" + std::to_string(sp);
                        map_port[key] = true;
                        exitfor = true;
                        break;
                    }
                }
                if (exitfor)
                    break;
            }

            json["outputs"].append(name);
        }
        else
        {
            Json::Value df;
            df["type"] = op->getOpCode();
            df["label"] = op->getOpCode() + std::to_string(op->getId());
            Operator *x;
            for (int i = 0; x = op->getSrc(i); ++i)
            {
                std::string name = x->getOpCode() + std::to_string(x->getId());
                // if (x->getType() != OP_IN)
                //     name += ".out" + std::to_string(x->getDstPort(op));
                auto exitfor = false;
                for (auto dp : x->getDstPort(op))
                {
                    for (auto sp : op->getSrcPort(x))
                    {
                        auto key = std::tuple<int, int, int, int>(op->getId(), x->getId(), sp, dp);

                        if (map_port.find(key) == map_port.end())
                        {
                            name += ".out" + std::to_string(sp);
                            map_port[key] = true;
                            exitfor = true;
                            break;
                        }
                    }
                    if (exitfor)
                        break;
                }
                df["inputs"].append(name);
            }
            if (op->getType() == OP_IMMEDIATE)
            {
                for (auto c : op->getConst())
                {
                    df["inputs"].append(c.second);
                }
            }
            for (int i = 0; op->getOutputs()[i].size(); ++i)
            {
                df["outputs"].append("out" + std::to_string(i));
            }
            json["dataflow"].append(df);
        }
    }
    std::ofstream fout(fileNamePath);
    Json::StreamWriterBuilder builder;
    const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(json, &fout);
}

void DataFlow::connect(Operator *src, int srcPort, Operator *dst, int dstPort)
{

    DataFlow::addOperator(src);
    DataFlow::addOperator(dst);
    DataFlow::graph[src->getId()].push_back(dst->getId());
    src->addDst(dst, srcPort);
    dst->addSrc(src, dstPort);
    DataFlow::updateOpLevel();
}

void DataFlow::updateOpLevel()
{
    std::queue<int> q;
    int parent;
    for (auto op : DataFlow::op_array)
    {
        if (op.second->getType() == OP_IN)
        {
            q.push(op.first);
            while (!q.empty())
            {
                parent = q.front();
                q.pop();
                for (auto child : DataFlow::graph[parent])
                {
                    int lp = DataFlow::op_array[parent]->getLevel();
                    int lc = DataFlow::op_array[child]->getLevel();
                    if (lp >= lc)
                    {
                        DataFlow::op_array[child]->setLevel(lp + 1);
                    }
                    q.push(child);
                }
            }
        }
    }
    for (auto op : DataFlow::op_array)
    {
        if (op.second->getType() == OP_IN)
        {
            int level = 0;
            for (auto child : DataFlow::graph[op.first])
            {
                if (DataFlow::op_array[child]->getLevel() > level)
                {
                    level = DataFlow::op_array[child]->getLevel();
                }
            }
            if (level > 0)
                level = level - 1;
            op.second->setLevel(level);
        }
    }
    for (auto op : DataFlow::op_array)
    {
        if (op.second->getLevel() > DataFlow::num_level)
        {
            DataFlow::num_level = op.second->getLevel();
        }
    }
}

int DataFlow::getId() const
{
    return id;
}

const std::string &DataFlow::getName() const
{
    return name;
}

const std::map<int, std::vector<int>> &DataFlow::getGraph() const
{
    return graph;
}

int DataFlow::getNumOpIn() const
{
    return num_op_in;
}

int DataFlow::getNumOpOut() const
{
    return num_op_out;
}

int DataFlow::getNumOp() const
{
    return num_op;
}

void DataFlow::setId(int id)
{
    DataFlow::id = id;
}

int DataFlow::getNumEdges() const
{
    int num_edges = 0;
    for (const auto &v : DataFlow::graph)
    {
        num_edges += v.second.size();
    }
    return num_edges;
}

int DataFlow::getNumLevel() const
{
    return DataFlow::num_level;
}

std::vector<int> &DataFlow::getInputIds()
{
    return DataFlow::input_op_ids;
}

std::vector<int> &DataFlow::getOutputIds()
{
    return DataFlow::output_op_ids;
}
