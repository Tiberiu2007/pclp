#include <stdio.h>

#define NMAX 1000

union un {
    float f;
    int i;
};

int main() {
    int n;
    union un vec[NMAX];

    scanf("%d", &n);\
    if(n > NMAX) {
        n = NMAX;
    }

    for(int i = 0; i < n; i++) {
        scanf("%f", &vec[i].f);
    }

    for(int i = 0; i < n; i++) {
        printf("%f,%d\n", vec[i].f, vec[i].i);
    }

    return 0;
}