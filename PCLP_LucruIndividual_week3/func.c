#include <stdio.h>
#include <stdlib.h>

int is_prim(int n);
void prime_nums(int start, int end);

int main(int argc, char** argv) {
    if(argc < 3) {
        return -1;
    }

    int start = atoi(argv[1]);
    int end = atoi(argv[2]);

    prime_nums(start, end);

    return 0;   
}

int is_prim(int n) {
    if(n < 2) {
        return 0;
    }

    for(int d = 2; d * d <= n; d++) {
        if(n % d == 0) {
            return 0;
        }
    }

    return 1;
}

void prime_nums(int start, int end) {
    for(int i = start; i <= end; i++) {
        if(is_prim(i)) {
            printf("%d ", i);
        }
    }

    printf("\n");
}