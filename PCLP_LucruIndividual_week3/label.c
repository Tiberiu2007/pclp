#include <stdio.h>

int main() {
    int start, end, i;

    scanf("%d %d", &start, &end);

    i = start;
    loop:
    if(i <= end) {
        if(i < 2) {
            goto not_prime;
        }

        for(int d = 2; d * d <= i; d++) {
            if(i % d == 0) {
                goto not_prime;
            }
        }

        printf("%d ", i);

        not_prime:
        i++;
        goto loop;
    }

    printf("\n");

    return 0;
}