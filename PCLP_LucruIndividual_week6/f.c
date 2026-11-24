#include <stdio.h>
#include <stdlib.h>

float **alloc_matrix(size_t n) {
    float **mat = malloc(n * sizeof(int*));
    if(mat == NULL) {
        return NULL;
    }

    for(size_t i = 0; i < n; i++) {
        mat[i] = malloc(n * sizeof(int));

        if(mat[i] == NULL) {
            for(size_t j = 0; j < i; j++) {
                free(mat[j]);
            }
            free(mat);
            return NULL;
        }
    }

    return mat;
}

void free_matrix(float **mat, size_t n) {
    for(size_t i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
}

float **matrix_product(size_t n) {
    float **mat1 = alloc_matrix(n);
    float **mat2 = alloc_matrix(n);
    float **out = alloc_matrix(n);

    if(mat1 == NULL || mat2 == NULL || out == NULL) {
        return NULL;
    }

    // Citire
    for(size_t i = 0; i < n; i++) {
        for(size_t j = 0; j < n; j++) {
            scanf("%f", &mat1[i][j]);
        }
    }
    for(size_t i = 0; i < n; i++) {
        for(size_t j = 0; j < n; j++) {
            scanf("%f", &mat2[i][j]);
        }
    }

    for(size_t line = 0; line < n; line++) {
        for(size_t col = 0; col < n; col++) {
            out[line][col] = 0;
            for(size_t i = 0; i < n; i++) {
                out[line][col] += mat1[line][i] * mat2[i][col];
            }
        }
    }

    free_matrix(mat1, n);
    free_matrix(mat2, n);
    return out;
}

int main() {
    size_t n;

    scanf("%lu", &n);

    float **mat = matrix_product(n);

    for(size_t i = 0; i < n; i++) {
        for(size_t j = 0; j < n; j++) {
            printf("%f ", mat[i][j]);
        }
        printf("\n");
    }

    free_matrix(mat, n);

    return 0;
}