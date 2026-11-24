#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/bmp_io_rgb.h"

#define MAX_LINE_LEN 100
#define UNK_ERR_CODE 1
#define ARG_ERR_CODE 2
#define IO_ERR_CODE 3
#define CMD_ERR_CODE 4
#define TAG_ERR_CODE 5

const char USAGE[] = "Utilizare: procesor_de_imagini <file>\n";

static struct image {
    unsigned char ***data;
    char *tag;
    size_t width;
    size_t height;
} *images;
static size_t images_len = 0;

struct image *find_image_by_tag(const char *tag);
int create(const char *tag, const size_t cols, const size_t rows);
int load(const char *tag, const char *path);
int clear(const char *tag, unsigned char red, unsigned char green, unsigned char blue);
int paste(const char *tag1, const char *tag2, size_t x, size_t y);
int rectangle(const char *tag, 
    size_t left, size_t right, size_t top, size_t bottom,
    unsigned char red, unsigned char green, unsigned char blue);
int save(const char *tag, const char *path);
int crop(const char *tag, size_t left, size_t right, size_t top, size_t bottom);
int grayscale(const char *tag);

int main(int argc, char **argv) {
    int return_code = 0;

    if(argc < 2) {
        printf(USAGE);
        return ARG_ERR_CODE;
    }

    FILE *file = fopen(argv[1], "r");
    if(file == NULL) {
        printf("Eroare: Nu s-a putut deschide fisierul %s!\n", argv[1]);
        return IO_ERR_CODE;
    }

    char *line = malloc(MAX_LINE_LEN);
    if(line == NULL) {
        fclose(file);
        return UNK_ERR_CODE;
    }

    int line_num = 1;
    while(fgets(line, MAX_LINE_LEN, file) != NULL) {
        if(line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }

        char *command = strtok(line, " ");
        if(command == NULL) {
            printf("Eroare: %d: Nu s-a putut citi comanda!\n", line_num);
            return_code = CMD_ERR_CODE;
            goto quit;
        }
        
        if(strcmp(command, "CREATE") == 0) {
            /*
            /   Comanda CREATE
            */
            char *tag = strtok(NULL, " ");
            if(tag == NULL) {
                printf("Eroare: %d: Nu s-a putut citi argumentul tag al comenzii CREATE!\n", line_num);
                return_code = CMD_ERR_CODE;
                goto quit;
            }

            char *end = NULL;

            char *cols_str = strtok(NULL, " ");
            if(cols_str == NULL) {
                printf("Eroare: %d: Nu s-a putut citi argumentul cols al comenzii CREATE!\n", line_num);
                return_code = CMD_ERR_CODE;
                goto quit;
            }
            size_t cols = strtoumax(cols_str, &end, 10);
            if(cols_str == end || *end != '\0') {
                printf("Eroare: %d: Argumentul cols al comenzii CREATE trebuie sa fie un numar!\n", line_num);
                return_code = CMD_ERR_CODE;
                goto quit;
            }

            char *rows_str = strtok(NULL, " ");
            if(rows_str == NULL) {
                printf("Eroare: %d: Nu s-a putut citi argumentul rows al comenzii CREATE!\n", line_num);
                return_code = CMD_ERR_CODE;
                goto quit;
            }
            size_t rows = strtoumax(rows_str, &end, 10);
            if(rows_str == end || *end != '\0') {
                printf("Eroare: %d: Argumentul rows al comenzii CREATE trebuie sa fie un numar!\n", line_num);
                return_code = CMD_ERR_CODE;
                goto quit;
            }

            switch (create(tag, cols, rows)) {
            case 0:
                break;
            case TAG_ERR_CODE:
                printf("Eroare: %d: Tag-ul este deja luat!\n", line_num);
                return_code = TAG_ERR_CODE;
                goto quit;
            default:
                printf("Eroare: %d: Comanda CREATE nu a putut rula!\n", line_num);
                return_code = UNK_ERR_CODE;
                goto quit;
            }
        } else if(strcmp(command, "LOAD") == 0) {
            /*
            /   Comanda LOAD
            */
            char *tag = strtok(NULL, " ");
            if(tag == NULL) {
                printf("Eroare: %d: Nu s-a putut citi argumentul tag al comenzii LOAD!\n", line_num);
                return_code = CMD_ERR_CODE;
                goto quit;
            }

            char *path = strtok(NULL, " ");
            if(path == NULL) {
                printf("Eroare: %d: Nu s-a putut citi argumentul path al comenzii LOAD!\n", line_num);
                return_code = CMD_ERR_CODE;
                goto quit;
            }

            switch (load(tag, path)) {
            case 0:
                break;
            case TAG_ERR_CODE:
                printf("Eroare: %d: Tag-ul este deja luat!\n", line_num);
                return_code = TAG_ERR_CODE;
                goto quit;
            case IO_ERR_CODE:
                printf("Eroare: %d: LOAD: Nu s-a putut deschide fisierul %s!\n", line_num, path);
                return_code = IO_ERR_CODE;
                goto quit;
            default:
                printf("Eroare: %d: Comanda LOAD nu a putut rula!\n", line_num);
                return_code = UNK_ERR_CODE;
                goto quit;
            }
        } else if(strcmp(command, "CLEAR") == 0) {
            char *tag = strtok(NULL, " ");
            if(tag == NULL) {
                printf("Eroare: %d: Nu s-a putut citi argumentul tag al comenzii CREATE!\n", line_num);
                return_code = CMD_ERR_CODE;
                goto quit;
            }

            char *end = NULL;

            // ROSU
            char *red_str = strtok(NULL, " ");
            if(red_str == NULL) {
                printf("Eroare: %d: Nu s-a putut citi argumentul red al comenzii CLEAR!\n", line_num);
                return_code = CMD_ERR_CODE;
                goto quit;
            }
            size_t red = strtoumax(red_str, &end, 10);
            if(red_str == end || *end != '\0' || red < 0 || red > 255) {
                printf("Eroare: %d: Argumentul red al comenzii CLEAR trebuie sa fie un numar intre 0 si 255!\n", line_num);
                return_code = CMD_ERR_CODE;
                goto quit;
            }

            // VERDE
            char *green_str = strtok(NULL, " ");
            if(green_str == NULL) {
                printf("Eroare: %d: Nu s-a putut citi argumentul green al comenzii CLEAR!\n", line_num);
                return_code = CMD_ERR_CODE;
                goto quit;
            }
            size_t green = strtoumax(green_str, &end, 10);
            if(green_str == end || *end != '\0' || green < 0 || green > 255) {
                printf("Eroare: %d: Argumentul green al comenzii CLEAR trebuie sa fie un numar intre 0 si 255!\n", line_num);
                return_code = CMD_ERR_CODE;
                goto quit;
            }

            // ALBASTRU
            char *blue_str = strtok(NULL, " ");
            if(blue_str == NULL) {
                printf("Eroare: %d: Nu s-a putut citi argumentul blue al comenzii CLEAR!\n", line_num);
                return_code = CMD_ERR_CODE;
                goto quit;
            }
            size_t blue = strtoumax(blue_str, &end, 10);
            if(blue_str == end || *end != '\0' || blue < 0 || blue > 255) {
                printf("Eroare: %d: Argumentul blue al comenzii CLEAR trebuie sa fie un numar intre 0 si 255!\n", line_num);
                return_code = CMD_ERR_CODE;
                goto quit;
            }

            switch (clear(tag, red, green, blue)) {
            case 0:
                break;
            case TAG_ERR_CODE:
                printf("Eroare: %d: Imaginea cu tag-ul %s nu exista!\n", line_num, tag);
                return_code = TAG_ERR_CODE;
                goto quit;
            default:
                printf("Eroare: %d: Comanda CREATE nu a putut rula!\n", line_num);
                return_code = UNK_ERR_CODE;
                goto quit;
            }
        } else if(strcmp(command, "PASTE") == 0) {
            char *tag1 = strtok(NULL, " ");
            if(tag1 == NULL) {
                printf("Eroare: %d: Nu s-a putut citi argumentul tag1 al comenzii PASTE!\n", line_num);
                return_code = CMD_ERR_CODE;
                goto quit;
            }

            char *tag2 = strtok(NULL, " ");
            if(tag2 == NULL) {
                printf("Eroare: %d: Nu s-a putut citi argumentul tag2 al comenzii PASTE!\n", line_num);
                return_code = CMD_ERR_CODE;
                goto quit;
            }

            char *end = NULL;

            char *x_str = strtok(NULL, " ");
            if(x_str == NULL) {
                printf("Eroare: %d: Nu s-a putut citi argumentul x al comenzii PASTE!\n", line_num);
                return_code = CMD_ERR_CODE;
                goto quit;
            }
            size_t x = strtoumax(x_str, &end, 10);
            if(x_str == end || *end != '\0') {
                printf("Eroare: %d: Argumentul x trebuie sa fie un numar!\n", line_num);
                return_code = CMD_ERR_CODE;
                goto quit;
            }

            char *y_str = strtok(NULL, " ");
            if(y_str == NULL) {
                printf("Eroare: %d: Nu s-a putut citi argumentul y al comenzii PASTE!\n", line_num);
                return_code = CMD_ERR_CODE;
                goto quit;
            }
            size_t y = strtoumax(y_str, &end, 10);
            if(y_str == end || *end != '\0') {
                printf("Eroare: %d: Argumentul y trebuie sa fie un numar!\n", line_num);
                return_code = CMD_ERR_CODE;
                goto quit;
            }

            switch (paste(tag1, tag2, x, y)) {
            case 0:
                break;
            case TAG_ERR_CODE:
                printf("Eroare: %d: Nu exista imagine/i cu tag/-urile!\n", line_num);
                return_code = TAG_ERR_CODE;
                goto quit;
            default:
                printf("Eroare: %d: Comanda PASTE nu a putut rula!\n", line_num);
                return_code = UNK_ERR_CODE;
                goto quit;
            }
        }
        else if(strcmp(command, "SAVE") == 0) {
            /*
            /   Comanda SAVE
            */
            char *tag = strtok(NULL, " ");
            if(tag == NULL) {
                printf("Eroare: %d: Nu s-a putut citi argumentul tag al comenzii SAVE!\n", line_num);
                return_code = CMD_ERR_CODE;
                goto quit;
            }

            char *path = strtok(NULL, " ");
            if(path == NULL) {
                printf("Eroare: %d: Nu s-a putut citi argumentul path al comenzii SAVE!\n", line_num);
                return_code = CMD_ERR_CODE;
                goto quit;
            }

            switch (save(tag, path)) {
            case 0:
                break;
            case IO_ERR_CODE:
                printf("Eroare: %d: Nu s-a putut scrie fisierul %s!\n", line_num, path);
                return_code = IO_ERR_CODE;
                goto quit;
            case TAG_ERR_CODE:
                printf("Eroare: %d: Imaginea cu tag-ul %s nu exista!\n", line_num, tag);
                return_code = TAG_ERR_CODE;
                goto quit;
            default:
                printf("Eroare: %d: Comanda SAVE nu a putut rula!\n", line_num);
                return_code = UNK_ERR_CODE;
                goto quit;
            }
        } else {
            /*
            /   Comanda NECUNOSCUTA
            */
            printf("Eroare: %d: Comanda necunoscuta!\n", line_num);
            return_code = CMD_ERR_CODE;
            goto quit;
        }

        line_num++;
    }

    quit:
    fclose(file);
    if(line != NULL) {
        free(line);
    }
    if(images != NULL) {
        for(size_t i = 0; i < images_len; i++) {
            for(size_t j = 0; j < images[i].height; j++) {
                for(size_t k = 0; k < images[i].width; k++) {
                    free(images[i].data[j][k]);
                }
                free(images[i].data[j]);
            }
            free(images[i].data);
            free(images[i].tag);
        }
        free(images);
        images_len = 0;
    }

    return return_code;
}

