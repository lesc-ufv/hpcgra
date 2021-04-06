#include <data_flow.h>

DataFlow::DataFlow(int id, std::string name) :
        id(id),
        name(std::move(name)),
        num_op_in(0),
        num_op_out(0),
        num_op(0),
        num_level(0) {}

DataFlow::~DataFlow() {
    for (auto op:op_array) {
        delete op.second;
    }
    DataFlow::op_array.clear();
    for (auto g:DataFlow::graph) {
        g.second.clear();
    }
    DataFlow::graph.clear();
}

void DataFlow::addOperator(Operator *op) {
    if (op->getDataFlowId() == -1) {
        op->setDataFlowId(DataFlow::id);
        DataFlow::op_array[op->getId()] = op;
        DataFlow::num_op++;
        if (op->getType() == OP_IN) {
            DataFlow::num_op_in++;
            DataFlow::input_op_ids.push_back(op->getId());
        }
        if (op->getType() == OP_OUT) {
            DataFlow::num_op_out++;
            DataFlow::output_op_ids.push_back(op->getId());
        }
    }
}

Operator *DataFlow::removeOperator(int op_id) {
    Operator *r = DataFlow::op_array[op_id];
    DataFlow::op_array.erase(op_id);
    r->setDataFlowId(-1);
    return r;
}

void DataFlow::compute() {

    auto n = DataFlow::getNumLevel();
    unsigned int allIsEnd = 0;
    while (allIsEnd != DataFlow::getNumOpIn()) {
        allIsEnd = 0;
        for (int i = 0; i <= n; ++i) {
            for (auto item:DataFlow::getOpArray()) {
                auto op = item.second;
                if (op->getLevel() == i) {
                    op->compute();
                    if (op->getType() == OP_IN && op->getIsEnd()) {
                        allIsEnd++;
                    }
                }
            }
            if (allIsEnd == DataFlow::getNumOpIn()) {
                break;
            }
        }
    }
}

const std::map<int, Operator *> &DataFlow::getOpArray() const {
    return op_array;
}

Operator *DataFlow::getOp(int id) {
    if (DataFlow::op_array.find(id) != DataFlow::op_array.end()) {
        return DataFlow::op_array[id];
    }
    return nullptr;
}

void DataFlow::toDOT(const std::string &fileNamePath) {
    std::ofstream myfile;
    myfile.open(fileNamePath);
    myfile << "digraph " << DataFlow::name << "{" << std::endl;
    for (auto op:DataFlow::op_array) {

        if (op.second->getType() == OP_IN) {
            myfile << " " << op.first << " [ label = input" << op.second->getId() << " ]" << std::endl;
        } else if (op.second->getType() == OP_OUT) {
            myfile << " " << op.first << " [ label = output" << op.second->getId() << " ]" << std::endl;
        } else if (op.second->getType() == OP_IMMEDIATE) {
            myfile << " " << op.first;
            myfile << " [ label = " << op.second->getOpCode() << "i";
            myfile << ", VALUE = " << op.second->getConst()[0][1];
            myfile << "]" << std::endl;
            myfile << " \"" << op.first << "." << op.second->getConst()[0][1] << "\"[ label = " << op.second->getConst()[0][1]
                   << " ]" << std::endl;

        } else {
            myfile << " " << op.first << " [ label = " << op.second->getOpCode() << "]" << std::endl;
        }

    }
    for (auto op:DataFlow::op_array) {
        if (op.second->getType() == OP_IMMEDIATE) {
            myfile << " \"" << op.first << "." << op.second->getConst()[0][1] << "\" -> " << op.first << std::endl;
        }
        for (auto op_dst:op.second->getDst()) {
            myfile << " " << op.first << " -> " << op_dst->getId() << std::endl;
        }
    }
    myfile << "}" << std::endl;
    myfile.close();
}

