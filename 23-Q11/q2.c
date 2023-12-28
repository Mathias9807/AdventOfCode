#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define DIM_SIZE ((size_t) 256)

typedef struct {
	long x;
	long y;
} Point;

int main() {
	char* map = malloc(DIM_SIZE * DIM_SIZE);

	long* xGaps = calloc(256, sizeof(long));
	size_t xGapsLen = 0;
	long* yGaps = calloc(256, sizeof(long));
	size_t yGapsLen = 0;

	char* line = NULL;
	size_t len;
	size_t xLen;
	size_t y = 0;
	while (getline(&line, &len, stdin) != -1) {
		if (len >= DIM_SIZE) printf("DIM_SIZE Too Small!\n");

		for (len = 0; line[len] != '\n'; len++) ;
		xLen = len;
		line[len] = 0;

		printf("Line: %s, len=%ld\n", line, len);
		memcpy(&map[y * DIM_SIZE], line, len);

		// If entire line is dots, register gap
		bool dots = true;
		for (int i = 0; i < len; i++) 
			if (line[i] == '#') dots = false;

		if (dots) yGaps[yGapsLen++] = y;

		map[y * DIM_SIZE + len] = 0;

		free(line);
		y++;
		line = NULL;
		len = 0;
	}
	size_t yLen = y;

	for (int x = 0; x < xLen; x++) {
		bool dots = true;
		printf("x: %d\n", x);
		for (y = 0; y < yLen; y++)
			if (map[y * DIM_SIZE + x] == '#') dots = false;

		if (dots) {
			printf("Dots! %s\n", &map[y * DIM_SIZE + x]);
			xGaps[xGapsLen++] = x;
		}
	}

	for (int i = 0; i < yLen; i++)
		printf("> %s\n", &map[i * DIM_SIZE]);

	Point* list = calloc(256, sizeof(Point));
	size_t listLen = 0;
	for (long y = 0; y < yLen; y++)
		for (long x = 0; x < xLen; x++)
			if (map[y * DIM_SIZE + x] == '#') {
				long xWithGaps = x;
				for (int i = 0; i < xGapsLen && xGaps[i] < x; i++)
					xWithGaps += 1000000 - 1;
				long yWithGaps = y;
				for (int i = 0; i < yGapsLen && yGaps[i] < y; i++)
					yWithGaps += 1000000 - 1;
				list[listLen++] = (Point) {.x = xWithGaps, .y = yWithGaps};
			}
	printf("List len: %d\n", listLen);

	long sum = 0;
	int pairs = 0;
	for (int i = 0; i < listLen; i++) {
		// For each element, loop through every element after it to get all pairs
		for (int j = i + 1; j < listLen; j++) {
			sum += labs(list[i].x - list[j].x) + labs(list[i].y - list[j].y);
			// printf("Distance between %d and %d is %d\n", i+1, j+1, abs(list[i].x - list[j].x) + abs(list[i].y - list[j].y));
			pairs++;
		}
	}
	printf("Pairs: %d\n", pairs);
	printf("Sum: %ld\n", sum);

	return 0;
}

