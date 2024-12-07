#ifndef TEST_H
#define TEST_H

#include "data.h"

int **init_conf_matrix();
void free_conf_matrix(int **confusion_matrix);
void print_metrics(int **confusion_matrix);
float cross_validate(Dataset *dataset, int k_neighbors, int k_folds, int **confusion_matrix);
float silhouette_score(Dataset *dataset, int *clusters, int k);

#endif