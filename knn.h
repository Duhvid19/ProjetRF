#ifndef KNN_H
#define KNN_H

#include "data.h"
#include <math.h>


typedef struct{
    float distance;
    int class_label;
} Neighbor;

float euclidean(float *a, float *b, int n);
void init_neighbors(Neighbor *neighbors, Dataset *dataset, Data target);
int majority_vote(Neighbor *neighbors, int k);
int compare_neighbors(const void *n1, const void *n2);
int knn(Dataset *dataset, Data target, int k);

#endif
