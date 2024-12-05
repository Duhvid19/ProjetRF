#ifndef EVALUATION_H
#define EVALUATION_H

#include "data.h"
#include "knn.h" // Inclut pour utiliser knn()

float evaluate_knn(Dataset *dataset, int k); // Évalue la performance du classifieur k-NN

#endif
