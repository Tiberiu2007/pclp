#include <stdio.h>

void compute(int *a, int *b, char operation, int *result) {
    switch (operation) {
    case '+':
        *result = *a + *b;
        break;
    case '-':
        *result = *a - *b;
        break;
    case '*':
        *result = *a * *b;
        break;
    case '/':
        *result = *a / *b;
        break;
    }
}

int main() {
    int a, b, result;

    a = 5; b = 5;
    compute(&a, &b, '+', &result);
    printf("%d ", result);

    a = 10; b = 5;
    compute(&a, &b, '-', &result);
    printf("%d ", result);

    a = 5; b = 2;
    compute(&a, &b, '*', &result);
    printf("%d ", result);

    a = 10; b = 2;
    compute(&a, &b, '/', &result);
    printf("%d\n", result);

    return 0;
}