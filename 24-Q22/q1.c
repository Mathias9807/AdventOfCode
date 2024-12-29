#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

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

int main() {
	FILE *in = fopen("input.txt", "r");
	int n_nums = 0;
	while (!feof(in))
		if (fgetc(in) == '\n') n_nums++;
	rewind(in);

	uint32_t *start_nums = calloc(n_nums, sizeof(uint32_t));
	for (int i = 0; i < n_nums; i++)
		fscanf(in, "%d\n", &start_nums[i]);

	uint64_t sum = 0;
	for (int i = 0; i < n_nums; i++) {
		uint32_t secret = nth_secret(start_nums[i], 2000);
		sum += secret;
		printf("2000:th secret from %u: %u\n", start_nums[i], secret);
	}
	printf("Sum: %lu\n", sum);

	return 0;
}
