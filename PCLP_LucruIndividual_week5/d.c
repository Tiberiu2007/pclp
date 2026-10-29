#include <stdio.h>

void shift_pointers(int **addr_p1, int **addr_p2, int **addr_p3) {
    int *tmp = *addr_p1;
    *addr_p1 = *addr_p2;
    *addr_p2 = *addr_p3;
    *addr_p3 = tmp;
}

int main() {
    int a = 1, b = 2, c = 3;
    int *p1 = &a, *p2 = &b, *p3 = &c;

    printf("*p1 = %d, *p2 = %d, *p3 = %d\n", *p1, *p2, *p3);

    shift_pointers(&p1, &p2, &p3);
    printf("*p1 = %d, *p2 = %d, *p3 = %d\n", *p1, *p2, *p3);
    
    shift_pointers(&p1, &p2, &p3);
    printf("*p1 = %d, *p2 = %d, *p3 = %d\n", *p1, *p2, *p3);

    shift_pointers(&p1, &p2, &p3);
    printf("*p1 = %d, *p2 = %d, *p3 = %d\n", *p1, *p2, *p3);

    return 0;
}