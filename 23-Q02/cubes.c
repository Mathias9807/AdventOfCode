#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define INPUT_BUF 5000

regex_t game_re, cset_re, color_re;

typedef struct {
	int red;
	int green;
	int blue;
} CubeSet;

int strToInt(const char* str, size_t len) {
	char newStr[8] = { 0 };
	memcpy(newStr, str, MIN(len, 7));
	return atoi(newStr);
}

CubeSet superSet(CubeSet a, CubeSet b) {
	a.red	= MAX(a.red, b.red);
	a.green	= MAX(a.green, b.green);
	a.blue	= MAX(a.blue, b.blue);

	return a;
}

bool isSubSet(CubeSet smallSet, CubeSet superSet) {
	if (smallSet.red   > superSet.red)   return false;
	if (smallSet.green > superSet.green) return false;
	if (smallSet.blue  > superSet.blue)  return false;

	return true;
}

CubeSet parseSet(const char* setStr) {
	CubeSet set = {0};
	const char* endOfSet = strchr(setStr, ';');
	if (!endOfSet) endOfSet = strchr(setStr, 0);

	for (int i = 0; i < 3; i++) { // Loop through each possible color in the set (up to three)
		regmatch_t matches[3];
		regexec(&color_re, setStr, 3, matches, 0);
		if (matches[0].rm_so == -1 || setStr + matches[1].rm_so > endOfSet) break; // Stop if we've parsed all colors
		if (setStr[matches[2].rm_so] == 'r') set.red   = strToInt(setStr + matches[1].rm_so, matches[1].rm_eo - matches[1].rm_so);
		if (setStr[matches[2].rm_so] == 'g') set.green = strToInt(setStr + matches[1].rm_so, matches[1].rm_eo - matches[1].rm_so);
		if (setStr[matches[2].rm_so] == 'b') set.blue  = strToInt(setStr + matches[1].rm_so, matches[1].rm_eo - matches[1].rm_so);

		// Move pointer past current element
		setStr += matches[0].rm_eo;
	}
	return set;
}

CubeSet parseLargestSet(const char* line) {
	CubeSet largeSet = {0};
	// Split line into set descriptors
	while (true) {
		regmatch_t matches[2];
		regexec(&cset_re, line, 2, matches, 0);
		if (matches[0].rm_so == -1) break;

		CubeSet nextSet = parseSet(line);

		// Merge new set with previous superset
		largeSet = superSet(largeSet, nextSet);

		line += matches[0].rm_eo + 1;
		if (!*line) break;
	}
	// Return largest superset
	return largeSet;
}

int main() {
	int reti = regcomp(&game_re, "^Game ([0-9]+):", REG_EXTENDED | REG_ICASE | REG_NEWLINE);
	if (reti) {
	    fprintf(stderr, "Could not compile regex\n");
	    exit(1);
	}
	// Finds the first set in a game
	reti = regcomp(&cset_re, "([0-9]+ [0-9a-z ,]+)", REG_EXTENDED | REG_NEWLINE);
	if (reti) {
	    fprintf(stderr, "Could not compile regex\n");
	    exit(1);
	}
	reti = regcomp(&color_re, "([0-9]+) (r|g|b)", REG_EXTENDED | REG_NEWLINE);
	if (reti) {
	    fprintf(stderr, "Could not compile regex\n");
	    exit(1);
	}

	CubeSet elfSet = { .red = 12, .green = 13, .blue = 14 };

	int result = 0;

	// For each line (each game)
	char* line = calloc(1, INPUT_BUF);
	while (fgets(line, INPUT_BUF, stdin)) {
		// Parse game ID
		regmatch_t matches[4];
		regexec(&game_re, line, 4, matches, 0);
		// happy path coding leads to sad time coding
		int gameID = strToInt(line + matches[1].rm_so, matches[1].rm_eo - matches[1].rm_so);

		// Extract largest guaranteed set from line
		CubeSet superSet = parseLargestSet(line);

		// Check if passes elf's test
		if (isSubSet(superSet, elfSet))
			result += gameID;
	}

	printf("Result: %d\n", result);

	free(line);
	return 0;
}

