#ifndef CGRASCHEDULER_OPERATOR_H
#define CGRASCHEDULER_OPERATOR_H

#include <string>
#include <unordered_map>
#include <operator.h>
#include <params.h>
#include <abs.h>
#include <add.h>
#include <and.h>
#include <seq.h>
#include <sne.h>
#include <input_stream.h>
#include <max.h>
#include <min.h>
#include <mul.h>
#include <mux.h>
#include <not.h>
#include <or.h>
#include <output_stream.h>
#include <sgt.h>
#include <shl.h>
#include <shr.h>
#include <slt.h>
#include <sub.h>
#include <xor.h>
#include <const.h>
#include <reg.h>

typedef Operator *(*pfnCreate_t)(Params); // function pointer type

class OperatorFactory {
private:
    OperatorFactory();

    OperatorFactory(const OperatorFactory &) {}

    OperatorFactory &operator=(const OperatorFactory &) { return *this; }

    typedef std::unordered_map<std::string, pfnCreate_t> FactoryMap;
    FactoryMap m_FactoryMap;

public:
    ~OperatorFactory() { m_FactoryMap.clear(); }

    static OperatorFactory *Get() {
        static OperatorFactory instance;
        return &instance;
    }

    void Register(const std::string &operatorName, pfnCreate_t pfnCreate);

    Operator *CreateOperator(const std::string &operatorName, Params &params);
};

#endif //CGRASCHEDULER_OPERATOR_H
