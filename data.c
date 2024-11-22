#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_FEATURES 50

typedef struct{
    int class;
    int sample;
    float features[MAX_FEATURES];
    int featureCount;
} ShapeDescriptor;

typedef struct{
    ShapeDescriptor *descriptors;
    int n_descriptors;
} DataSet;


DataSet *readDataSet(const char *directory){
    DIR *dir = opendir(directory);

    if (!dir){
        printf("Erreur : Impossible d'ouvrir le repertoire %s\n", directory);
        return NULL;
    }

    DataSet *dataSet = (DataSet *)malloc(sizeof(DataSet));
    if (!dataSet){
        printf("Erreur : Memoire insuffisante pour le DataSet\n");
        closedir(dir);
        return NULL;
    }

    dataSet->n_descriptors = 0;
    dataSet->descriptors = NULL;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL){

        // Verifie si le nom du fichier est ".DS_Store", ".", ou "..", si c'est le cas on ignore
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

        // Verifie si la classe est entre 1 et 10, si c'est pas le cas on ignore
        int class_num;
        sscanf(entry->d_name, "s%d", &class_num);
        if (class_num < 1 || class_num > 10){
            fclose(file);
            continue;
        }

        // Creation du descripteur
        ShapeDescriptor descriptor;
        sscanf(entry->d_name, "s%dn%d", &descriptor.class, &descriptor.sample);
        descriptor.featureCount = 0;
        while (fscanf(file, "%f", &descriptor.features[descriptor.featureCount]) == 1){
            descriptor.featureCount++;
        }
        fclose(file);

        // Ajout du descripteur au tableau
        dataSet->descriptors = (ShapeDescriptor *)realloc(dataSet->descriptors, (dataSet->n_descriptors + 1) * sizeof(ShapeDescriptor));
        dataSet->descriptors[dataSet->n_descriptors++] = descriptor;
    }

    closedir(dir);
    return dataSet;
}

void freeDataSet(DataSet *dataSet){
    if (dataSet){
        free(dataSet->descriptors);
        free(dataSet);
    }
}

void printDataSet(const DataSet *dataSet){
    for (int i = 0; i < dataSet->n_descriptors; i++){
        ShapeDescriptor desc = dataSet->descriptors[i];
        printf("Classe: %d, Echantillon: %d, Caracteristiques: ", desc.class, desc.sample);
        for (int j = 0; j < desc.featureCount; j++){
            printf("%.2f ", desc.features[j]);
        }
        printf("\n");
    }
}

int main(){
    const char *dataDirectory = "./=SharvitB2/=Signatures/=Zernike7";
    DataSet *dataSet = readDataSet(dataDirectory);
    if (dataSet){
        printDataSet(dataSet);
        printf("Chargement termine : %d descripteurs trouves.\n", dataSet->n_descriptors);
        freeDataSet(dataSet);
    }
    else{
        printf("Erreur lors du chargement des donnees.\n");
    }
    return 0;
}
