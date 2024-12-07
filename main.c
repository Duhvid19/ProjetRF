#include "data.h"
#include "knn.h"
#include "kmeans.h"
#include "test.h"

int main(){
    const char *data_directory[] = {"./=SharvitB2/=Signatures/=ART", "./=SharvitB2/=Signatures/=GFD", "./=SharvitB2/=Signatures/=Yang", "./=SharvitB2/=Signatures/=Zernike7"};

    for (int i = 0; i < 4; i++){
        Dataset *dataset = load_dataset(data_directory[i]);
        //normalize_dataset(dataset);
        if (!dataset){
            fprintf(stderr, "Erreur : Impossible de charger le jeu de donnees.\n");
            return EXIT_FAILURE;
        }
        const char *name = strrchr(data_directory[i], '=');
        printf("\nMethode : %s\n", name + 1);

        printf("=== KMEANS ===\n");
        int k_clusters = 12;
        int *clusters = malloc(dataset->data_count * sizeof(int));
        kmeans(dataset, k_clusters, 100, clusters);
        printf("Indice de silhouette : %f\n", silhouette_score(dataset, clusters, k_clusters));

        printf("=== KNN ===\n");
        int k_neighbors = 12;
        int **confusion_matrix = init_conf_matrix();
        float accuracy_knn = cross_validate(dataset, k_neighbors, dataset->data_count, confusion_matrix);
        print_metrics(confusion_matrix);
        printf("Accuracy : %.2f\n", accuracy_knn);

        free(clusters);
        free_dataset(dataset);
    }

    return EXIT_SUCCESS;
}
