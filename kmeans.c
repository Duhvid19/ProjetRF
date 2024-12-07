#include "kmeans.h"
#include <time.h> 

float **init_centroids(int k, Dataset *dataset){
    float **centroids = malloc(k * sizeof(float *));
    for (int i = 0; i < k; i++){
        centroids[i] = malloc(dataset->feature_count * sizeof(float));
    }

    srand(time(NULL)); // techniquement pas obligatoire
    int *selected_indices = calloc(dataset->data_count,  sizeof(int));

    for (int i = 0; i < k; i++){

        // on choisit un index aleatoire et verifie que ce n'est pas un doublon
        int index;
        do{
            index = rand() % dataset->data_count;
        } while (selected_indices[index] != 0);
        selected_indices[index] = 1;

        for (int j = 0; j < dataset->feature_count; j++){
            centroids[i][j] = dataset->data[index].features[j];
        }
    }

    free(selected_indices);
    return centroids;
}


void assign_clusters(Dataset *dataset, float **centroids, int k, int *clusters){
    for (int i = 0; i < dataset->data_count; i++){
        float min_dist = INFINITY;
        int closest_cluster = -1;

        // obtient le cluster le plus proche pour chaque data
        for (int j = 0; j < k; j++){
            float dist = euclidean(dataset->data[i].features, centroids[j], dataset->feature_count);
            if (dist < min_dist){
                min_dist = dist;
                closest_cluster = j;
            }
        }
        clusters[i] = closest_cluster;
    }
}

void update_centroids(Dataset *dataset, float **centroids, int k, int *clusters){
    int *cluster_counts = calloc(k, sizeof(int)); // nombre de data dans les clusters
    float **new_centroids = malloc(k * sizeof(float *));
    for (int i = 0; i < k; i++){
        new_centroids[i] = calloc(dataset->feature_count, sizeof(float));
    }

    for (int i = 0; i < dataset->data_count; i++){
        int cluster = clusters[i];
        cluster_counts[cluster]++; 
        for (int j = 0; j < dataset->feature_count; j++){
            new_centroids[cluster][j] += dataset->data[i].features[j]; 
        }
    }

    for (int i = 0; i < k; i++){
        if (cluster_counts[i] > 0){
            for (int j = 0; j < dataset->feature_count; j++){
                centroids[i][j] = new_centroids[i][j]/cluster_counts[i];
            }
        }
    }

    for (int i = 0; i < k; i++){
        free(new_centroids[i]);
    }
    free(new_centroids);
    free(cluster_counts);
}


void kmeans(Dataset *dataset, int k, int max_iterations, int *clusters){

    // initialisation des centroides
    float **centroids = init_centroids(k, dataset);

    // assignation des clusters + mise a jour des centroides
    for (int i = 0; i < max_iterations; i++){
        assign_clusters(dataset, centroids, k, clusters);
        update_centroids(dataset, centroids, k, clusters);
    }

    for (int i = 0; i < k; i++){
        free(centroids[i]);
    }
    free(centroids);
}
