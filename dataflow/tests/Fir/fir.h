//
// Created by lucas on 22/05/19.
//

#ifndef FIR_FIR_H
#define FIR_FIR_H

#include <data_flow.h>

using namespace std;

DataFlow *createDataFlow(int id, int copies, unsigned short *coef, int taps);

int main(int argc, char *argv[]);

#endif //FIR_FIR_H
