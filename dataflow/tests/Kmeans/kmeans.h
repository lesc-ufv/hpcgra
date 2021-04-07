#ifndef KMEANS_KMEANS_H
#define KMEANS_KMEANS_H

#include <data_flow.h>
#include <algorithm>

using namespace std;

DataFlow *createDataFlow(int id, int num_clusters, int num_dim);

bool compare(Operator *a, Operator *b);

int main(int argc, char *argv[]);



#endif //KMEANS_KMEANS_H

