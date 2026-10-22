#include <stdio.h>

int main() {
    int start, end, prim, i;

    scanf("%d %d", &start, &end);

    i = start;
    do {
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
        i++;
    } while(i <= end);

    printf("\n");

    return 0;
}