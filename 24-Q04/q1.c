// Search for the word XMAS in a square of text
// XMAS may be arranged in any orientation
// n, -n, n*w, -n*w, n*(w+1), -n*(w+1), n*(w-1), -n*(w-1)

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define LINE_SIZE 200

int matches = 0;

size_t lines, lineSize;

void match(char* lines, int l, int c) {
	char line[LINE_SIZE];
	memcpy(line, &lines[l * lineSize], lineSize);
	printf("l=%d, c=%d, %s\n", l, c, line);

	matches++;
}

bool checkMatch(char* text, int l, int c, int xDiff, int yDiff) {
	if (l + yDiff * 3 < 0) return false;
	if (l + yDiff * 3 >= lines) return false;
	if (c + xDiff * 3 < 0) return false;
	if (c + xDiff * 3 >= lines) return false;

	// for (char letter = 0, step = 0; (letter = text[(l + yDiff * step) * lineSize + step * xDiff]) == "XMAS"[step]; step++)
	// 	if (step == 3) return true;

	char letter = 0;
	char step = 0;
	while (true) {
		letter = text[(l + yDiff * step) * lineSize + c + step * xDiff];

		if (letter != "XMAS"[step]) break;

		if (step == 3) return true;

		step++;
	}

	return false;
}

int main() {
	FILE* f = fopen("./input.txt", "r");

	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);

	lineSize = 0;
	for (char c = 0; (c = fgetc(f)) != '\n' && c != EOF; lineSize++) ;
	printf("Line length: %ld\n", lineSize);
	
	fseek(f, 0, SEEK_SET);
	char* line = calloc(size + 1, sizeof(char));
	for (int l = 0; fgets(line + l*lineSize, lineSize + 1, f); l++) {
		printf("line: %s\n", line + l*lineSize);
		fgetc(f);
	}

	lines = size / lineSize - 1;

	printf("lines = %ld, line length = %ld\n", lines, lineSize);

	for (int l = 0; l < lines; l++) {
		for (int c = 0; c < lineSize; c++) {
			// printf("l=%d, c=%d, %c\n", l, c, line[l * lineSize + c]);

			if (checkMatch(line, l, c, -1, -1)) match(line, l, c);
			if (checkMatch(line, l, c,  0, -1)) match(line, l, c);
			if (checkMatch(line, l, c, +1, -1)) match(line, l, c);

			if (checkMatch(line, l, c, -1,  0)) match(line, l, c);

			if (checkMatch(line, l, c, +1,  0)) match(line, l, c);

			if (checkMatch(line, l, c, -1,  1)) match(line, l, c);
			if (checkMatch(line, l, c,  0,  1)) match(line, l, c);
			if (checkMatch(line, l, c, +1,  1)) match(line, l, c);
		}
		printf("\n");
	}

	printf("# Matches: %d\n", matches);

	free(line);
	return 0;
}
