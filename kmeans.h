#ifndef KMEANS_H
#define KMEANS_H

#include "data.h"
#include "knn.h" // necessaire pour euclidean()

float **init_centroids(int k, Dataset *dataset);
void assign_clusters(Dataset *dataset, float **centroids, int k, int *clusters);
void update_centroids(Dataset *dataset, float **centroids, int k, int *clusters);
void kmeans(Dataset *dataset, int k, int max_iterations, int *clusters);

#endif