struct image *find_image_by_tag(const char *tag) {
    struct image *img = NULL;

    for(size_t i = 0; i < images_len; i++) {
        if(strcmp(images[i].tag, tag) == 0) {
            img = &images[i];
        }
    }

    return img;
}

int create(const char *tag, const size_t cols, const size_t rows) {
    if(find_image_by_tag(tag) != NULL) {
        return TAG_ERR_CODE;
    }

    unsigned char ***img_data = malloc(rows * sizeof(char**));
    if(img_data == NULL) {
        return UNK_ERR_CODE;
    }

    for(size_t i = 0; i < rows; i++) {
        img_data[i] = malloc(cols * sizeof(char*));
        if(img_data[i] == NULL) {
            for(size_t j = 0; j < i; j++) {
                free(img_data[j]);
            }
            free(img_data);

            return UNK_ERR_CODE;
        }

        for(size_t j = 0; j < cols; j++) {
            img_data[i][j] = malloc(3 * sizeof(char));
            if(img_data[i][j] == NULL) {
                for(size_t k = 0; k < j; k++) {
                    free(img_data[i][k]);
                }
                for(size_t k = 0; k < i; k++) {
                    for(size_t w = 0; w < cols; w++) {
                        free(img_data[k][w]);
                    }
                    free(img_data[k]);
                }
                free(img_data);

                return UNK_ERR_CODE;
            }
        }
    }

    if(images == NULL) {
        images = malloc(1 * sizeof(struct image));
        if(images == NULL) {
            for(size_t i = 0; i < rows; i++) {
                for(size_t j = 0; j < cols; j++) {
                    free(img_data[i][j]);
                }
                free(img_data[i]);
            }
            free(img_data);

            return UNK_ERR_CODE;
        }
    } else {
        images = realloc(images, (images_len + 1) * sizeof(struct image));
        if(images == NULL) {
            for(size_t i = 0; i < rows; i++) {
                for(size_t j = 0; j < cols; j++) {
                    free(img_data[i][j]);
                }
                free(img_data[i]);
            }
            free(img_data);

            return UNK_ERR_CODE;
        }
    }
    images[images_len].data = img_data;
    images[images_len].tag = strdup(tag);
    images[images_len].width = cols;
    images[images_len].height = rows;
    images_len++;

    return 0;
}

