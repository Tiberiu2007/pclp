#include <stdio.h>

int main() {
    int start, end, prim;

    scanf("%d %d", &start, &end);

    for(int i = start; i <= end; i++) {
        prim = 1;

        if(i < 2) {
            prim = 0;
        }

        for(int d = 2; d * d <= i; d++) {
            if(i % d == 0) {
                prim = 0;
            }
        }

        if(prim) {
            printf("%d ", i);
        }
    }

    printf("\n");

    return 0;
}