#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define MAX_N 500

unsigned int numbers[MAX_N];
unsigned int tmpBuffer[MAX_N];
char trash[MAX_N];

bool isDigit(int n) {
	return n >= '0' && n <= '9';
}

char readNextNumber(char nextChar, long* num) {
	*num = 0;
	do {
		*num = *num * 10 + (nextChar - '0');
	} while (isDigit(nextChar = fgetc(stdin)));
	return nextChar;
}

/** Reads a single map and applies each rule in turn to the numbers array. If no more maps exist, returns false */
bool transformNumbers(size_t nNums) {
	if (fgets(trash, MAX_N, stdin) == NULL) return false;
	printf("Section %s", trash);

	memcpy(tmpBuffer, numbers, nNums * sizeof(unsigned int));

	char firstChar = 0; // Need to keep track of double \n's
	while ((firstChar = fgetc(stdin)) != '\n' && firstChar != -1) {
		long dest = 0, src = 0, len = 0;
		readNextNumber(firstChar, &dest);
		readNextNumber(fgetc(stdin), &src);
		readNextNumber(fgetc(stdin), &len);

		for (int i = 0; i < nNums; i++) {
			if (tmpBuffer[i] == numbers[i] && numbers[i] >= src && numbers[i] < src + len)
				tmpBuffer[i] = numbers[i] + (dest - src);
		}
		printf("\tApplied rule %ld,%ld,%ld\n\t\tNumbers: ", dest, src, len);
		for (int i = 0; i < nNums; i++) printf("%u ", numbers[i]);
		printf("\n\t\tNew numbers: ");
		for (int i = 0; i < nNums; i++) printf("%u ", tmpBuffer[i]);
		printf("\n");
	}
	memcpy(numbers, tmpBuffer, nNums * sizeof(unsigned int));

	printf("\n");
	return true;
}

int main() {
	size_t nNums = 0;

	char nextChar = 0;
	while (!isDigit(nextChar = fgetc(stdin))) ; // Skip "seeds: "

	// Parse each seed number
	while (nextChar != '\n') {
		long n = 0;
		readNextNumber(nextChar, &n);

		numbers[nNums] = n;
		nNums++;
		nextChar = fgetc(stdin);
	}
	fgetc(stdin); // Skip final newline

	while (transformNumbers(nNums)) ;

	unsigned long lowest = (unsigned long) -1;
	for (int i = 0; i < nNums; i++)
		if (numbers[i] < lowest) lowest = numbers[i];

	printf("Answer: %lu\n", lowest);

	return 0;
}

