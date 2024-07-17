#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

cl_int LoadMatrix(const char *path, Matrix *matrix)
{
    FILE *data_file;

    data_file = fopen(path, "r");
    if (!data_file) // Error opening file
        return CL_INVALID_VALUE;

    unsigned int rows = 0;
    unsigned int cols = 0;
    if (fscanf(data_file, "# (%u, %u)\n", &rows, &cols) == EOF)
        return CL_INVALID_VALUE; // Error parsing dimensions

    if (rows == 0)
        rows = 1;
    if (cols == 0)
        cols = 1;

    matrix->shape[0] = rows;
    matrix->shape[1] = cols;

    matrix->data = malloc(sizeof(float) * rows * cols);
    if (!matrix->data) // Error mallocing matrix data
        return CL_OUT_OF_HOST_MEMORY;

    int n = 0;
    while (fscanf(data_file, "%f", &(matrix->data[n++])) != EOF)
        ;

    fclose(data_file);

    return CL_SUCCESS;
}