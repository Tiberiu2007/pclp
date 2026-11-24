#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_students(const char *in) {
    char *str1 = strdup(in);
    size_t len = strlen(str1);

    char *p1 = strtok(str1, ";");
    char *p2 = strtok(NULL, ";");
    if(p2 == NULL) {
        p2 = str1 + len;
    }

    while(p1 != str1 + len) {
        char *str2 = malloc(p2 - p1 + 1);
        strncpy(str2, p1, p2 - p1);
        *(str2 + (p2 - p1)) = 0;
        char *p3 = strchr(str2, ':');
        *p3 = 0;
        printf("%s\t%s\n", str2, p3 + 1);

        p1 = p2;
        p2 = strtok(NULL, ";");
        if(p2 == NULL) {
            p2 = str1 + len;
        }

        free(str2);
    }

    free(str1);
}

int main(void) {
    print_students("ana:8,10;dan:7,9;ioana:10");

    return 0;
}