#include "include/bmp_io_rgb.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const float blur[3][3] = {{0.111111111111f, 0.111111111111f, 0.111111111111f},
                          {0.111111111111f, 0.111111111111f, 0.111111111111f},
                          {0.111111111111f, 0.111111111111f, 0.111111111111f}};

struct point {
	int x, y;
};

void draw_rectangle(unsigned char ***img, struct point from, struct point to,
                    int k);

unsigned char ***crop(unsigned char ***img, int *width, int *height,
                      struct point from, struct point to);

// Transforma imaginea din color in alb-negru
void normalize(unsigned char ***img, int width, int height);

unsigned char ***resize(unsigned char ***img, int *width, int *height);

unsigned char ***conv_2d(unsigned char ***img, int width, int height,
                         float filter[3][3]);

unsigned char compute_filter(unsigned char ***img, int line, int col, int rgb,
                             float filter[3][3]);

int main() {
	int N, M;
	unsigned char ***img = bmp_read("alpaca.bmp", &N, &M);
	if (img == NULL) {
		return 1;
	}

	struct point from = {.x = 280, .y = 30};
	struct point to = {.x = 530, .y = 500};

	// draw_rectangle(img, from, to, 5);
	// crop(img, &M, &N, from, to);
	normalize(img, M, N);
	// img = resize(img, &M, &N);
	// unsigned char ***new_img = conv_2d(img, M, N, blur);
	bmp_write("output.bmp", img, N, M);

	return 0;
}

// From: 280 30
// To: 530 500

void draw_rectangle(unsigned char ***img, struct point from, struct point to,
                    int k) {
	if (k < 1) {
		return;
	}

	if (from.x > to.x || from.y > to.y) {
		return;
	}

	for (int i = from.x; i <= to.x; i++) {
		img[from.y][i][0] = UCHAR_MAX;
		img[from.y][i][1] = 0;
		img[from.y][i][2] = 0;

		img[to.y][i][0] = UCHAR_MAX;
		img[to.y][i][1] = 0;
		img[to.y][i][2] = 0;
	}

	for (int i = from.y; i <= to.y; i++) {
		img[i][from.x][0] = UCHAR_MAX;
		img[i][from.x][1] = 0;
		img[i][from.x][2] = 0;

		img[i][to.x][0] = UCHAR_MAX;
		img[i][to.x][1] = 0;
		img[i][to.x][2] = 0;
	}

	from.x++;
	from.y++;
	to.x--;
	to.y--;
	draw_rectangle(img, from, to, k - 1);
}

unsigned char **crop_line(unsigned char **line, int len, int from, int to) {
	for (int col = 0; col < from; col++) {
		free(line[col]);
	}
	len -= from;
	memmove(&line[0], &line[from], len * sizeof(unsigned char *));
	line = realloc(line, len * sizeof(unsigned char *));
	to -= from;
	from = 0;

	for (int col = to + 1; col < len; col++) {
		free(line[col]);
	}
	len = to + 1;
	line = realloc(line, len * sizeof(unsigned char *));

	return line;
}

unsigned char ***crop(unsigned char ***img, int *width, int *height,
                      struct point from, struct point to) {
	if (from.x > to.x || from.y > to.y) {
		return 0;
	}

	int new_width = *width, new_height = *height;

	// Scoate ce este deasupra dreptunghiului
	for (int line = 0; line < from.y; line++) {
		for (int col = 0; col < new_width; col++) {
			free(img[line][col]);
		}
		free(img[line]);
	}
	new_height = new_height - from.y;
	memmove(&img[0], &img[from.y], new_height * sizeof(unsigned char **));
	img = realloc(img, new_height * sizeof(unsigned char **));
	to.y -= from.y;
	from.y = 0;

	// Scoate ce e sub dreptunghi
	for (int line = to.y + 1; line < new_height; line++) {
		for (int col = 0; col < new_width; col++) {
			free(img[line][col]);
		}
		free(img[line]);
	}
	new_height = to.y + 1;
	img = realloc(img, new_height * sizeof(unsigned char **));

	for (int line = 0; line < new_height; line++) {
		img[line] = crop_line(img[line], new_width, from.x, to.x);
	}
	to.x -= from.x;
	from.x = 0;
	new_width = to.x + 1;

	*width = new_width;
	*height = new_height;

	return img;
}

void normalize(unsigned char ***img, int width, int height) {
	int sum;

	for (int line = 0; line < height; line++) {
		for (int col = 0; col < width; col++) {
			sum = 0;
			for (int i = 0; i < 3; i++) {
				sum += img[line][col][i];
			}
			sum /= 3;
			img[line][col][0] = sum;
			img[line][col][1] = sum;
			img[line][col][2] = sum;
		}
	}
}

unsigned char ***resize(unsigned char ***img, int *width, int *height) {
	int sum_r = 0;
	int sum_g = 0;
	int sum_b = 0;
	for (int col = 0; col < *width; col++) {
		sum_r += img[0][col][0];
		sum_r += img[*height - 1][col][0];
		sum_g += img[0][col][1];
		sum_g += img[*height - 1][col][1];
		sum_b += img[0][col][2];
		sum_b += img[*height - 1][col][2];
	}
	sum_r /= 2 * *width;
	sum_g /= 2 * *width;
	sum_b /= 2 * *width;

	if (*height < *width) {
		img = realloc(img, *width * sizeof(unsigned char **));
		int offset = *width - *height;
		memmove(&img[offset / 2], &img[0], *height * sizeof(unsigned char **));

		for (int line = 0; line < offset / 2; line++) {
			img[line] = malloc(*width * sizeof(unsigned char *));
			for (int col = 0; col < *width; col++) {
				img[line][col] = malloc(3 * sizeof(unsigned char));
				img[line][col][0] = sum_r;
				img[line][col][1] = sum_g;
				img[line][col][2] = sum_b;
			}
		}

		for (int line = *height + offset / 2; line < *width; line++) {
			img[line] = malloc(*width * sizeof(unsigned char *));
			for (int col = 0; col < *width; col++) {
				img[line][col] = malloc(3 * sizeof(unsigned char));
				img[line][col][0] = sum_r;
				img[line][col][1] = sum_g;
				img[line][col][2] = sum_b;
			}
		}

		*height = *width;

		return img;
	}

	return NULL;
}

unsigned char ***conv_2d(unsigned char ***img, int width, int height,
                         float filter[3][3]) {
	unsigned char ***new_img = malloc(height * sizeof(unsigned char **));

	for (int line = 0; line < height; line++) {
		new_img[line] = malloc(width * sizeof(unsigned char *));
		for (int col = 0; col < width; col++) {
			new_img[line][col] = malloc(3 * sizeof(unsigned char));
		}
	}

	for (int line = 1; line < height - 1; line++) {
		for (int col = 1; col < width - 1; col++) {
			for (int i = 0; i < 3; i++) {
				new_img[line][col][i] =
				    compute_filter(img, line, col, i, filter);
			}
		}
	}

	return new_img;
}

unsigned char compute_filter(unsigned char ***img, int line, int col, int rgb,
                             float filter[3][3]) {
	float out = 0.0f;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			out += img[line - 1 + i][col - 1 + j][rgb] * filter[i][j];
		}
	}

	return (unsigned char)out;
}
