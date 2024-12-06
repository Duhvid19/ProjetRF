#include "test.h"
#include "knn.h"

float cross_validate(Dataset *dataset, int k_neighbors, int k_folds){
    int fold_size = dataset->data_count/k_folds;
    float accuracy_sum = 0.0;

    for (int i = 0; i < k_folds; i++){

        // on creer des ensembles d'entrainement et de test
        Dataset train_set, test_set;
        train_set.data_count = dataset->data_count - fold_size;
        test_set.data_count = fold_size;

        train_set.data = malloc(train_set.data_count * sizeof(Data));
        test_set.data = malloc(test_set.data_count * sizeof(Data));

        int train_index = 0;
        int test_index = 0;
        for (int j = 0; j < dataset->data_count; j++){
            if (j >= i * fold_size && j < (i + 1) * fold_size){
                test_set.data[test_index++] = dataset->data[j];
            }
            else{
                train_set.data[train_index++] = dataset->data[j];
            }
        }

        // on evalue
        int correct = 0;
        for (int i = 0; i < test_set.data_count; i++){
            Data target = test_set.data[i];
            int predicted = knn(&train_set, target, k_neighbors);
            if (predicted == target.class){
                correct++;
            }
        }
        accuracy_sum += (float)correct/test_set.data_count;
        free(train_set.data);
        free(test_set.data);
    }

    return accuracy_sum/k_folds;
}