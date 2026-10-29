#include <stdio.h>

void endian(void) {
    int a = 1;
    unsigned char *ch = (unsigned char*)&a;
    if(*ch < 1) {
        printf("Big-endian\n");
    } else {
        printf("Little-endian\n");
    }
}

int main() {
    endian();

    return 0;
}