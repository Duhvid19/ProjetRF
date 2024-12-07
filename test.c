#include "test.h"
#include "knn.h"

int **init_conf_matrix(){
    int **confusion_matrix = malloc(MAX_CLASSES * sizeof(int *));
    for (int i = 0; i < MAX_CLASSES; i++){
        confusion_matrix[i] = calloc(MAX_CLASSES, sizeof(int));
    }
    return confusion_matrix;
}

void free_conf_matrix(int **confusion_matrix){
    for (int i = 0; i < MAX_CLASSES; i++){
        free(confusion_matrix[i]);
    }
    free(confusion_matrix);
}

void print_metrics(int **confusion_matrix){
    int tp[MAX_CLASSES] = {0}; // true positive
    int fp[MAX_CLASSES] = {0}; // false positive
    int fn[MAX_CLASSES] = {0}; // false negative
    int total = 0; // total de classe
    float precision = 0.0;
    float recall = 0.0;
    float f_measure = 0.0;
    for (int i = 0; i < MAX_CLASSES; i++){
        if (confusion_matrix[i] != 0){
            total++;
        }
        for (int j = 0; j < MAX_CLASSES; j++){
            if (i == j){
                tp[i] += confusion_matrix[i][j];
            } 
            else{
                fp[j] += confusion_matrix[i][j];
                fn[i] += confusion_matrix[i][j];
            }
        }
    }

    // calcul de precison, rappel et f-mesure
    for (int i = 0; i < MAX_CLASSES; i++){
        float tmp_precision = 0.0;
        float tmp_recall = 0.0;
        float tmp_f = 0.0;
        //printf("Classe : %d - ", i);
        if (tp[i] + fp[i] > 0){
            tmp_precision = (float)tp[i]/(tp[i] + fp[i]);
            precision += tmp_precision;
            //printf("Precision : %.2f - ", tmp_precision);
        }
        if (tp[i] + fn[i] > 0){
            tmp_recall += (float)tp[i]/(tp[i] + fn[i]);
            recall += tmp_recall;
            //printf("Rappel : %.2f - ", tmp_recall); 
        }
        if (tmp_precision + tmp_recall > 0){
            tmp_f = 2 * (tmp_precision * tmp_recall)/(tmp_precision + tmp_recall);
            f_measure += tmp_f;
            //printf("F-mesure : %.2f\n", tmp_f); 
        }
    }
    printf("Precision : %.2f, Rappel : %.2f, F-mesure : %.2f\n", precision/total, recall/total, f_measure/total);
    free_conf_matrix(confusion_matrix);
}

float cross_validate(Dataset *dataset, int k_neighbors, int k_folds, int **confusion_matrix){
    int fold_size = dataset->data_count/k_folds;
    float accuracy_sum = 0.0;

    for (int i = 0; i < k_folds; i++){

        // on creer des ensembles d'entrainement et de test
        Dataset train_set, test_set;
        train_set.data_count = dataset->data_count - fold_size;
        test_set.data_count = fold_size;

        train_set.data = malloc(train_set.data_count * sizeof(Data));
        test_set.data = malloc(test_set.data_count * sizeof(Data));
        train_set.feature_count = dataset->feature_count;

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
            confusion_matrix[target.class-1][predicted-1]++;
            if (target.class == predicted){
                correct++;
            }
        }

        accuracy_sum += (float)correct/test_set.data_count;

        free(train_set.data);
        free(test_set.data);
    }

    return accuracy_sum/k_folds;
}

float silhouette_score(Dataset *dataset, int *clusters, int k){
    float score = 0.0;

    for (int i = 0; i < dataset->data_count; i++){
        int cluster_i = clusters[i];

        // calcul de a(i) : distance moyenne intra cluster
        float a_i = 0.0;
        int intra_count = 0;
        for (int j = 0; j < dataset->data_count; j++){
            if (i != j && clusters[j] == cluster_i){
                a_i += euclidean(dataset->data[i].features, dataset->data[j].features, dataset->feature_count);
                intra_count++;
            }
        }
        if (intra_count == 0){
            continue;
        }
        a_i /= intra_count;

        // calcul de b(i) : distance moyenne au cluster voisin le plus proche
        float b_i = INFINITY;
        for (int c = 0; c < k; c++){
            if (c != cluster_i){
                float cluster_dist = 0.0;
                int inter_count = 0;
                for (int j = 0; j < dataset->data_count; j++){
                    if (clusters[j] == c){
                        cluster_dist += euclidean(dataset->data[i].features, dataset->data[j].features, dataset->feature_count);
                        inter_count++;
                    }
                }
                cluster_dist /= inter_count;
                if (cluster_dist < b_i){
                    b_i = cluster_dist;
                }
            }
        }

        // calcul de s(i)
        float s_i = 0.0;
        if (a_i != 0 && b_i != 0){
            s_i = (b_i - a_i)/fmax(a_i, b_i);
        }
        score += s_i;
    }

    return score/dataset->data_count;
}