int load(const char *tag, const char *path) {
    if(find_image_by_tag(tag) != NULL) {
        return TAG_ERR_CODE;
    }

    int width, height;

    unsigned char ***img_data = bmp_read(path, &height, &width);
    if(img_data == NULL) {
        return IO_ERR_CODE;
    }

    if(images == NULL) {
        images = malloc(1 * sizeof(struct image));
        if(images == NULL) {
            for(size_t i = 0; i < height; i++) {
                for(size_t j = 0; j < width; j++) {
                    free(img_data[i][j]);
                }
                free(img_data[i]);
            }
            free(img_data);

            return UNK_ERR_CODE;
        }
    } else {
        images = realloc(images, (images_len + 1) * sizeof(struct image));
        if(images == NULL) {
            for(size_t i = 0; i < height; i++) {
                for(size_t j = 0; j < width; j++) {
                    free(img_data[i][j]);
                }
                free(img_data[i]);
            }
            free(img_data);

            return UNK_ERR_CODE;
        }
    }
    images[images_len].data = img_data;
    images[images_len].tag = strdup(tag);
    images[images_len].width = width;
    images[images_len].height = height;
    images_len++;

    return 0;
}

int clear(const char *tag, unsigned char red, unsigned char green, unsigned char blue) {
    struct image *img = find_image_by_tag(tag);
    if(img == NULL) {
        return TAG_ERR_CODE;
    }

    for(size_t row = 0; row < img->height; row++) {
        for(size_t col = 0; col < img->width; col++) {
            img->data[row][col][0] = red;
            img->data[row][col][1] = blue;
            img->data[row][col][2] = green;
        }
    }

    return 0;
}

