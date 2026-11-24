#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void put_students(FILE *out, size_t n, const char **nume, const float *medii) {
    for(size_t i = 0; i < n; i++) {
        char *indice = malloc(5);
        indice[4] = 0;
        indice[3] = i % 10 + '0';
        indice[2] = i / 10 % 10 + '0';
        indice[1] = i / 100 % 10 + '0';
        indice[0] = i / 1000 % 10 + '0';
        fputs(indice, out);
        fputs("|", out);

        char *nume1 = malloc(13);
        strncpy(nume1, *(nume + i), 12);
        for(int j = strlen(nume1); j < 12; j++) {
            *(nume1 + j) = ' ';
        }
        *(nume1 + 12) = 0;
        fputs(nume1, out);
        fputs("|", out);

        char *medie = malloc(7);
        snprintf(medie, 6, "%.*f", 2, medii[i]);
        for(int j = strlen(medie); j < 6; j++) {
            *(medie + j) = ' ';
        }
        *(medie + 6) = 0;
        fputs(medie, out);
        fputs("\n", out);

        free(indice);
        free(nume1);
        free(medie);
    }
}

int main(void) {
    char *nume[3] = {"Ana", "Matei", "1234567890abcdef"};
    float medii[3] = {9.0, 8, 7.521};

    put_students(stdout, 3, (const char**)nume, medii);

    return 0;
}