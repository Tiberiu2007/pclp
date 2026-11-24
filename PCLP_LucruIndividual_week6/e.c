
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void swap_lines(float **mat, size_t line1, size_t line2) {
    float* tmp = mat[line1];
    mat[line1] = mat[line2];
    mat[line2] = tmp;
}

void divide_line_by_k(float *arr, size_t m, float k) {
    for(size_t i = 0; i < m; i++) {
        arr[i] /= k;
    }
}

void sum_lines(float *lm, float *l, float k, size_t m) {
    for(size_t i = 0; i < m; i++) {
        l[i] = l[i] + k * lm[i];
    }
}

int gauss(float ***mat, size_t *n, size_t *m) {
    scanf("%lu %lu", n, m);

    mat[0] = malloc(*n * sizeof(int*));

    for(size_t i = 0; i < *n; i++) {
        mat[0][i] = malloc(*m * sizeof(int));
        for(int j = 0; j < *m; j++) {
            scanf("%f", &mat[0][i][j]);
        }
    }

    for(size_t i = 0; i < *n; i++) {
        if(mat[0][i][i] == 0.0f) {
            for(size_t j = i + 1; j < *n; j++) {
                if(mat[0][j][i] != 0.0f) {
                    swap_lines(mat[0], i, j);
                }
            }
        }

        divide_line_by_k(mat[0][i], *m, mat[0][i][i]);

        for(size_t j = 0; j < *n; j++) {
            if(j == i)
                continue;

            sum_lines(mat[0][i], mat[0][j], -1.0f * mat[0][j][i], *m);
        }
    }

    return 0;
}

int main() {
    float **mat;
    size_t n, m;

    char err = gauss(&mat, &n, &m);

    for(size_t i = 0; i < n; i++) {
        for(size_t j = 0; j < m; j++) {
            printf("%f ", mat[i][j]);
        }
        printf("\n");
    }

    return 0;
}