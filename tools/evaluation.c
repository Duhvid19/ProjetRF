#include "evaluation.h"

// Évalue la performance du classifieur k-NN en calculant la précision
float evaluate_knn(Dataset *dataset, int k) {
    int correct = 0;
    for (int i = 0; i < dataset->data_count; i++) {
        Data target = dataset->data[i]; // Récupère un échantillon
        int predicted = knn(dataset, target, k); // Prédiction du classifieur k-NN
        if (predicted == target.class) { 
            correct++; 
        }
    }
    return (float)correct / dataset->data_count;
}
