#include <stdio.h>

void jmecherie(int a, int b) {
    int x = 1, y = 2, z = 3;

    printf("x = %d ", *(&y + 2));
    printf("z = %d ", *(&y + 1));
    printf("a = %d ", *(&y - 2));
    printf("b = %d\n", *(&y - 3));
} 

int main() {
    jmecherie(4, 5);

    return 0;
}