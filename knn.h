#ifndef KNN_H
#define KNN_H

#include "data.h"
#include <math.h>


typedef struct {
    float distance;
    int class_label;
} Neighbor;

float euclidean(float *a, float *b, int n); // Dist Eucli entre 2 points 
int knn(Dataset *dataset, Data target, int k); //algo k-NN pour classer un échantillon

#endif
