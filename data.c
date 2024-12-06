#include "data.h"

Dataset *load_dataset(const char *directory){
    DIR *dir = opendir(directory);

    if (!dir){
        printf("Erreur : Impossible d'ouvrir le repertoire %s\n", directory);
        return NULL;
    }

    Dataset *dataset = (Dataset *)malloc(sizeof(Dataset));
    if (!dataset){
        printf("Erreur : Memoire insuffisante pour le Dataset\n");
        closedir(dir);
        return NULL;
    }

    // initialisation du dataset
    dataset->data_count = 0;
    dataset->data = NULL; 

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL){
        // verifie si le nom du fichier est ".DS_Store", ".", ou "..", si c'est le cas on ignore
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".DS_Store") == 0){
            continue;
        }

        char filePath[256]; 
        snprintf(filePath, sizeof(filePath), "%s/%s", directory, entry->d_name); 
        //printf("fichier lu : %s\n", entry->d_name);
        FILE *file = fopen(filePath, "r"); 
        if (!file){
            fprintf(stderr, "Erreur : Impossible de lire le fichier %s\n", filePath);
            continue;
        }

         // verifie si la classe est entre 1 et 10, si c'est pas le cas on ignore
        int class_num;
        sscanf(entry->d_name, "s%d", &class_num);
        if (class_num < 1 || class_num > MAX_CLASSES){
            fclose(file);
            continue;
        }

        // creation du data
        Data data;
        sscanf(entry->d_name, "s%dn%d", &data.class, &data.sample); // extrait la classe et l'echantillon
        data.feature_count = 0;
        while (fscanf(file, "%f", &data.features[data.feature_count]) == 1){ // lit les caracteristiques du fichier
            data.feature_count++;
        }
        fclose(file);

        // ajout du descripteur au tableau
        dataset->data = (Data *)realloc(dataset->data, (dataset->data_count + 1) * sizeof(Data)); //redimension 
        dataset->data[dataset->data_count++] = data;
    }

    closedir(dir); 
    return dataset; 
}


void free_dataset(Dataset *dataset){
    if (dataset){
        free(dataset->data); 
        free(dataset); 
    }
}

void print_dataset(const Dataset *dataset){
    for (int i = 0; i < dataset->data_count; i++){
        Data desc = dataset->data[i];
        printf("Classe: %d, Echantillon: %d, Caracteristiques: ", desc.class, desc.sample);
        for (int j = 0; j < desc.feature_count; j++){
            printf("%.2f ", desc.features[j]); // affiche les caracteristiques
        }
        printf("\n");
    }
}