int paste(const char *tag1, const char *tag2, size_t x, size_t y) {
    struct image *img1 = find_image_by_tag(tag1);
    if(img1 == NULL) {
        return TAG_ERR_CODE;
    }

    struct image *img2 = find_image_by_tag(tag2);
    if(img2 == NULL) {
        return TAG_ERR_CODE;
    }

    for(size_t row = y; row < img2->height && row < img1->height; row++) {
        for(size_t col = x; col < img2->width && col < img1->width; col++) {
            img1->data[row][col][0] = img2->data[row - y][col - x][0];
            img1->data[row][col][1] = img2->data[row - y][col - x][1];
            img1->data[row][col][2] = img2->data[row - y][col - x][2];
        }
    }

    return 0;
}

int rectangle(const char *tag, 
    size_t left, size_t right, size_t top, size_t bottom,
    unsigned char red, unsigned char green, unsigned char blue) {
    struct image *img = find_image_by_tag(tag);
    if(img == NULL) {
        return TAG_ERR_CODE;
    }

    if(left >= img->width || right >= img->width || top >= img->height || bottom >= img->height) {
        return ARG_ERR_CODE;
    }

    for(size_t row = top; row <= bottom; row++) {
        for(size_t col = left; left <= right; left++) {
            img->data[row][col][0] = red;
            img->data[row][col][1] = green;
            img->data[row][col][2] = blue;
        }
    }

    return 0;
}

