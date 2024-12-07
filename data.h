#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>

#define MAX_FEATURES 110
#define MAX_CLASSES 10


typedef struct{
    int class; 
    int sample; //possiblement inutile
    float features[MAX_FEATURES]; 
} Data;


typedef struct{
    Data *data;
    int data_count;
    int feature_count;
} Dataset;


Dataset *load_dataset(const char *directory); 
void free_dataset(Dataset *dataset); 
void print_dataset(const Dataset *dataset);
void normalize_dataset(Dataset *dataset);

#endif
