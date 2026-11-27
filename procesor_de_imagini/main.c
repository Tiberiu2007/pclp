#include "include/bmp_io_rgb.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 100
#define UNK_ERR_CODE 1
#define ARG_ERR_CODE 2
#define IO_ERR_CODE 3
#define CMD_ERR_CODE 4
#define TAG_ERR_CODE 5

const char USAGE[] = "Utilizare: procesor_de_imagini <file>\n";
const float FILTERS[5][3][3] = {
    {{0.111111111111f, 0.111111111111f, 0.111111111111f},
     {0.111111111111f, 0.111111111111f, 0.111111111111f},
     {0.111111111111f, 0.111111111111f, 0.111111111111f}},
    {{0.0625f, 0.125f, 0.0625f},
     {0.125f, 0.250f, 0.0625f},
     {0.0625f, 0.125f, 0.0625f}},
    {{0.0f, -1.0f, 0.0f}, {-1.0f, 5.0f, -1.0f}, {0.0f, -1.0f, 0.0f}},
    {{0.0f, 1.0f, 0.0f}, {1.0f, -4.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
    {{-2.0f, -1.0f, 0.0f}, {-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 2.0f}}};

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
int clear(const char *tag, unsigned char red, unsigned char green,
          unsigned char blue);
int paste(const char *tag1, const char *tag2, size_t x, size_t y);
int rectangle(const char *tag, size_t left, size_t right, size_t top,
              size_t bottom, unsigned char red, unsigned char green,
              unsigned char blue);
int save(const char *tag, const char *path);
int crop(const char *tag, size_t left, size_t right, size_t top, size_t bottom);
int grayscale(const char *tag);
int square(const char *tag);
int filter(const char *filter_type, const char *tag);
int encode(const char *tag, const char *filepath);
int decode(const char *tag, const char *filepath);

int main(int argc, char **argv) {
	int return_code = 0;

	if (argc < 2) {
		printf(USAGE);
		return ARG_ERR_CODE;
	}

	FILE *file = fopen(argv[1], "r");
	if (file == NULL) {
		printf("Eroare: Nu s-a putut deschide fisierul %s!\n", argv[1]);
		return IO_ERR_CODE;
	}

	char *line = malloc(MAX_LINE_LEN);
	if (line == NULL) {
		fclose(file);
		return UNK_ERR_CODE;
	}

	int line_num = 1;
	while (fgets(line, MAX_LINE_LEN, file) != NULL) {
		if (line[strlen(line) - 1] == '\n') {
			line[strlen(line) - 1] = '\0';
		}

		if (line[0] == '#' || strlen(line) == 0) {
			continue;
		}

		char *command = strtok(line, " ");
		if (command == NULL) {
			printf("Eroare: %d: Nu s-a putut citi comanda!\n", line_num);
			return_code = CMD_ERR_CODE;
			goto quit;
		}

		if (strcmp(command, "CREATE") == 0) {
			/*
			/   Comanda CREATE
			*/
			char *tag = strtok(NULL, " ");
			if (tag == NULL) {
				printf(
				    "Eroare: %d: Nu s-a putut citi argumentul tag al comenzii "
				    "CREATE!\n",
				    line_num);
				return_code = CMD_ERR_CODE;
				goto quit;
			}

			char *end = NULL;

			char *cols_str = strtok(NULL, " ");
			if (cols_str == NULL) {
				printf(
				    "Eroare: %d: Nu s-a putut citi argumentul cols al comenzii "
				    "CREATE!\n",
				    line_num);
				return_code = CMD_ERR_CODE;
				goto quit;
			}
			size_t cols = strtoumax(cols_str, &end, 10);
			if (cols_str == end || *end != '\0') {
				printf("Eroare: %d: Argumentul cols al comenzii CREATE trebuie "
				       "sa fie "
				       "un numar!\n",
				       line_num);
				return_code = CMD_ERR_CODE;
				goto quit;
			}

			char *rows_str = strtok(NULL, " ");
			if (rows_str == NULL) {
				printf(
				    "Eroare: %d: Nu s-a putut citi argumentul rows al comenzii "
				    "CREATE!\n",
				    line_num);
				return_code = CMD_ERR_CODE;
				goto quit;
			}
			size_t rows = strtoumax(rows_str, &end, 10);
			if (rows_str == end || *end != '\0') {
				printf("Eroare: %d: Argumentul rows al comenzii CREATE trebuie "
				       "sa fie "
				       "un numar!\n",
				       line_num);
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
				printf("Eroare: %d: Comanda CREATE nu a putut rula!\n",
				       line_num);
				return_code = UNK_ERR_CODE;
				goto quit;
			}
		} else if (strcmp(command, "LOAD") == 0) {
			/*
			/   Comanda LOAD
			*/
			char *tag = strtok(NULL, " ");
			if (tag == NULL) {
				printf("Eroare: %d: Nu s-a putut citi argumentul tag al "
				       "comenzii LOAD!\n",
				       line_num);
				return_code = CMD_ERR_CODE;
				goto quit;
			}

			char *path = strtok(NULL, " ");
			if (path == NULL) {
				printf("Eroare: %d: Nu s-a putut citi argumentul path al "
				       "comenzii LOAD!\n",
				       line_num);
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
				printf("Eroare: %d: LOAD: Nu s-a putut deschide fisierul %s!\n",
				       line_num, path);
				return_code = IO_ERR_CODE;
				goto quit;
			default:
				printf("Eroare: %d: Comanda LOAD nu a putut rula!\n", line_num);
				return_code = UNK_ERR_CODE;
				goto quit;
			}
		} else if (strcmp(command, "CLEAR") == 0) {
			/*
			/   Comanda CLEAR
			*/
			char *tag = strtok(NULL, " ");
			if (tag == NULL) {
				printf(
				    "Eroare: %d: Nu s-a putut citi argumentul tag al comenzii "
				    "CREATE!\n",
				    line_num);
				return_code = CMD_ERR_CODE;
				goto quit;
			}

			char *end = NULL;
			unsigned char uchar_args[3];
			const char *uchar_arg_names[3] = {"red", "green", "blue"};

			for (int i = 0; i < 3; i++) {
				const char *str = strtok(NULL, " ");
				if (str == NULL) {
					printf("Eroare: %d: Nu s-a putut citi argumentul %s\n",
					       line_num, uchar_arg_names[i]);
					return_code = CMD_ERR_CODE;
					goto quit;
				}
				size_t num = strtoumax(str, &end, 10);
				if (str == end || *end != '\0' || num < 0 || num > 255) {
					printf("Eroare: %d: Argumentul %s trebuie sa fie un numar "
					       "intre 0 si "
					       "255!\n",
					       line_num, uchar_arg_names[i]);
					return_code = CMD_ERR_CODE;
					goto quit;
				}
				uchar_args[i] = num;
			}

			switch (clear(tag, uchar_args[0], uchar_args[1], uchar_args[2])) {
			case 0:
				break;
			case TAG_ERR_CODE:
				printf("Eroare: %d: Imaginea cu tag-ul %s nu exista!\n",
				       line_num, tag);
				return_code = TAG_ERR_CODE;
				goto quit;
			default:
				printf("Eroare: %d: Comanda CREATE nu a putut rula!\n",
				       line_num);
				return_code = UNK_ERR_CODE;
				goto quit;
			}
		} else if (strcmp(command, "PASTE") == 0) {
			/*
			/   Comanda PASTE
			*/
			char *tag1 = strtok(NULL, " ");
			if (tag1 == NULL) {
				printf(
				    "Eroare: %d: Nu s-a putut citi argumentul tag1 al comenzii "
				    "PASTE!\n",
				    line_num);
				return_code = CMD_ERR_CODE;
				goto quit;
			}

			char *tag2 = strtok(NULL, " ");
			if (tag2 == NULL) {
				printf(
				    "Eroare: %d: Nu s-a putut citi argumentul tag2 al comenzii "
				    "PASTE!\n",
				    line_num);
				return_code = CMD_ERR_CODE;
				goto quit;
			}

			char *end = NULL;
			size_t uintmax_args[2];
			const char *uintmax_arg_names[2] = {"x", "y"};

			for (int i = 0; i < 2; i++) {
				const char *str = strtok(NULL, " ");
				if (str == NULL) {
					printf("Eroare: %d: Nu s-a putut citi argumentul %s\n",
					       line_num, uintmax_arg_names[i]);
					return_code = CMD_ERR_CODE;
					goto quit;
				}
				size_t num = strtoumax(str, &end, 10);
				if (str == end || *end != '\0') {
					printf("Eroare: %d: Argumentul %s trebuie sa fie un numar "
					       "natural!\n",
					       line_num, uintmax_arg_names[i]);
					return_code = CMD_ERR_CODE;
					goto quit;
				}
				uintmax_args[i] = num;
			}

			switch (paste(tag1, tag2, uintmax_args[0], uintmax_args[1])) {
			case 0:
				break;
			case TAG_ERR_CODE:
				printf("Eroare: %d: Nu exista imagine/i cu tag/-urile!\n",
				       line_num);
				return_code = TAG_ERR_CODE;
				goto quit;
			default:
				printf("Eroare: %d: Comanda PASTE nu a putut rula!\n",
				       line_num);
				return_code = UNK_ERR_CODE;
				goto quit;
			}
		} else if (strcmp(command, "RECTANGLE") == 0) {
			/*
			/   Comanda RECTANGLE
			*/
			const char *tag = strtok(NULL, " ");
			if (tag == NULL) {
				printf("Eroare: %d: Nu s-a putut citi argumentul tag!\n",
				       line_num);
				return_code = CMD_ERR_CODE;
				goto quit;
			}

			char *end = NULL;
			size_t uintmax_args[4];
			unsigned char uchar_args[3];

			const char *uintmax_arg_names[4] = {"left", "right", "top",
			                                    "bottom"};
			const char *uchar_arg_names[3] = {"red", "green", "blue"};

			for (int i = 0; i < 4; i++) {
				const char *str = strtok(NULL, " ");
				if (str == NULL) {
					printf("Eroare: %d: Nu s-a putut citi argumentul %s\n",
					       line_num, uintmax_arg_names[i]);
					return_code = CMD_ERR_CODE;
					goto quit;
				}
				size_t num = strtoumax(str, &end, 10);
				if (str == end || *end != '\0') {
					printf("Eroare: %d: Argumentul %s trebuie sa fie un numar "
					       "natural!\n",
					       line_num, uintmax_arg_names[i]);
					return_code = CMD_ERR_CODE;
					goto quit;
				}
				uintmax_args[i] = num;
			}

			for (int i = 0; i < 3; i++) {
				const char *str = strtok(NULL, " ");
				if (str == NULL) {
					printf("Eroare: %d: Nu s-a putut citi argumentul %s\n",
					       line_num, uchar_arg_names[i]);
					return_code = CMD_ERR_CODE;
					goto quit;
				}
				size_t num = strtoumax(str, &end, 10);
				if (str == end || *end != '\0' || num < 0 || num > 255) {
					printf("Eroare: %d: Argumentul %s trebuie sa fie un numar "
					       "intre 0 si "
					       "255!\n",
					       line_num, uchar_arg_names[i]);
					return_code = CMD_ERR_CODE;
					goto quit;
				}
				uchar_args[i] = num;
			}

			switch (rectangle(tag, uintmax_args[0], uintmax_args[1],
			                  uintmax_args[2], uintmax_args[3], uchar_args[0],
			                  uchar_args[1], uchar_args[2])) {
			case 0:
				break;
			case ARG_ERR_CODE:
				printf("Eroare: %d: Coordonatele nu sunt corecte raportate la "
				       "marimea "
				       "imaginii\n",
				       line_num);
				return_code = ARG_ERR_CODE;
				goto quit;
			case TAG_ERR_CODE:
				printf("Eroare: %d: Nu exista imagine cu tag-ul %s\n", line_num,
				       tag);
				return_code = TAG_ERR_CODE;
				goto quit;
			default:
				printf("Eroare: %d: Nu s-a putut rula comanda RECTANGLE!\n",
				       line_num);
				return_code = UNK_ERR_CODE;
				goto quit;
			}
		} else if (strcmp(command, "SAVE") == 0) {
			/*
			/   Comanda SAVE
			*/
			char *tag = strtok(NULL, " ");
			if (tag == NULL) {
				printf("Eroare: %d: Nu s-a putut citi argumentul tag al "
				       "comenzii SAVE!\n",
				       line_num);
				return_code = CMD_ERR_CODE;
				goto quit;
			}

			char *path = strtok(NULL, " ");
			if (path == NULL) {
				printf("Eroare: %d: Nu s-a putut citi argumentul path al "
				       "comenzii SAVE!\n",
				       line_num);
				return_code = CMD_ERR_CODE;
				goto quit;
			}

			switch (save(tag, path)) {
			case 0:
				break;
			case IO_ERR_CODE:
				printf("Eroare: %d: Nu s-a putut scrie fisierul %s!\n",
				       line_num, path);
				return_code = IO_ERR_CODE;
				goto quit;
			case TAG_ERR_CODE:
				printf("Eroare: %d: Imaginea cu tag-ul %s nu exista!\n",
				       line_num, tag);
				return_code = TAG_ERR_CODE;
				goto quit;
			default:
				printf("Eroare: %d: Comanda SAVE nu a putut rula!\n", line_num);
				return_code = UNK_ERR_CODE;
				goto quit;
			}
		} else if (strcmp(command, "CROP") == 0) {
			/*
			/   Comanda CROP
			*/
			const char *tag = strtok(NULL, " ");
			if (tag == NULL) {
				printf("Eroare: %d: Nu s-a putut citi argumentul tag!\n",
				       line_num);
				return_code = CMD_ERR_CODE;
				goto quit;
			}

			char *end = NULL;
			size_t uintmax_args[4];

			const char *uintmax_arg_names[4] = {"left", "right", "top",
			                                    "bottom"};

			for (int i = 0; i < 4; i++) {
				const char *str = strtok(NULL, " ");
				if (str == NULL) {
					printf("Eroare: %d: Nu s-a putut citi argumentul %s\n",
					       line_num, uintmax_arg_names[i]);
					return_code = CMD_ERR_CODE;
					goto quit;
				}
				size_t num = strtoumax(str, &end, 10);
				if (str == end || *end != '\0') {
					printf("Eroare: %d: Argumentul %s trebuie sa fie un numar "
					       "natural!\n",
					       line_num, uintmax_arg_names[i]);
					return_code = CMD_ERR_CODE;
					goto quit;
				}
				uintmax_args[i] = num;
			}

			switch (crop(tag, uintmax_args[0], uintmax_args[1], uintmax_args[2],
			             uintmax_args[3])) {
			case 0:
				break;
			case ARG_ERR_CODE:
				printf("Eroare: %d: Coordonatele date nu sunt corecte "
				       "raportate la "
				       "dimensiunile imaginii!\n",
				       line_num);
				return_code = CMD_ERR_CODE;
				goto quit;
			case TAG_ERR_CODE:
				printf("Eroare: %d: Nu exista imagine cu tag-ul %s\n", line_num,
				       tag);
				return_code = TAG_ERR_CODE;
				goto quit;
			default:
				printf("Eroare: %d: Nu s-a putut rula comanda CROP!\n",
				       line_num);
				return_code = UNK_ERR_CODE;
				goto quit;
			}
		} else if (strcmp(command, "GRAYSCALE") == 0) {
			/*
			/   Comanda GRAYSCALE
			*/
			const char *tag = strtok(NULL, " ");
			if (tag == NULL) {
				printf("Eroare: %d: Nu s-a putut citi argumentul tag!\n",
				       line_num);
				return_code = CMD_ERR_CODE;
				goto quit;
			}

			switch (grayscale(tag)) {
			case 0:
				break;
			case TAG_ERR_CODE:
				printf("Eroare: %d: Nu exista imagine cu tag-ul %s\n", line_num,
				       tag);
				return_code = TAG_ERR_CODE;
				goto quit;
			default:
				printf("Eroare: %d: Comanda GRAYSCALE nu a putut rula!\n",
				       line_num);
				return_code = UNK_ERR_CODE;
				goto quit;
			}
		} else if (strcmp(command, "SQUARE") == 0) {
			/*
			/   Comanda SQUARE
			*/
			const char *tag = strtok(NULL, " ");
			if (tag == NULL) {
				printf("Eroare: %d: Nu s-a putut citi argumentul tag!\n",
				       line_num);
				return_code = CMD_ERR_CODE;
				goto quit;
			}

			switch (square(tag)) {
			case 0:
				break;
			case TAG_ERR_CODE:
				printf("Eroare: %d: Nu exista imagine cu tag-ul %s\n", line_num,
				       tag);
				return_code = TAG_ERR_CODE;
				goto quit;
			default:
				printf("Eroare: %d: Comanda SQUARE nu a putut rula!\n",
				       line_num);
				return_code = UNK_ERR_CODE;
				goto quit;
			}
		} else if (strcmp(command, "FILTER") == 0) {
			/*
			 *  Comanda FILTER
			 */
			const char *filter_type = strtok(NULL, " ");
			if (filter_type == NULL) {
				printf(
				    "Eroare: %d: Nu s-a putut citi argumentul filter_type!\n",
				    line_num);
				return_code = CMD_ERR_CODE;
				goto quit;
			}

			const char *tag = strtok(NULL, " ");
			if (tag == NULL) {
				printf("Eroare: %d: Nu s-a putut citi argumentul tag!\n",
				       line_num);
				return_code = CMD_ERR_CODE;
				goto quit;
			}

			switch (filter(filter_type, tag)) {
			case 0:
				break;
			case CMD_ERR_CODE:
				printf("Eroare: %d: Nu exista filtrul %s!\n", line_num,
				       filter_type);
				return_code = CMD_ERR_CODE;
				goto quit;
			case TAG_ERR_CODE:
				printf("Eroare: %d: Nu exista imaginea cu tag-ul %s\n",
				       line_num, tag);
				return_code = TAG_ERR_CODE;
				goto quit;
			default:
				printf("Eroare: %d: Comanda FILTER nu a putut rula!\n",
				       line_num);
				return_code = UNK_ERR_CODE;
				goto quit;
			}
		} else if (strcmp(command, "ENCODE") == 0) {
			/*
			 *  Comanda ENCODE
			 */
			const char *tag = strtok(NULL, " ");
			if (tag == NULL) {
				printf("Eroare: %d: Nu s-a putut citi argumentul tag!\n",
				       line_num);
				return_code = CMD_ERR_CODE;
				goto quit;
			}

			const char *filepath = strtok(NULL, " ");
			if (filepath == NULL) {
				printf("Eroare: %d: Nu s-a putut citi argumentul filepath!\n",
				       line_num);
				return_code = CMD_ERR_CODE;
				goto quit;
			}

			switch (encode(tag, filepath)) {
			case 0:
				break;
			case IO_ERR_CODE:
				printf("Eroare: %d: Nu s-a putut scrie fisierul %s!\n",
				       line_num, filepath);
				return_code = IO_ERR_CODE;
				goto quit;
			case TAG_ERR_CODE:
				printf("Eroare: %d: Nu exista imaginea cu tag-ul %s\n",
				       line_num, tag);
				return_code = TAG_ERR_CODE;
				goto quit;
			default:
				printf("Eroare: %d: Comanda ENCODE nu a putut rula!\n",
				       line_num);
				return_code = UNK_ERR_CODE;
				goto quit;
			}
		} else if (strcmp(command, "DECODE") == 0) {
			/*
			 *  Comanda DECODE
			 */
			const char *tag = strtok(NULL, " ");
			if (tag == NULL) {
				printf("Eroare: %d: Nu s-a putut citi argumentul tag!\n",
				       line_num);
				return_code = CMD_ERR_CODE;
				goto quit;
			}

			const char *filepath = strtok(NULL, " ");
			if (filepath == NULL) {
				printf("Eroare: %d: Nu s-a putut citi argumentul filepath!\n",
				       line_num);
				return_code = CMD_ERR_CODE;
				goto quit;
			}

			switch (decode(tag, filepath)) {
			case 0:
				break;
			case IO_ERR_CODE:
				printf("Eroare: %d: Nu s-a putut citi fisierul %s!\n", line_num,
				       filepath);
				return_code = IO_ERR_CODE;
				goto quit;
			case TAG_ERR_CODE:
				printf("Eroare: %d: Exista deja imagine cu tag-ul %s\n",
				       line_num, tag);
				return_code = TAG_ERR_CODE;
				goto quit;
			default:
				printf("Eroare: %d: Comanda DECODE nu a putut rula!\n",
				       line_num);
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
	if (line != NULL) {
		free(line);
	}
	if (images != NULL) {
		for (size_t i = 0; i < images_len; i++) {
			for (size_t j = 0; j < images[i].height; j++) {
				for (size_t k = 0; k < images[i].width; k++) {
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

	for (size_t i = 0; i < images_len; i++) {
		if (strcmp(images[i].tag, tag) == 0) {
			img = &images[i];
		}
	}

	return img;
}

int create(const char *tag, const size_t cols, const size_t rows) {
	if (find_image_by_tag(tag) != NULL) {
		return TAG_ERR_CODE;
	}

	unsigned char ***img_data = malloc(rows * sizeof(char **));
	if (img_data == NULL) {
		return UNK_ERR_CODE;
	}

	for (size_t i = 0; i < rows; i++) {
		img_data[i] = malloc(cols * sizeof(char *));
		if (img_data[i] == NULL) {
			for (size_t j = 0; j < i; j++) {
				free(img_data[j]);
			}
			free(img_data);

			return UNK_ERR_CODE;
		}

		for (size_t j = 0; j < cols; j++) {
			img_data[i][j] = malloc(3 * sizeof(char));
			if (img_data[i][j] == NULL) {
				for (size_t k = 0; k < j; k++) {
					free(img_data[i][k]);
				}
				for (size_t k = 0; k < i; k++) {
					for (size_t w = 0; w < cols; w++) {
						free(img_data[k][w]);
					}
					free(img_data[k]);
				}
				free(img_data);

				return UNK_ERR_CODE;
			}
		}
	}

	if (images == NULL) {
		images = malloc(1 * sizeof(struct image));
		if (images == NULL) {
			for (size_t i = 0; i < rows; i++) {
				for (size_t j = 0; j < cols; j++) {
					free(img_data[i][j]);
				}
				free(img_data[i]);
			}
			free(img_data);

			return UNK_ERR_CODE;
		}
	} else {
		images = realloc(images, (images_len + 1) * sizeof(struct image));
		if (images == NULL) {
			for (size_t i = 0; i < rows; i++) {
				for (size_t j = 0; j < cols; j++) {
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
	if (find_image_by_tag(tag) != NULL) {
		return TAG_ERR_CODE;
	}

	int width, height;

	unsigned char ***img_data = bmp_read(path, &height, &width);
	if (img_data == NULL) {
		return IO_ERR_CODE;
	}

	if (images == NULL) {
		images = malloc(1 * sizeof(struct image));
		if (images == NULL) {
			for (size_t i = 0; i < height; i++) {
				for (size_t j = 0; j < width; j++) {
					free(img_data[i][j]);
				}
				free(img_data[i]);
			}
			free(img_data);

			return UNK_ERR_CODE;
		}
	} else {
		images = realloc(images, (images_len + 1) * sizeof(struct image));
		if (images == NULL) {
			for (size_t i = 0; i < height; i++) {
				for (size_t j = 0; j < width; j++) {
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

int clear(const char *tag, unsigned char red, unsigned char green,
          unsigned char blue) {
	struct image *img = find_image_by_tag(tag);
	if (img == NULL) {
		return TAG_ERR_CODE;
	}

	for (size_t row = 0; row < img->height; row++) {
		for (size_t col = 0; col < img->width; col++) {
			img->data[row][col][0] = red;
			img->data[row][col][1] = blue;
			img->data[row][col][2] = green;
		}
	}

	return 0;
}

int paste(const char *tag1, const char *tag2, size_t x, size_t y) {
	struct image *img1 = find_image_by_tag(tag1);
	if (img1 == NULL) {
		return TAG_ERR_CODE;
	}

	struct image *img2 = find_image_by_tag(tag2);
	if (img2 == NULL) {
		return TAG_ERR_CODE;
	}

	for (size_t row = y; row < img2->height && row < img1->height; row++) {
		for (size_t col = x; col < img2->width && col < img1->width; col++) {
			img1->data[row][col][0] = img2->data[row - y][col - x][0];
			img1->data[row][col][1] = img2->data[row - y][col - x][1];
			img1->data[row][col][2] = img2->data[row - y][col - x][2];
		}
	}

	return 0;
}

int rectangle(const char *tag, size_t left, size_t right, size_t top,
              size_t bottom, unsigned char red, unsigned char green,
              unsigned char blue) {
	struct image *img = find_image_by_tag(tag);
	if (img == NULL) {
		return TAG_ERR_CODE;
	}

	if (left >= img->width || right >= img->width || top >= img->height ||
	    bottom >= img->height) {
		return ARG_ERR_CODE;
	}

	for (size_t row = top; row <= bottom; row++) {
		for (size_t col = left; col <= right; col++) {
			img->data[row][col][0] = red;
			img->data[row][col][1] = green;
			img->data[row][col][2] = blue;
		}
	}

	return 0;
}

int save(const char *tag, const char *path) {
	struct image *img = find_image_by_tag(tag);
	if (img == NULL) {
		return TAG_ERR_CODE;
	}

	if (bmp_write(path, img->data, img->height, img->width) == 0) {
		return IO_ERR_CODE;
	}

	return 0;
}

int crop(const char *tag, size_t left, size_t right, size_t top,
         size_t bottom) {
	struct image *img = find_image_by_tag(tag);
	if (img == NULL) {
		return TAG_ERR_CODE;
	}

	if (left >= img->width || right >= img->width || top >= img->height ||
	    bottom >= img->height) {
		return ARG_ERR_CODE;
	}
	size_t start_x = left;
	size_t start_y = top;
	size_t end_x = img->width - right;
	size_t end_y = img->height - bottom;

	// Scoate ce este deasupra dreptunghiului
	for (size_t row = 0; row < start_y; row++) {
		for (size_t col = 0; col < img->width; col++) {
			free(img->data[row][col]);
		}
		free(img->data[row]);
	}
	memmove(img->data, img->data + start_y,
	        (img->height - start_y) * sizeof(char **));
	img->data = realloc(img->data, (img->height - start_y) * sizeof(char **));
	img->height = img->height - start_y;
	end_y = end_y - start_y;
	start_y = 0;

	// Scoate ce este sub dreptunghi
	for (size_t row = end_y + 1; row < img->height; row++) {
		for (size_t col = 0; col < img->width; col++) {
			free(img->data[row][col]);
		}
		free(img->data[row]);
	}
	img->data = realloc(img->data, (end_y + 1) * sizeof(char **));
	img->height = end_y + 1;

	// Scoate ce este in stanga dreptunghiului
	for (size_t row = 0; row < img->height; row++) {
		for (size_t col = 0; col < start_x; col++) {
			free(img->data[row][col]);
		}
		memmove(img->data[row], img->data[row] + start_x,
		        (img->width - start_x) * sizeof(char *));
		img->data[row] =
		    realloc(img->data[row], (img->width - start_x) * sizeof(char *));
	}
	img->width = img->width - start_x;
	end_x = end_x - start_x;
	start_x = 0;

	// Scoate ce este in dreapta dreptunghiului
	for (size_t row = 0; row < img->height; row++) {
		for (size_t col = end_x + 1; col < img->width; col++) {
			free(img->data[row][col]);
		}
		img->data[row] = realloc(img->data[row], (end_x + 1) * sizeof(char *));
	}
	img->width = end_x + 1;

	return 0;
}

int grayscale(const char *tag) {
	struct image *img = find_image_by_tag(tag);
	if (img == NULL) {
		return TAG_ERR_CODE;
	}

	int sum;

	for (size_t row = 0; row < img->height; row++) {
		for (size_t col = 0; col < img->width; col++) {
			sum = 0;
			for (int i = 0; i < 3; i++) {
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
	if (img == NULL) {
		return TAG_ERR_CODE;
	}

	if (img->width == img->height) {
		return 0;
	} else if (img->width > img->height) {
		img->data = realloc(img->data, img->width * sizeof(char **));
		size_t dif = (img->width - img->height) / 2;
		memmove(img->data + dif, img->data, img->height * sizeof(char **));

		// Pixeli negri deasupra imaginii
		for (size_t row = 0; row < dif; row++) {
			img->data[row] = malloc(img->width * sizeof(char *));
			for (size_t col = 0; col < img->width; col++) {
				img->data[row][col] = malloc(3 * sizeof(char));
				for (int i = 0; i < 3; i++) {
					img->data[row][col][i] = 0;
				}
			}
		}

		// Pixeli negri sub imagine
		for (size_t row = img->height + dif; row < img->width; row++) {
			img->data[row] = malloc(img->width * sizeof(char *));
			for (size_t col = 0; col < img->width; col++) {
				img->data[row][col] = malloc(3 * sizeof(char));
				for (int i = 0; i < 3; i++) {
					img->data[row][col][i] = 0;
				}
			}
		}

		img->height = img->width;
	} else {
		// TODO
	}

	return 0;
}

int filter(const char *filter_type, const char *tag) {
	struct image *img = find_image_by_tag(tag);
	if (img == NULL) {
		return TAG_ERR_CODE;
	}
	unsigned int filter_index = 0;

	if (strcmp(filter_type, "blur") == 0) {
		filter_index = 0;
	} else if (strcmp(filter_type, "gaussian") == 0) {
		filter_index = 1;
	} else if (strcmp(filter_type, "sharpen") == 0) {
		filter_index = 2;
	} else if (strcmp(filter_type, "laplacian") == 0) {
		filter_index = 3;
	} else if (strcmp(filter_type, "emboss") == 0) {
		filter_index = 4;
	} else {
		return CMD_ERR_CODE;
	}

	// Alocheaza o noua imagine temporara
	unsigned char ***new_img = malloc(img->height * sizeof(char **));
	if (new_img == NULL) {
		return UNK_ERR_CODE;
	}
	for (size_t row = 0; row < img->height; row++) {
		new_img[row] = malloc(img->width * sizeof(char *));
		if (new_img[row] == NULL) {
			for (size_t i = 0; i < row; i++) {
				for (size_t j = 0; j < img->width; j++) {
					free(new_img[i][j]);
				}
				free(new_img[i]);
			}
			free(new_img);
			return UNK_ERR_CODE;
		}

		for (size_t col = 0; col < img->width; col++) {
			new_img[row][col] = malloc(3);
			if (new_img[row][col] == NULL) {
				for (size_t i = 0; i < col; i++) {
					free(new_img[row][i]);
				}
				free(new_img[row]);

				for (size_t i = 0; i < row; i++) {
					for (size_t j = 0; j < img->width; j++) {
						free(new_img[i][j]);
					}
					free(new_img[i]);
				}
				free(new_img);
				return UNK_ERR_CODE;
			}
		}
	}

	for (size_t row = 1; row < img->height - 1; row++) {
		for (size_t col = 1; col < img->width - 1; col++) {
			for (int k = 0; k < 3; k++) {
				float sum = 0.0f;
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						sum += img->data[row + i - 1][col + j - 1][k] *
						       FILTERS[filter_index][i][j];
					}
				}
				if (sum <= 0.0f) {
					new_img[row][col][k] = 0;
				} else if (sum >= 255.0f) {
					new_img[row][col][k] = 255;
				} else {
					new_img[row][col][k] = (unsigned char)sum;
				}
			}
		}
	}

	for (size_t row = 0; row < img->height; row++) {
		for (size_t col = 0; col < img->width; col++) {
			for (int k = 0; k < 3; k++) {
				img->data[row][col][k] = new_img[row][col][k];
			}
			free(new_img[row][col]);
		}
		free(new_img[row]);
	}
	free(new_img);

	return 0;
}

int encode(const char *tag, const char *filepath) {
	int return_code = 0;
	struct image *img = find_image_by_tag(tag);
	if (img == NULL) {
		return TAG_ERR_CODE;
	}

	// Codeaza
	size_t img_size = img->width * img->height * 3;
	unsigned char *encoded_img = malloc(2 * img_size);
	if (encoded_img == NULL) {
		return UNK_ERR_CODE;
	}
	unsigned char pattern = img->data[0][0][0];
	unsigned char pattern_size = 1;
	size_t cur_index = 0;
	for (size_t i = 1; i < img_size; i++) {
		size_t row = i / 3 / img->width % img->height;
		size_t col = i / 3 % img->width;
		size_t k = i % 3;

		if (abs(img->data[row][col][k] - pattern) <= 20 && pattern_size < 255) {
			pattern_size++;
			continue;
		}

		encoded_img[cur_index] = pattern_size;
		encoded_img[cur_index + 1] = pattern;
		pattern_size = 1;
		pattern = img->data[row][col][k];
		cur_index += 2;
	}
	encoded_img[cur_index] = pattern_size;
	encoded_img[cur_index + 1] = pattern;
	cur_index += 2;
	encoded_img = realloc(encoded_img, cur_index);
	if (encoded_img == NULL) {
		free(encoded_img);
		return UNK_ERR_CODE;
	}

	FILE *file = fopen(filepath, "wb");
	if (file == NULL) {
		free(encoded_img);
		return IO_ERR_CODE;
	}

	size_t out = 0;
	out = fwrite(&img->width, sizeof(size_t), 1, file);
	if (out != 1) {
		return_code = UNK_ERR_CODE;
		goto quit;
	}
	out = fwrite(&img->height, sizeof(size_t), 1, file);
	if (out != 1) {
		return_code = UNK_ERR_CODE;
		goto quit;
	}
	out = fwrite(encoded_img, sizeof(char), cur_index, file);
	if (out != cur_index) {
		return_code = UNK_ERR_CODE;
		goto quit;
	}

quit:
	fclose(file);
	free(encoded_img);

	return return_code;
}

int decode(const char *tag, const char *filepath) {
	struct image *img = find_image_by_tag(tag);
	if (img != NULL) {
		return TAG_ERR_CODE;
	}

	FILE *file = fopen(filepath, "rb");
	if (file == NULL) {
		return IO_ERR_CODE;
	}

	size_t out = 0, width, height;
	out = fread(&width, sizeof(size_t), 1, file);
	if (out != 1) {
		fclose(file);
		return UNK_ERR_CODE;
	}
	out = fread(&height, sizeof(size_t), 1, file);
	if (out != 1) {
		fclose(file);
		return UNK_ERR_CODE;
	}

	if (create(tag, width, height) != 0) {
		fclose(file);
		return UNK_ERR_CODE;
	}
	img = find_image_by_tag(tag);
	if (img == NULL) {
		fclose(file);
		return UNK_ERR_CODE;
	}

	// Decodeaza
	unsigned char pattern_size, pattern;
	size_t row = 0, col = 0, k = 0;

	while (fread(&pattern_size, sizeof(char), 1, file) == 1) {
		out = fread(&pattern, sizeof(char), 1, file);
		if (out != 1) {
			fclose(file);
			return UNK_ERR_CODE;
		}

		for (unsigned int i = 0; i < pattern_size; i++) {
			img->data[row][col][k] = pattern;

			k++;
			if (k >= 3) {
				k = 0;
				col++;
				if (col >= img->width) {
					col = 0;
					row++;
				}
			}
		}
	}

	fclose(file);

	return 0;
}
