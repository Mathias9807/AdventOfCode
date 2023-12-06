#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define MAX_N 100

typedef struct {
	unsigned int start;
	unsigned int len;
	bool moved;
} Range;

Range numbers[MAX_N];
Range tmpBuffer[MAX_N];
size_t nNums = 0;

char trash[MAX_N];

bool isDigit(int n) {
	return n >= '0' && n <= '9';
}

char readNextNumber(char nextChar, unsigned* num) {
	*num = 0;
	do {
		*num = *num * 10 + (nextChar - '0');
	} while (isDigit(nextChar = fgetc(stdin)));
	return nextChar;
}

/** Transforms the given subrange of a range of numbers by some offset. May create new ranges if a range is split */
void transformRange(size_t rgIndex, Range transformRg, unsigned offset) {
	// There are multiple cases that need to be handled here. The ranges may be disjoint, partly overlapping, or either may completely contain the other

	// Is transformRg even inside our range? Return if not
	if (transformRg.start + transformRg.len <= numbers[rgIndex].start || transformRg.start >= numbers[rgIndex].start + numbers[rgIndex].len)
		return;

	// Modify transformRg to become a subset of our range
	if (transformRg.start < numbers[rgIndex].start) { // Remove preceeding numbers not in our range
		unsigned diff = numbers[rgIndex].start - transformRg.start;
		transformRg.start += diff;
		transformRg.len -= diff;
	}
	if (transformRg.start + transformRg.len > numbers[rgIndex].start + numbers[rgIndex].len) { // Remove succeeding numbers not in our range
		unsigned diff = transformRg.start + transformRg.len - (numbers[rgIndex].start + numbers[rgIndex].len);
		transformRg.len -= diff;
	}

	// Here, transformRg is a subset of our range but we don't know if it's actually equal or only partly overlapping
	
	// If the transform doesn't start at the beginning of our range, cut off the start into it's own new range and resize our range to start at the transform
	if (transformRg.start > numbers[rgIndex].start) {
		size_t newRg = nNums++;
		unsigned diff = transformRg.start - numbers[rgIndex].start;
		numbers[newRg].start = numbers[rgIndex].start;
		numbers[newRg].len = diff;
		numbers[rgIndex].start += diff;
		numbers[rgIndex].len -= diff;
	}

	// If the transform doesn't end at the end of our range, cut off the end into it's own new range and resize our range to end at the transform
	if (transformRg.start + transformRg.len < numbers[rgIndex].start + numbers[rgIndex].len) {
		size_t newRg = nNums++;
		unsigned diff = numbers[rgIndex].start + numbers[rgIndex].len - (transformRg.start + transformRg.len);
		numbers[newRg].start = transformRg.start + transformRg.len;
		numbers[newRg].len = diff;
		numbers[rgIndex].len -= diff;
	}

	// The transform range is now exactly equal to our range! The offset can be applied immediately
	numbers[rgIndex].start += offset;
	numbers[rgIndex].moved = true; // Mark moved so it isn't transformed again this round
}

/** Reads a single map and applies each rule in turn to the numbers array. If no more maps exist, returns false */
bool transformNumbers() {
	if (fgets(trash, MAX_N, stdin) == NULL) return false;
	printf("Section %s", trash);

	for (int i = 0; i < nNums; i++)
		numbers[i].moved = false;

	char firstChar = 0; // Need to keep track of double \n's
	while ((firstChar = fgetc(stdin)) != '\n' && firstChar != -1) {
		unsigned dest = 0, src = 0, len = 0;
		readNextNumber(firstChar, &dest);
		readNextNumber(fgetc(stdin), &src);
		readNextNumber(fgetc(stdin), &len);

		Range transformRg = { .start = src, .len = len, .moved = false };
		unsigned offs = dest - src;

		for (int i = 0; i < nNums; i++) {
			if (numbers[i].moved == false)
				transformRange(i, transformRg, offs);
		}
		printf("\tApplied rule %u,%u,%u\n\t\tNumbers: ", dest, src, len);
		for (int i = 0; i < nNums; i++) printf("start:%u len:%u ", numbers[i].start, numbers[i].len);
		printf("\n");
	}

	printf("\n");
	return true;
}

int main() {
	char nextChar = 0;
	while (!isDigit(nextChar = fgetc(stdin))) ; // Skip "seeds: "

	// Parse each seed range
	while (nextChar != '\n') {
		numbers[nNums].moved = false;
		readNextNumber(nextChar, &numbers[nNums].start);
		readNextNumber(fgetc(stdin), &numbers[nNums].len);

		nNums++;
		nextChar = fgetc(stdin);
	}
	fgetc(stdin); // Skip final newline

	while (transformNumbers()) ;

	long sum = 0;
	for (int i = 0; i < nNums; i++) {
		sum += numbers[i].len;
	}
	printf("Sum of lengths: %ld\n", sum);

	unsigned long lowest = (unsigned long) -1;
	for (int i = 0; i < nNums; i++)
		if (numbers[i].start < lowest) lowest = numbers[i].start;

	printf("Answer: %lu\n", lowest);

	return 0;
}

