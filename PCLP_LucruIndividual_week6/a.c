#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int remove_value_from_array(int *arr, int len, int e);
void quick_sort(int *arr, int low, int high);
int binary_search(int *arr, int len, int x);
int** matrix(int n);

int main() {
    int *vec, n;

    scanf("%d", &n);

    vec = malloc(n * sizeof(int));

    for(int i = 0; i < n; i++) {
        scanf("%d", &vec[i]);
    }

    // Minimul
    int min = INT_MAX;
    for(int i = 0; i < n; i++) {
        if(vec[i] < min) {
            min = vec[i];
        }
    }

    // Maximul
    int max = INT_MIN;
    for(int i = 0; i < n; i++) {
        if(vec[i] > max) {
            max = vec[i];
        }
    }

    // Media aritmetica
    double sum = 0.0;
    for(int i = 0; i < n; i++) {
        sum += (float)vec[i];
    }

    printf("Minimul: %d\nMaximul: %d\nMedia aritmetica: %lf\n", min, max, sum / n);

    n = remove_value_from_array(vec, n, 2);
    quick_sort(vec, 0, n - 1);
    int bi = binary_search(vec, n, 3);
    int **mat = matrix(5);

    for(int i = 0; i < n; i++) {
        printf("%d ", vec[i]);
    }
    printf("\n");

    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }

    free(vec);
    free(mat);
    return 0;
}

int remove_value_from_array(int *arr, int len, int e) {
    for(int i = 0; i < len; i++) {
        if(arr[i] == e) {
            memmove(&arr[i], &arr[i+1], (len - i - 1) * sizeof(int));
            arr = realloc(arr, (len - 1) * sizeof(int));
            len--;
        }
    }

    return len;
}

void quick_sort(int *arr, int low, int high) {
    if(low < high) {
        int pivot = arr[high];

        int i = low - 1;

        for(int j = low; j < high; j++) {
            if(arr[j] < pivot) {
                i++;
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
        }

        int tmp = arr[i+1];
        arr[i+1] = arr[high];
        arr[high] = tmp;

        int pi = i + 1;

        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

int binary_search(int *arr, int len, int x) {
    int low = 0;
    int high = len - 1;

    while(low <= high) {
        int mid = (low + high) / 2;

        if(arr[mid] < x) {
            low = mid + 1;
        } else if(arr[mid] > x) {
            high = mid - 1;
        } else {
            return mid;
        }
    }

    return -1;
}

int** matrix(int n) {
    int **mat = malloc(n * sizeof(int*));
    for(int i = 0; i < n; i++) {
        mat[i] = malloc(n * sizeof(int));
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(j > i) {
                mat[i][j] = 1;
            } else if(j < i) {
                mat[i][j] = 2;
            } else {
                mat[i][j] = 3;
            }

            if(i + j == n - 1) {
                mat[i][j] = 4;
            }
        }
    }

    return mat;
}