#ifndef HEADERS_H
#define HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <dirent.h>
#include <time.h>

#define MAX_FEATURES 50
#define MAX_CLASSES 10

typedef struct{
    int class;
    int sample; //possiblement inutile
    float features[MAX_FEATURES];
    int feature_count;
} Data;

typedef struct{
    Data *data;
    int data_count;
} Dataset;

typedef struct{
    float distance;
    int class_label;
} Neighbor;

Dataset *load_dataset(const char *directory);
void free_dataset(Dataset *dataset);
void print_dataset(const Dataset *dataset);

float euclidean(float *a, float *b, int n);
float manhattan(float *a, float *b, int n);

void init_neighbors(Neighbor *neighbors, Dataset *dataset, Data target);
int compare_neighbors(const void *n1, const void *n2);
int majority_vote(Neighbor *neighbors, int k);
int knn(Dataset *dataset, Data target, int k);

void init_centroids(float centroids[][MAX_FEATURES], int k, Dataset *dataset);
int assign_clusters(Dataset *dataset, float centroids[][MAX_FEATURES], int k, int *labels);
void update_centroids(Dataset *dataset, float centroids[][MAX_FEATURES], int k, int *labels);
void kmeans(Dataset *dataset, int k, int maxIterations, int *labels);

#endif
