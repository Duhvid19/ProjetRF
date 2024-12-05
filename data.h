#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

// Def nbr Max caractériqtiques 
#define MAX_FEATURES 50
// Def nbr Max classe 
#define MAX_CLASSES 10


typedef struct {
    int class; 
    int sample; //possiblement inutile
    float features[MAX_FEATURES]; 
    int feature_count; 
} Data;


typedef struct {
    Data *data; 
    int data_count; 
} Dataset;


Dataset *load_dataset(const char *directory); 
void free_dataset(Dataset *dataset); 
void print_dataset(const Dataset *dataset); 

#endif
