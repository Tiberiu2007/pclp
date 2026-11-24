#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int durata_secunde(const char *s, double *out) {
    char *tmp, *str;
    double secs;
    size_t len = strlen(s);

    str = malloc(len + 1);

    size_t j = 0;
    for(size_t i = 0; i < len; i++) {
        if(*(s + i) != ' ') {
            *(str + j) = *(s + i);
            j++;
        }
    }
    *(str + j) = 0;

    secs = strtof(str, &tmp);
    if(tmp == NULL) {
        free(str);
        return 1;
    }

    if(strcmp(tmp, "ms") == 0) {
        secs /= 1000.0;
    } else if(strcmp(tmp, "min") == 0) {
        secs *= 60;
    } else if(strcmp(tmp, "h") == 0) {
        secs *= 3600;
    } else if(strcmp(tmp, "s") != 0) {
        free(str);
        return 1;
    }

    *out = secs;

    free(str);
    return 0;
}

int main(void) {
    double a;

    if(!durata_secunde(" 12.5ms", &a)) {
        printf("%lf\n", a);
    }
    if(!durata_secunde(" 0.25s  ", &a)) {
        printf("%lf\n", a);
    }
    if(!durata_secunde("3min", &a)) {
        printf("%lf\n", a);
    }

    return 0;
}