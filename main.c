#include "data.h"
#include "knn.h"
#include "kmeans.h"
#include "tools/evaluation.h"

int main() {
    const char *dataDirectory = "./=SharvitB2/=Signatures/=Yang";
    Dataset *dataset = load_dataset(dataDirectory);

    if (!dataset) {
        fprintf(stderr, "Erreur : Impossible de charger le jeu de données.\n");
        return EXIT_FAILURE;
    }

    printf("Jeu de données chargé avec %d descripteurs.\n", dataset->data_count);

    // Test de k-means
    int k_clusters = MAX_CLASSES;
    int *labels = malloc(dataset->data_count * sizeof(int));
    if (!labels) {
        fprintf(stderr, "Erreur : Mémoire insuffisante pour les labels.\n");
        free_dataset(dataset);
        return EXIT_FAILURE;
    }

    kmeans(dataset, k_clusters, 100, labels);
    printf("\nRésultat du clustering (k-means):\n");
    for (int i = 0; i < dataset->data_count; i++) {
        printf("Descripteur %d assigné au cluster %d, classe réelle : %d\n", i, labels[i], dataset->data[i].class);
    }

    // Test de k-NN
    int k_neighbors = 3;
    float accuracy = evaluate_knn(dataset, k_neighbors);
    printf("\nPrécision du classifieur k-NN avec k=%d : %.2f%%\n", k_neighbors, accuracy * 100);

    free(labels);
    free_dataset(dataset);

    return EXIT_SUCCESS;
}
