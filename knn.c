#include "headers.h"

float euclidean(float *a, float *b, int n){
    float sum = 0.0;
    for (int i = 0; i < n; i++){
        sum += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return sqrt(sum);
}

void init_neighbors(Neighbor *neighbors, Dataset *dataset, Data target){
    for (int i = 0; i < dataset->data_count; i++){
        neighbors[i].distance = euclidean(dataset->data[i].features, target.features, target.feature_count);
        neighbors[i].class_label = dataset->data[i].class;
    }
}

int compare_neighbors(const void *n1, const void *n2){
    Neighbor *neighbor1 = (Neighbor *)n1;
    Neighbor *neighbor2 = (Neighbor *)n2;
    return (neighbor1->distance > neighbor2->distance) - (neighbor1->distance < neighbor2->distance);
}

int majority_vote(Neighbor *neighbors, int k){
    int votes[MAX_CLASSES] = {0};
    for (int i = 0; i < k; i++){
        votes[neighbors[i].class_label - 1]++;
    }

    int max = 0, class = -1;
    for (int i = 0; i < MAX_CLASSES; i++){
        if (votes[i] > max){
            max = votes[i];
            class = i + 1;
        }
    }
    return class;
}

int knn(Dataset *dataset, Data target, int k){
    Neighbor *neighbors = malloc(dataset->data_count * sizeof(Neighbor));
    if (!neighbors){
        fprintf(stderr, "Erreur : mémoire insuffisante.\n");
        return -1;
    }

    // initialisation des voisins
    init_neighbors(neighbors, dataset, target);

    // trie des voisins selon la distance
    qsort(neighbors, dataset->data_count, sizeof(Neighbor), compare_neighbors);

    // vote majoritaire pour obtenir la classe
    int class = majority_vote(neighbors, k);

    free(neighbors);
    return class;
}


void init_centroids(float centroids[][MAX_FEATURES], int k, Dataset *dataset){

    srand(time(NULL)); // techniquement pas obligatoire

    // on initialise un tableau d'indice à -1
    int selected_indices[k];
    memset(selected_indices, -1, k * sizeof(int));

    for (int i = 0; i < k; i++){

        // on choisit un index aleatoire et verifie que ce n'est pas un doublon
        int index;
        do {
            index = rand() % dataset->data_count;
        } while (selected_indices[index] != -1);
        selected_indices[index] = 1;

        memcpy(centroids[i], dataset->data[index].features, dataset->data[index].feature_count * sizeof(float));
    }
}

int assign_clusters(Dataset *dataset, float centroids[][MAX_FEATURES], int k, int *labels){
    int changes = 0;

    for (int i = 0; i < dataset->data_count; i++){
        float min = INFINITY;
        int closest_cluster = -1;

        for (int j = 0; j < k; j++){
            float dist = euclidean(dataset->data[i].features, centroids[j], dataset->data[i].feature_count);
            if (dist < min){
                min = dist;
                closest_cluster = j;
            }
        }

        if (labels[i] != closest_cluster){
            labels[i] = closest_cluster;
            changes++;
        }
    }
    return changes;
}

void update_centroids(Dataset *dataset, float centroids[][MAX_FEATURES], int k, int *labels){
    int clusterCounts[k];
    memset(clusterCounts, 0, sizeof(clusterCounts));
    memset(centroids, 0, k * MAX_FEATURES * sizeof(float));

    for (int i = 0; i < dataset->data_count; i++){
        int cluster = labels[i];

        for (int j = 0; j < dataset->data[i].feature_count; j++){
            centroids[cluster][j] += dataset->data[i].features[j];
        }
        clusterCounts[cluster]++;
    }

    for (int i = 0; i < k; i++){
        if (clusterCounts[i] > 0){
            for (int j = 0; j < MAX_FEATURES; j++){
                centroids[i][j] /= clusterCounts[i];
            }
        }
    }
}

void kmeans(Dataset *dataset, int k, int max_iterations, int *labels){
    float centroids[k][MAX_FEATURES];
    init_centroids(centroids, k, dataset);

    int iterations = 0;
    int changes;

    do {
        changes = assign_clusters(dataset, centroids, k, labels);
        update_centroids(dataset, centroids, k, labels);
        iterations++;
    } while (changes > 0 && iterations < max_iterations);

    printf("k-means terminé en %d itérations.\n", iterations);
}

float evaluate(Dataset *dataset, int k){
    int correct = 0;
    for (int i = 0; i < dataset->data_count; i++){
        Data target = dataset->data[i];
        int predicted = knn(dataset, target, k);
        if (predicted == target.class){
            correct++;
        }
    }
    return (float)correct / dataset->data_count;
}

int main(){
    const char *dataDirectory = "./=SharvitB2/=Signatures/=Yang";
    Dataset *dataset = load_dataset(dataDirectory);

    if (!dataset){
        fprintf(stderr, "Erreur : Impossible de charger le jeu de données.\n");
        return EXIT_FAILURE;
    }

    printf("Jeu de données chargé avec %d descripteurs.\n", dataset->data_count);

    // Partie 2b : k-means
    int k_clusters = MAX_CLASSES; 
    int *labels = malloc(dataset->data_count * sizeof(int));
    if (!labels){
        fprintf(stderr, "Erreur : Mémoire insuffisante pour les labels.\n");
        free_dataset(dataset);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < dataset->data_count; i++){
        labels[i] = dataset->data[i].class;
    }

    kmeans(dataset, k_clusters, 100, labels);

    for (int i = 0; i < dataset->data_count; i++){
        printf("Descripteur %d assigné au cluster %d, classe réelle : %d\n", i, labels[i], dataset->data[i].class);
    }

    // Partie 2a : k-NN
    int k_neighbors = 1; 
    float accuracy = evaluate(dataset, k_neighbors);
    printf("Précision du classifieur k-NN avec k=%d : %.2f%%\n", k_neighbors, accuracy * 100);

    free(labels);
    free_dataset(dataset);
    return EXIT_SUCCESS;
}

