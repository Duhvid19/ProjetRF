#ifndef KMEANS_H
#define KMEANS_H

#include "data.h"
#include "knn.h" // necessaire pour euclidean()

void init_centroids(float centroids[][MAX_FEATURES], int k, Dataset *dataset);
int assign_clusters(Dataset *dataset, float centroids[][MAX_FEATURES], int k, int *labels);
void update_centroids(Dataset *dataset, float centroids[][MAX_FEATURES], int k, int *labels);
void kmeans(Dataset *dataset, int k, int max_iterations, int *labels);

#endif
