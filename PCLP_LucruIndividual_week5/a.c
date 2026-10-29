#include <stdio.h>

union {
    int i;
    float f;
} u;

int main() {
    u.f = 1.0f;

    printf("%d\n", u.i);

    return 0;
}