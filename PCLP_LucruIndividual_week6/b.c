#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int insert_slice(int **v, size_t *n, size_t *capacity, size_t position, size_t first, size_t count) {
    if(*n + count > *capacity) {
        return 1;
    }
    if(position > *n - 1 || first > *n - 1) {
        return 2;
    }

    *v = realloc(*v, (*n + count) * sizeof(int));
    int *tmp = malloc(count * sizeof(int));
    memcpy(tmp, &v[0][first], count * sizeof(int));
    memmove(&v[0][position + count], &v[0][position], (*n - position) * sizeof(int));
    memcpy(&v[0][position], tmp, count * sizeof(int));
    free(tmp);
    *n = *n + count;
    return 0;
}

int main() {
    int *arr;
    size_t n, capacity = 100;

    scanf("%lu", &n);
    if(n > capacity) {
        n = capacity;
    }

    arr = malloc(n * sizeof(int));

    for(int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    insert_slice(&arr, &n, &capacity, 2, 1, 3);

    for(int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}