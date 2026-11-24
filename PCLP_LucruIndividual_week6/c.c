#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int insert_new_line_at(int ***mat, size_t *n, size_t *m, size_t *index) {
    if(*index >= *n) {
        return 1;
    }

    *n = *n + 1;
    mat[0] = realloc(mat[0], *n * sizeof(int*));

    memmove(&mat[0][*index + 1], &mat[0][*index], (*n - *index) * sizeof(int*));

    mat[0][*index] = malloc(*m * sizeof(int));

    for(size_t i = 0; i < *m; i++) {
        scanf("%d", &mat[0][*index][i]);
    }

    return 0;
}

int insert_new_column_at(int ***mat, size_t *n, size_t *m, size_t *index) {
    if(*index >= *m) {
        return 1;
    }

    *m = *m + 1;
    for(size_t i = 0; i < *n; i++) {
        mat[0][i] = realloc(mat[0][i], *m * sizeof(int));

        memmove(&mat[0][i][*index + 1], &mat[0][i][*index], (*m - *index) * sizeof(int));

        scanf("%d", &mat[0][i][*index]);
    }

    return 0;
}

int main() {
    int **mat;
    size_t n, m, k, p, rc;

    scanf("%lu %lu", &n, &m);

    mat = malloc(n * sizeof(int*));
    for(int i = 0; i < n; i++) {
        mat[i] = malloc(m * sizeof(int));
    }

    for(size_t i = 0; i < n; i++) {
        for(size_t j = 0; j < m; j++) {
            scanf("%d", &mat[i][j]);
        }
    }

    printf("k = ");
    scanf("%lu", &k);

    for(int i = 0; i < k; i++) {
        scanf("%lu %lu", &p, &rc);

        switch (rc) {
        case 1:
            printf("Elementele noului rand:\n");
            insert_new_line_at(&mat, &n, &m, &p);
            break;
        case 2:
            printf("Elementele noii coloane:\n");
            insert_new_column_at(&mat, &n, &m, &p);
            break;
        default:
            printf("Comenzi: \n1 - insereaza rand now\n2 - insereaza coloana noua\n");
            break;
        }

        for(int j = 0; j < n; j++) {
            for(int k = 0; k < m; k++) {
                printf("%d ", mat[j][k]);
            }
            printf("\n");
        }
    }

    return 0;
}