void DataFlow::toJSON(const std::string &fileNamePath) {
    Json::Value df;
    Json::Value node;
    Json::Value edge;
    int port;
    std::map<std::tuple<int, int, int>, bool> map_port;

    for (auto const item : op_array) {
        auto op = item.second;
        node["id"] =  std::to_string(op->getId());
        node["opcode"] = op->getOpCode();
        node["label"] = op->getLabel();
        if(op->getType() == OP_IMMEDIATE){
            for(auto t : op->getConst()){
                Json::Value c;
                c.append(std::to_string(t[0]));
                c.append(std::to_string(t[1]));
                node["const"].append(c);
            }
        }

        df["nodes"].append(node);
        node.clear();
    }
    for (auto item:DataFlow::op_array) {
        auto op = item.second;
        for (auto neighbor:op->getDst()) {
            port = -1;
            if (neighbor->getSrcA()) {
                if (neighbor->getSrcA()->getId() == op->getId()) {
                    auto key = std::tuple<int, int, int>(op->getId(), neighbor->getId(), 0);
                    if (map_port.find(key) == map_port.end()) {
                        port = 0;
                        map_port[key] = true;
                    }
                }
            }
            if (neighbor->getSrcB() && port == -1) {
                if (neighbor->getSrcB()->getId() == op->getId()) {
                    auto key = std::tuple<int, int, int>(op->getId(), neighbor->getId(), 1);
                    if (map_port.find(key) == map_port.end()) {
                        port = 1;
                        map_port[key] = true;
                    }
                }
            }
            if (neighbor->getBranchIn()) {
                if (neighbor->getBranchIn()->getId() == op->getId()) {
                    port = 2;
                }
            }
            edge["source"] = std::to_string(op->getId());
            edge["target"] = std::to_string(neighbor->getId());
            edge["port"] = std::to_string(port);
            df["edges"].append(edge);
            edge.clear();
        }
    }

    std::ofstream myfile;
    myfile.open(fileNamePath);
    Json::StreamWriterBuilder builder;
    const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(df, &myfile);
}

void DataFlow::connect(Operator *src, Operator *dst, int dstPort) {

    DataFlow::addOperator(src);
    DataFlow::addOperator(dst);
    DataFlow::graph[src->getId()].push_back(dst->getId());

    src->getDst().push_back(dst);

    if (dstPort == 0) {
        dst->setSrcA(src);
    } else if (dstPort == 1) {
        dst->setSrcB(src);
    } else if (dstPort == 2) {
        dst->setBranchIn(src);
    }
    DataFlow::updateOpLevel();
}

void DataFlow::updateOpLevel() {
    std::queue<int> q;
    int parent;
    for (auto op:DataFlow::op_array) {
        if (op.second->getType() == OP_IN) {
            q.push(op.first);
            while (!q.empty()) {
                parent = q.front();
                q.pop();
                for (auto child:DataFlow::graph[parent]) {
                    int lp = DataFlow::op_array[parent]->getLevel();
                    int lc = DataFlow::op_array[child]->getLevel();
                    if (lp >= lc) {
                        DataFlow::op_array[child]->setLevel(lp + 1);
                    }
                    q.push(child);
                }
            }
        }
    }
    for (auto op:DataFlow::op_array) {
        if (op.second->getType() == OP_IN) {
            int level = 0;
            for (auto child:DataFlow::graph[op.first]) {
                if (DataFlow::op_array[child]->getLevel() > level) {
                    level = DataFlow::op_array[child]->getLevel();
                }
            }
            if (level > 0)
                level = level - 1;
            op.second->setLevel(level);
        }
    }
    for (auto op:DataFlow::op_array) {
        if (op.second->getLevel() > DataFlow::num_level) {
            DataFlow::num_level = op.second->getLevel();
        }
    }
}


int DataFlow::getId() const {
    return id;
}

const std::string &DataFlow::getName() const {
    return name;
}

const std::map<int, std::vector<int>> &DataFlow::getGraph() const {
    return graph;
}

int DataFlow::getNumOpIn() const {
    return num_op_in;
}

int DataFlow::getNumOpOut() const {
    return num_op_out;
}

int DataFlow::getNumOp() const {
    return num_op;
}

void DataFlow::setId(int id) {
    DataFlow::id = id;
}

int DataFlow::getNumEdges() const {
    int num_edges = 0;
    for (const auto &v:DataFlow::graph) {
        num_edges += v.second.size();
    }
    return num_edges;
}

int DataFlow::getNumLevel() const {
    return DataFlow::num_level;
}

std::vector<int> &DataFlow::getInputIds() {
    return DataFlow::input_op_ids;
}

std::vector<int> &DataFlow::getOutputIds() {
    return DataFlow::output_op_ids;
}
