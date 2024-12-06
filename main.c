#include "data.h"
#include "knn.h"
#include "kmeans.h"
#include "test.h"

int main(){
    const char *dataDirectory = "./=SharvitB2/=Signatures/=GFD";
    Dataset *dataset = load_dataset(dataDirectory);

    if (!dataset){
        fprintf(stderr, "Erreur : Impossible de charger le jeu de donnees.\n");
        return EXIT_FAILURE;
    }

    printf("Jeu de donnees charge avec %d donnees.\n", dataset->data_count);

    // Test de k-means
    int k_clusters = MAX_CLASSES;
    int *labels = malloc(dataset->data_count * sizeof(int)); // on en fait quoi ???
    if (!labels){
        fprintf(stderr, "Erreur : Memoire insuffisante pour les labels.\n");
        free_dataset(dataset);
        return EXIT_FAILURE;
    }

    kmeans(dataset, k_clusters, 100, labels);

    // Test de k-NN
    float max_accuracy = 0.0;
    int index_neighbor, index_fold;
    for (int i = 1; i < 11; i++){
        for (int j = 2; j < 11; j++){
            float accuracy = cross_validate(dataset, i, j);
            printf("Precision de la validation croisee du classifieur k-NN avec k=%d et fold=%d: %.2f%%\n", i, j, accuracy * 100);
            if (accuracy > max_accuracy){
                max_accuracy = accuracy;
                index_neighbor = i;
                index_fold = j;
            }
        }
    }
    printf("max accuracy : %.2f%%, best neighbor : %d, best fold : %d\n", max_accuracy * 100, index_neighbor, index_fold);

    free(labels);
    free_dataset(dataset);

    return EXIT_SUCCESS;
}
