#include <stdio.h>

int main() {
    int start, end, prim, i;

    scanf("%d %d", &start, &end);

    i = start;
    while(i <= end) {
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
    }

    printf("\n");

    return 0;
}