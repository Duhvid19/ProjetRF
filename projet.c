#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#define INITIAL_CAPACITY 10

typedef struct{
    float *attributes;
    size_t size;
    size_t capacity;
} Data;

typedef struct{
    Data *data_array;
    size_t size;
    size_t capacity;
} Methode;

void init_data(Data *data){
    data->attributes = malloc(INITIAL_CAPACITY * sizeof(float));
    data->size = 0;
    data->capacity = INITIAL_CAPACITY;
}

void add_attribute(Data *data, float attribute){
    if (data->size >= data->capacity){
        data->capacity *= 2;
        data->attributes = realloc(data->attributes, data->capacity * sizeof(float));
    }
    data->attributes[data->size] = attribute;
    data->size++;
}

void free_data(Data *data){
    free(data->attributes);
}

void init_methode(Methode *methode){
    methode->data_array = malloc(INITIAL_CAPACITY * sizeof(Data));
    methode->size = 0;
    methode->capacity = INITIAL_CAPACITY;
}

void add_data(Methode *methode, Data *data){
    if (methode->size >= methode->capacity){
        methode->capacity *= 2;
        methode->data_array = realloc(methode->data_array, methode->capacity * sizeof(Data));
    }
    methode->data_array[methode->size] = *data;
    methode->size++;
}

void free_Methode(Methode *methode){
    for (size_t i = 0; i < methode->size; i++){
        free_data(&methode->data_array[i]);
    }
    free(methode->data_array);
}

void read_file(const char *file_name, Data *data){
    FILE *file = fopen(file_name, "r");
    if (file == NULL){
        printf("Erreur lors de l'ouverture du fichier: %s\n", file_name);
        return;
    }

    float attribute;
    while (fscanf(file, "%f", &attribute) == 1){  // Lire chaque ligne comme un float
        add_attribute(data, attribute);
    }

    fclose(file);
}

void read_dir(const char *dir_name, Methode *methode){
    DIR *directory = opendir(dir_name);
    struct dirent *entree;

    if (directory == NULL){
        printf("Erreur lors de l'ouverture du dossier %s\n", dir_name);
        return;
    }

    while ((entree = readdir(directory)) != NULL){
        if (entree->d_type == DT_REG){
            char path[512];
            snprintf(path, sizeof(path), "%s/%s", dir_name, entree->d_name);

            Data data;
            init_data(&data);
            read_file(path, &data);
            add_data(methode, &data);
        }
    }

    closedir(directory);
}

void print_Methode(const Methode *methode){
    for (size_t i = 0; i < methode->size; i++){
        printf("Donnée %zu:\n", i + 1);
        for (size_t j = 0; j < methode->data_array[i].size; j++) {
            printf("  Attribut %zu: %f\n", j + 1, methode->data_array[i].attributes[j]); // précision à 6 décimales par défaut avec %f
        }
        printf("\n");
    }
}

int main(){
    const char *dir = "./=SharvitB2/=Signatures/=Zernike7";
    Methode methode;
    init_methode(&methode);

    read_dir(dir, &methode);
    print_Methode(&methode);

    free_Methode(&methode);
    return 0;
}
