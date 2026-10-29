#include <stdio.h>

int main() {
    int a, b, c;
    int *p1 = &a, *p2 = &b, *p3 = &c;

    *p1 = 10;
    *p2 = *p1;
    *p3 = 2 * (*p2);
    printf("a = %d, b = %d, c = %d\n", *p1, *p2 , *p3);

    return 0;
}