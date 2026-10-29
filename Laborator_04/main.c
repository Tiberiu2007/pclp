#include <stdio.h>

#define NMAX 100

int citire(int *v) {
    int len;

    scanf("%d", &len);

    if(len > 100)
        len = 100;

    for(int i = 0; i < len; i++) {
        scanf("%d", &v[i]);
    }

    return len;
}

void print_array(int *v, int len) {
    for(int i = 0; i < len; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

int sum_array(int v[], int n) {
    int suma = 0;

    for(int i = 0; i < n; i++) {
        suma += v[i];
    }

    return suma;
}

int min_array(int v[], int n) {
    int min = 1 << 29;

    for(int i = 0; i < n; i++) {
        if(v[i] < min)
            min = v[i];
    }

    return min;
}

int posmax_array(int v[], int n) {
    int max = 1 << 31, maxi = -1;

    for(int i = 0; i < n; i++) {
        if(v[i] > max) {
            max = v[i];
            maxi = i;
        }
    }

    return maxi;
}

double avg_array(int v[], int n) {
    int sum = sum_array(v, n);

    return (double)sum / (double)n;
}

int find_array(int v[], int n, int target) {
    for(int i = 0; i < n; i++) {
        if(v[i] == target)
            return i;
    }

    return -1;
}

void sort_array(int v[], int n) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n - 1; j++) {
            if(v[j] > v[j + 1]) {
                int tmp = v[j];
                v[j] = v[j+1];
                v[j+1] = tmp;
            }
        }
    }
}

void multiply_matrix(int n, int m, int p, int a[NMAX][NMAX], int b[NMAX][NMAX], int c[NMAX][NMAX]) {
    int out[NMAX][NMAX];

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < p; j++) {
            int sum = 0;
            for(int k = 0; k < m; k++) {
                sum += a[i][k] * b[k][j];
            }
            out[i][j] = sum;
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < p; j++) {
            c[i][j] = out[i][j];
        }
    }
}

void print_matrix(int a[NMAX][NMAX], int n, int m) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
}

void power_matrix(int n, int a[NMAX][NMAX], int k, int b[NMAX][NMAX]) {
    for(int i = 0; i < k; i++) {
        multiply_matrix(n, n, n, a, a, a);
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            b[i][j] = a[i][j];
        }
    }
}



int main() {
    int vec[NMAX];

    int len = citire(vec);
    print_array(vec, len);

    int v[] = {10, 30, -1, 4, 5};
    len = 5;

    printf("Suma elementelor: %d\n", sum_array(v, len));
    printf("min_array: %d\n", min_array(v, len));
    printf("posmax_array: %d\n", posmax_array(v, len));
    printf("avg_array: %lf\n", avg_array(v, len));
    printf("pos 30: %d\n", find_array(v, len, 30));
    printf("pos 4: %d\n", find_array(v, len, 4));
    sort_array(v, len);
    print_array(v, len);

    int a[NMAX][NMAX] = {
        { 0,  1,  1,  1},
        { 1,  1,  1,  1},
        { 0, 10,  0,  0},
        { 0, 10,  0,  0}
    };

    int b[NMAX][NMAX] = {
        { -1, 10,  1},
        { -2, 20,  2},
        { -3, 30,  3},
        { -4, 40,  4}
    };

    int c[NMAX][NMAX];

    multiply_matrix(4, 4, 3, a, b, c);
    print_matrix(c, 4, 3);

    power_matrix(4, a, 2, b);
    print_matrix(b, 4, 4);

    return 0;
}