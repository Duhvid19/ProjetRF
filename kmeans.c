#include "kmeans.h"
#include <time.h> 

void init_centroids(float centroids[][MAX_FEATURES], int k, Dataset *dataset){

    srand(time(NULL)); // techniquement pas obligatoire

    // on initialise un tableau d'indice à -1
    int selected_indices[k];
    memset(selected_indices, -1, k * sizeof(int));

    for (int i = 0; i < k; i++){

        // on choisit un index aleatoire et verifie que ce n'est pas un doublon
        int index;
        do{
            index = rand() % dataset->data_count;
        } while (selected_indices[index] != -1);
        selected_indices[index] = 1;

        memcpy(centroids[i], dataset->data[index].features, dataset->data[index].feature_count * sizeof(float));
    }
}


int assign_clusters(Dataset *dataset, float centroids[][MAX_FEATURES], int k, int *labels){
    int changes = 0;
    for (int i = 0; i < dataset->data_count; i++){
        float min_dist = INFINITY;
        int closest_cluster = -1;

        for (int j = 0; j < k; j++){
            float dist = euclidean(dataset->data[i].features, centroids[j], dataset->data[i].feature_count);
            if (dist < min_dist){
                min_dist = dist;
                closest_cluster = j;
            }
        }

        if (labels[i] != closest_cluster){
            labels[i] = closest_cluster;
            changes++; // si changement, alors incremente
        }
    }
    return changes; 
}

void update_centroids(Dataset *dataset, float centroids[][MAX_FEATURES], int k, int *labels){
    int cluster_counts[k];
    memset(cluster_counts, 0, sizeof(cluster_counts)); 
    memset(centroids, 0, k * MAX_FEATURES * sizeof(float)); 

    for (int i = 0; i < dataset->data_count; i++){
        int cluster = labels[i];
        for (int j = 0; j < dataset->data[i].feature_count; j++){
            centroids[cluster][j] += dataset->data[i].features[j]; 
        }
        cluster_counts[cluster]++; 
    }

    for (int i = 0; i < k; i++){
        if (cluster_counts[i] > 0){
            for (int j = 0; j < MAX_FEATURES; j++){
                centroids[i][j] /= cluster_counts[i];
            }
        }
    }
}


void kmeans(Dataset *dataset, int k, int max_iterations, int *labels){
    float centroids[k][MAX_FEATURES];
    init_centroids(centroids, k, dataset); 

    int iterations = 0;
    int changes;
    do{
        changes = assign_clusters(dataset, centroids, k, labels);
        update_centroids(dataset, centroids, k, labels); 
        iterations++; 
    } while (changes > 0 && iterations < max_iterations); // repete jusqu'à stabilisation des clusters

    printf("k-means termine en %d iterations.\n", iterations); 
}
