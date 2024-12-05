#ifndef KMEANS_H
#define KMEANS_H

#include "data.h"
#include "knn.h" // Necessaire pour euclidean()

void kmeans(Dataset *dataset, int k, int max_iterations, int *labels); // Effectue le clustering K-means

#endif
