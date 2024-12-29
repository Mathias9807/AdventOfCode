#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

// Table of total banana count for every 4 number sequence
// Stored in a 4d array with 0-18 ranged values
// Sum for -2,5,0,-3 is in index 19^3(-2 + 9) + 19^2(5 + 9) + 19(0 + 9) + (-3 + 9)
uint32_t *table;

uint32_t next_secret(int secret) {
	uint32_t limit = (uint32_t) -1 >> 8;
	secret = (secret ^ (secret * 64)) & limit;
	secret = (secret ^ (secret / 32)) & limit;
	secret = (secret ^ (secret * 2048)) & limit;
	return secret;
}

uint32_t nth_secret(uint32_t start, size_t n) {
	for (size_t i = 0; i < n; i++)
		start = next_secret(start);
	return start;
}

int query(int d, int c, int b, int a) {
	return table[19*19*19*(d + 9) + 19*19*(c + 9) + 19*(b + 9) + (a + 9)];
}

int main() {
	FILE *in = fopen("input.txt", "r");
	int n_apes = 0;
	while (!feof(in))
		if (fgetc(in) == '\n') n_apes++;
	rewind(in);

	uint32_t *start_nums = calloc(n_apes, sizeof(uint32_t));
	for (int i = 0; i < n_apes; i++)
		fscanf(in, "%d\n", &start_nums[i]);
	fclose(in);

	table = calloc(19*19*19*19, sizeof(uint32_t));
	printf("Allocated %lu\n", 19*19*19*19 * sizeof(uint32_t));

	char *seenFlags = calloc(19*19*19*19, sizeof(char));
	for (int i = 0; i < n_apes; i++) {
		memset(seenFlags, 0, 19*19*19*19*sizeof(char));

		// For every 4 number sequence
		uint32_t e = start_nums[i];
		uint32_t d = next_secret(e);
		uint32_t c = next_secret(d);
		uint32_t b = next_secret(c);
		uint32_t a = next_secret(b);
		for (int j = 4; j < 2000; j++) {
			int D = (d % 10) - (e % 10);
			int C = (c % 10) - (d % 10);
			int B = (b % 10) - (c % 10);
			int A = (a % 10) - (b % 10);

			// Add this value to its corresponding entry
			int index = 19*19*19*(D + 9) + 19*19*(C + 9) + 19*(B + 9) + (A + 9);
			if (!seenFlags[index]) {
				table[index] += a % 10;
				seenFlags[index] = true;
				if (D == -2 && C == 1 && B == -1 && A == 3)
					printf("Ape %d, seq: %u,%u,%u,%u, diffs: %d,%d,%d,%d: %u\n", i, d, c, b, a, D, C, B, A, a % 10);
			}

			e = d;
			d = c;
			c = b;
			b = a;
			a = next_secret(a);
		}
	}
	uint32_t highest = 0;
	for (int a = 0; a < 19; a++)
		for (int b = 0; b < 19; b++)
			for (int c = 0; c < 19; c++)
				for (int d = 0; d < 19; d++)
					if (table[19*19*19*d + 19*19*c + 19*b + a] > table[highest])
						highest = 19*19*19*d + 19*19*c + 19*b + a;
	int a = highest % 19;
	int b = (highest / 19) % 19;
	int c = (highest / 19 / 19) % 19;
	int d = (highest / 19 / 19 / 19) % 19;
	printf("Best sequence is: %d,%d,%d,%d at %u\n", d-9, c-9, b-9, a-9, table[highest]);

	return 0;
}
