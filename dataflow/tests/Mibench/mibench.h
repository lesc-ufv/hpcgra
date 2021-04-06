#ifndef MIBENCH_MIBENCH_H
#define MIBENCH_MIBENCH_H

#include <data_flow.h>

using namespace std;

DataFlow *createDataFlow(int id, int copies);

int main(int argc, char *argv[]);

#endif //MIBENCH_MIBENCH_H
