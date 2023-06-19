#include <stdio.h>

int save_file(char* file, int dimension, uint8_t* image) {
    char *filename = file;
    FILE *fp = fopen(file, "w");
    if (fp == NULL)
    {
        printf("Error opening the file %s", filename);
        return -1;
    }
    for (int i = 0; i < dimension; i++)
        fprintf(fp, "%u ", image[i]);
    fclose(fp);

    return 0;
}