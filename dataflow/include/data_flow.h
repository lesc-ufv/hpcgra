#ifndef CGRASCHEDULER_DATAFLOW_H
#define CGRASCHEDULER_DATAFLOW_H

#include <queue>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <json/json.h>
#include <data_flow_defs.h>
#include <operator.h>
#include <operator_factory.h>
#include <params.h>

class DataFlow {

private:
    int id;
    std::string name;
    std::map<int, Operator *> op_array;
    std::map<int, std::vector<int>> graph;
    std::vector<int> input_op_ids;
    std::vector<int> output_op_ids;    
    int num_op_in;
    int num_op_out;
    int num_op;
    int num_level;

    void addOperator(Operator *op);

public:

    explicit DataFlow(int id, std::string name);

    ~DataFlow();

    Operator *removeOperator(int op_id);

    void compute();

    const std::map<int, Operator *> &getOpArray() const;

    Operator *getOp(int id);

    void toDOT(const std::string& fileNamePath);

    void toJSON(const std::string &fileNamePath);

    void toJsonOperator(const std::string &fileNamePath);

    void connect(Operator *src, int srcPort, Operator *dst, int dstPort);
    
    void disconnect(Operator *src, int srcPort, Operator *dst, int dstPort);
    
    void updateOpLevel();

    int getId() const;

    void setId(int id);

    const std::string &getName() const;

    const std::map<int, std::vector<int>> &getGraph() const;

    int getNumOpIn() const;

    int getNumOpOut() const;

    int getNumOp() const;

    int getNumEdges() const;

    int getNumLevel() const;
    
    std::vector<int> &getInputIds();
    
    std::vector<int> &getOutputIds();
    
};


#endif //CGRASCHEDULER_DATAFLOW_H
