#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 100

int find_substring_in_file(FILE *finput, FILE *fout, char *substring) {
    char *line = malloc(MAX_LINE_LEN), *line_num = malloc(10);
    
    for(size_t i = 0; fgets(line, MAX_LINE_LEN, finput) != NULL; i++) {
        *(line + MAX_LINE_LEN - 1) = 0;

        if(strstr(line, substring) != NULL) {
            snprintf(line_num, 10, "%ld: ", i);
            *(line_num + 9) = 0;
            fputs(line_num, fout);

            fputs(line, fout);
            if(*(line + strlen(line) - 1) != '\n') {
                fputs("\n", fout);
            }
        }
    }

    free(line);
    free(line_num);

    return 0;
}

int main(void) {
    find_substring_in_file(stdin, stdout, "a");

    return 0;
}