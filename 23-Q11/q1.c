#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define DIM_SIZE ((size_t) 256)

typedef struct {
	int x;
	int y;
} Point;

int main() {
	char* map = malloc(DIM_SIZE * DIM_SIZE);

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

		// If entire line is dots, copy it
		bool dots = true;
		for (int i = 0; i < len; i++) 
			if (line[i] == '#') dots = false;

		if (dots) memcpy(&map[++y * DIM_SIZE], line, len);

		map[y * DIM_SIZE + len] = 0;

		free(line);
		line = NULL;
		len = 0;
		y++;
	}
	size_t yLen = y;

	for (int x = 0, X = 0; x < xLen; x++, X++) {
		bool dots = true;
		printf("x: %d\n", X);
		for (y = 0; y < yLen; y++)
			if (map[y * DIM_SIZE + x] == '#') dots = false;

		if (dots) {
			printf("Dots! %s\n", &map[y * DIM_SIZE + x]);
			for (y = 0; y < yLen; y++)
				memcpy(&map[y * DIM_SIZE + x + 1],
					&map[y * DIM_SIZE + x], xLen - x);
			x++;
			xLen++;
		}
	}

	for (int i = 0; i < yLen; i++)
		printf("> %s\n", &map[i * DIM_SIZE]);

	Point* list = calloc(256, sizeof(Point));
	size_t listLen = 0;
	for (int y = 0; y < yLen; y++)
		for (int x = 0; x < xLen; x++)
			if (map[y * DIM_SIZE + x] == '#')
				list[listLen++] = (Point) {.x = x, .y = y};
	printf("List len: %d\n", listLen);

	int sum = 0;
	int pairs = 0;
	for (int i = 0; i < listLen; i++) {
		// For each element, loop through every element after it to get all pairs
		for (int j = i + 1; j < listLen; j++) {
			sum += abs(list[i].x - list[j].x) + abs(list[i].y - list[j].y);
			printf("Distance between %d and %d is %d\n", i+1, j+1, abs(list[i].x - list[j].x) + abs(list[i].y - list[j].y));
			pairs++;
		}
	}
	printf("Pairs: %d\n", pairs);
	printf("Sum: %d\n", sum);

	return 0;
}

