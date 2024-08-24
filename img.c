#include <stdio.h>
#include <stdlib.h>

#include "img.h"

#define RGB_COMPONENT_COLOR 255

cl_int LoadImg(const char *path, Matrix* img)
{
    char buff[16];
    FILE *fp;
    int c, rgb_comp_color;
    //open PPM file for reading
    fp = fopen(path, "rb");
    if (!fp) {
        fprintf(stderr, "Unable to open file '%s'\n", path);
        return CL_INVALID_VALUE;
    }

    //read image format
    if (!fgets(buff, sizeof(buff), fp)) {
        perror(path);
        return CL_INVALID_VALUE;
    }

    //check the image format
    if (buff[0] != 'P' || buff[1] != '6') {
        fprintf(stderr, "Invalid image format (must be 'P6')\n");
        return CL_INVALID_VALUE;
    }

    //check for comments
    c = getc(fp);
    while (c == '#') {
    while (getc(fp) != '\n') ;
        c = getc(fp);
    }

    ungetc(c, fp);
    //read image size information
    if (fscanf(fp, "%d %d", &img->shape[0], &img->shape[1]) != 2) {
        fprintf(stderr, "Invalid image size (error loading '%s')\n", path);
        return CL_INVALID_VALUE;
    }

    //read rgb component
    if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
        fprintf(stderr, "Invalid rgb component (error loading '%s')\n", path);
        return CL_INVALID_VALUE;
    }

    //check rgb component depth
    if (rgb_comp_color != RGB_COMPONENT_COLOR) {
        fprintf(stderr, "'%s' does not have 8-bits components\n", path);
        return CL_INVALID_VALUE;
    }

    while (fgetc(fp) != '\n') ;
    //memory allocation for pixel data
    unsigned char* data = (unsigned char *)malloc(img->shape[0] * img->shape[1] * IMAGE_CHANNELS * sizeof(char));
    img->data = (float *)malloc(img->shape[0] * img->shape[1] * IMAGE_CHANNELS * sizeof(float));

    if (!data || !img->data) {
        fprintf(stderr, "Unable to allocate memory\n");
        return CL_INVALID_VALUE;
    }

    //read pixel data from file
    if (fread(data, IMAGE_CHANNELS * img->shape[0], img->shape[1], fp) != img->shape[1]) {
        fprintf(stderr, "Error loading image '%s'\n", filename);
        return CL_INVALID_VALUE;
    }

    int count = img->shape[0] * img->shape[1];
    for (int i = 0; i < count; i++)
    {
        img->data[i] = (float)data[i] / 255.0f;
    }

    fclose(fp);
    free(data);

    return CL_SUCCESS;
}

cl_int SaveImg(const char *filename, Matrix* img)
{
    // Place holder
    return 0;
}

cl_int CheckImg(Matrix *truth, Matrix *student)
{
    if (truth->shape[0] != student->shape[0] || truth->shape[1] != student->shape[1])
    {
        printf("!!SOLUTION IS NOT CORRECT!!\n");
        return CL_INVALID_VALUE;
    }

    int count = truth->shape[0] * truth->shape[1] * IMAGE_CHANNELS;
    for (int i = 0; i < count; i++)
    {
        float epsilon = fabs(truth->data[i]) * 0.1f;
        float diff = fabs(truth->data[i] - student->data[i]);
        if (diff > epsilon)
        {
            printf("!!SOLUTION IS NOT CORRECT!! Expected: %0.2f, Found %0.2f\n", truth->data[i], student->data[i]);
            return CL_INVALID_VALUE;
        }
    }

    printf("!!SOLUTION IS CORRECT!!\n");
    return CL_SUCCESS;
}