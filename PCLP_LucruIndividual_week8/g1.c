#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

int main(void) {
    int **mat;
    size_t n, m;

    scanf("%zu %zu", &n, &m);

    mat = malloc(n * sizeof(int*));
    for(size_t i = 0; i < n; i++) {
        *(mat + i) = malloc(m * sizeof(int));
        if(*(mat + i) == NULL) {
            free(mat);
            return 1;
        }
    }

    for(size_t i = 0; i < n; i++) {
        for(size_t j = 0; j < m; j++) {
            scanf("%d", &mat[i][j]);
        }
    }

    FILE *file = fopen("g.out", "wb");
    if(file == NULL) {
        for(size_t i = 0; i < n; i++) {
            free(*(mat + i));
        }
        free(mat);

        printf("ERROR: Nu s-a putut deschide fisierul!\n");
        return 1;
    }

    if(fwrite(&n, sizeof(n), 1, file) != 1) {
        fclose(file);
        for(size_t i = 0; i < n; i++) {
            free(*(mat + i));
        }
        free(mat);

        printf("ERROR: Nu s-a putut scrie n!\n");
        return 1;
    }

    if(fwrite(&m, sizeof(m), 1, file) != 1) {
        fclose(file);
        for(size_t i = 0; i < n; i++) {
            free(*(mat + i));
        }
        free(mat);

        printf("ERROR: Nu s-a putut scrie m!\n");
        return 1;
    }

    for(size_t i = 0; i < n; i++) {
        if(fwrite(*(mat + i), sizeof(int), m, file) != m) {
            fclose(file);
            for(size_t i = 0; i < n; i++) {
                free(*(mat + i));
            }
            free(mat);

            printf("ERROR: Nu s-a putut scrie mat de %zu!\n", i);
            return 1;
        }
    }

    fclose(file);
    for(size_t i = 0; i < n; i++) {
        free(*(mat + i));
    }
    free(mat);

    return 0;
}