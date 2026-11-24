#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void add(
    unsigned char *v1, size_t n1,
    unsigned char *v2, size_t n2,
    unsigned char **out, size_t *n_out
) {
    size_t ns, i = 0;
    unsigned char *vs, s, rest = 0;

    if(n1 < n2) {
        ns = n2;
    } else {
        ns = n1;
    }

    vs = malloc(ns * sizeof(unsigned char));

    while(i < n1 && i < n2) {
        s = v1[n1 - 1 - i] + v2[n2 - 1 - i] + rest;
        vs[ns - 1 - i] = s % 10;
        rest = s / 10;
        i++;
    }
    while(i < n1) {
        s = v1[n1 - 1 - i] + rest;
        vs[ns - 1 - i] = s % 10;
        rest = s / 10;
        i++;
    }
    while(i < n2) {
        s = v2[n2 - 1 - i] + rest;
        vs[ns - 1 - i] = s % 10;
        rest = s / 10;
        i++;
    }

    if(rest > 0) {
        ns++;
        vs = realloc(vs, ns * sizeof(unsigned char));
        memmove(&vs[1], &vs[0], (ns - 1) * sizeof(unsigned char));
        vs[0] = rest;
    }

    *out = vs;
    *n_out = ns;
}

void multiplication(
    unsigned char *v1, size_t n1, 
    unsigned char *v2, size_t n2,
    unsigned char **out, size_t *n_out) {

    
}

int main() {
    size_t n1, n2, n;
    unsigned char *v1, *v2, *v;

    scanf("%lu %lu", &n1, &n2);

    v1 = malloc(n1 * sizeof(unsigned char));
    v2 = malloc(n2 * sizeof(unsigned char));

    for(size_t i = 0; i < n1; i++) {
        scanf("%hhu", &v1[i]);
        if(v1[i] > 9) {
            printf("Eroare: Numar mai mare decat 9.\n");
            return 1;
        }
    }

    for(size_t i = 0; i < n2; i++) {
        scanf("%hhu", &v2[i]);
        if(v2[i] > 9) {
            printf("Eroare: Numar mai mare decat 9.\n");
            return 1;
        }
    }

    add(v1, n1, v2, n2, &v, &n);

    // Afisare vs
    for(int i = 0; i < n; i++) {
        printf("%hhu ", v[i]);
    }
    printf("\n");

    free(v);

    multiplication(v1, n1, v2, n2, &v, &n);
    // Afisare vs
    for(int i = 0; i < n; i++) {
        printf("%hhu ", v[i]);
    }
    printf("\n");

    free(v1);
    free(v2);
    free(v);
    return 0;
}