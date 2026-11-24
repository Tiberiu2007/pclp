#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int **mat;
    size_t n, m;

    FILE *file = fopen("g.out", "rb");
    if(file == NULL) {
        return 1;
    }

    if(fread(&n, sizeof(n), 1, file) != 1) {
        fclose(file);
        printf("ERROR: Nu s-a putut citi n!\n");
        return 1;
    }
    if(fread(&m, sizeof(m), 1, file) != 1) {
        fclose(file);
        printf("ERROR: Nu s-a putut citi m!\n");
        return 1;
    }

    mat = malloc(n * sizeof(int*));
    if(mat == NULL) {
        fclose(file);
        printf("ERROR: Nu s-a putut aloca memoria!\n");
        return 1;
    }

    for(size_t i = 0; i < n; i++) {
        *(mat + i) = malloc(m * sizeof(int));
        if(*(mat + i) == NULL) {
            for(size_t j = 0; j < i; j++) {
                free(*(mat + j));
            }
            free(mat);
            fclose(file);
            printf("ERROR: Nu s-a putut aloca memoria!\n");
            return 1;
        }
    }

    for(size_t i = 0; i < n; i++) {
        if(fread(*(mat + i), sizeof(int), m, file) != m) {
            for(size_t j = 0; j < n; j++) {
                free(*(mat + j));
            }
            free(mat);
            fclose(file);
            printf("ERROR: Nu s-a putut citi matricea!\n");
            return 1;
        }
    }

    // Afiseaza matricea
    printf("%zu\n%zu\n", n, m);
    for(size_t i = 0; i < n; i++) {
        for(size_t j = 0; j < m; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }

    for(size_t i = 0; i < n; i++) {
        free(*(mat + i));
    }
    free(mat);
    fclose(file);
    return 0;
}