int save(const char *tag, const char *path) {
    struct image *img = find_image_by_tag(tag);
    if(img == NULL) {
        return TAG_ERR_CODE;
    }

    if(bmp_write(path, img->data, img->height, img->width) == 0) {
        return IO_ERR_CODE;
    }

    return 0;
}

int crop(const char *tag, size_t left, size_t right, size_t top, size_t bottom) {
    struct image *img = find_image_by_tag(tag);
    if(img == NULL) {
        return TAG_ERR_CODE;
    }

    // Scoate ce este deasupra dreptunghiului
    for(size_t row = 0; row < top; row++) {
        for(size_t col = 0; col < img->width; col++) {
            free(img->data[row][col]);
        }
        free(img->data[row]);
    }
    memmove(img->data, img->data + top, (img->height - top) * sizeof(char**));
    img->data = realloc(img->data, (img->height - top) * sizeof(char**));
    img->height = img->height - top;
    bottom = bottom - top;
    top = 0;

    // Scoate ce este sub dreptunghi
    for(size_t row = bottom + 1; row < img->height; row++) {
        for(size_t col = 0; col < img->width; col++) {
            free(img->data[row][col]);
        }
        free(img->data[row]);
    }
    img->data = realloc(img->data, (bottom + 1) * sizeof(char**));
    img->height = bottom + 1;

    // Scoate ce este in stanga dreptunghiului
    for(size_t row = 0; row < img->height; row++) {
        for(size_t col = 0; col < left; col++) {
            free(img->data[row][col]);
        }
        memmove(img->data[row], img->data[row] + left, (img->width - left) * sizeof(char*));
        img->data[row] = realloc(img->data[row], (img->width - left) * sizeof(char*));
    }
    img->width = img->width - left;
    right = right - left;
    left = 0;

    // Scoate ce este in dreapta dreptunghiului
    for(size_t row = 0; row < img->height; row++) {
        for(size_t col = right + 1; col < img->width; col++) {
            free(img->data[row][col]);
        }
        img->data[row] = realloc(img->data[row], (right + 1) * sizeof(char*));
    }
    img->width = right + 1;

    return 0;
}

int grayscale(const char *tag) {
    struct image *img = find_image_by_tag(tag);
    if(img == NULL) {
        return TAG_ERR_CODE;
    }

    int sum;

    for(size_t row = 0; row < img->height; row++) {
        for(size_t col = 0; col < img->width; col++) {
            sum = 0;
            for(int i = 0; i < 3; i++) {
                sum += img->data[row][col][i];
            }
            sum /= 3;
            img->data[row][col][0] = sum;
            img->data[row][col][1] = sum;
            img->data[row][col][2] = sum;
        }
    }

    return 0;
}

int square(const char *tag) {
    struct image *img = find_image_by_tag(tag);
    if(img == NULL) {
        return TAG_ERR_CODE;
    }

    
}