#include "knn.h"

// Calcule la distance Euclidienne entre deux points
float euclidean(float *a, float *b, int n) {
    float sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return sqrt(sum);
}

// Initialise les voisins en calculant la distance Euclidienne de chaque échantillon
void init_neighbors(Neighbor *neighbors, Dataset *dataset, Data target) {
    for (int i = 0; i < dataset->data_count; i++) {
        neighbors[i].distance = euclidean(dataset->data[i].features, target.features, target.feature_count);
        neighbors[i].class_label = dataset->data[i].class;
    }
}

// trie les voisins selon distance
int compare_neighbors(const void *n1, const void *n2) {
    Neighbor *neighbor1 = (Neighbor *)n1;
    Neighbor *neighbor2 = (Neighbor *)n2;
    return (neighbor1->distance > neighbor2->distance) - (neighbor1->distance < neighbor2->distance);
}

//vote majoritaire parmi les k voisins les plus proches pour obtenir la classe
int majority_vote(Neighbor *neighbors, int k) {
    int votes[MAX_CLASSES] = {0};
    for (int i = 0; i < k; i++) {
        votes[neighbors[i].class_label - 1]++; 
    }

    int max = 0, class = -1;
    for (int i = 0; i < MAX_CLASSES; i++) {
        if (votes[i] > max) {
            max = votes[i];
            class = i + 1;
        }
    }
    return class; // Retourne la classe ayant le plus de votes
}


int knn(Dataset *dataset, Data target, int k) {
    Neighbor *neighbors = malloc(dataset->data_count * sizeof(Neighbor)); 
    if (!neighbors) {
        fprintf(stderr, "Erreur : mémoire insuffisante.\n");
        return -1;
    }

    init_neighbors(neighbors, dataset, target); // Initialise les voisins
    qsort(neighbors, dataset->data_count, sizeof(Neighbor), compare_neighbors); // Trie les voisins par distance

    //vote majoritaire pour obtenir la classe prédite
    int class = majority_vote(neighbors, k);

    free(neighbors); 
    return class;
}