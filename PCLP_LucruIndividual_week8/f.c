#include <stdio.h>

int inverse_file_contents(const char *filename) {
    FILE *file = fopen(filename, "r+");

    if(file == NULL) {
        return 1;
    }

    size_t file_size = 0;
    while(fgetc(file) != EOF) {
        file_size++;
    }

    for(size_t i = 0; i < file_size / 2; i++) {
        char tmp_start, tmp_end;
        if(fseek(file, i, SEEK_SET) != 0) {
            return 1;
        }
        tmp_start = fgetc(file);
        if(tmp_start == EOF) {
            return 1;
        }
        
        if(fseek(file, file_size - 1 - i, SEEK_SET) != 0) {
            return 1;
        }
        tmp_end = fgetc(file);
        if(tmp_end == EOF) {
            return 1;
        }

        if(fseek(file, i, SEEK_SET) != 0) {
            return 1;
        }
        if(fputc(tmp_end, file) == EOF) {
            return 1;
        }
        if(fseek(file, file_size - 1- i, SEEK_SET) != 0) {
            return 1;
        }
        if(fputc(tmp_start, file) == EOF) {
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int main(void) {
    if(inverse_file_contents("f.in") != 0) {
        printf("ERROR\n");
        return 1;
    }

    return 0;
}