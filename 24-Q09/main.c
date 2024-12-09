#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main() {
	FILE* in_file = fopen("./test-input.txt", "r");

	int blocks_len = 100000;
	int* blocks = calloc(blocks_len, sizeof(int));
	memset(blocks, -1, blocks_len * sizeof(int));

	int id = 0;
	int block_index = 0;
	while (!feof(in_file)) {
		if (block_index > blocks_len) {
			printf("Block overflow at %d!\n", block_index);
			return -1;
		}
		int n = fgetc(in_file) - '0';
		printf("read: %d\n", n);
		for (int i = 0; i < n; i++)
			blocks[block_index++] = id;

		id++;

		char empty = fgetc(in_file);
		if (empty == '\n' || feof(in_file)) break;
		printf("read empty: %d\n", empty - '0');
		for (int i = 0; i < empty - '0'; i++)
			block_index++;
	}
	int n_blocks = block_index;

	int len = 0, i = 0;
	for (i = 0; i < n_blocks; i++) {
		if (blocks[i] == -1) printf(".");
		else printf("%d", blocks[i]);
	}
	len = i;
	printf("\n");

	int first_space = 0;
	int last_num = len - 1;
	while (true) {
		while (blocks[first_space] != -1 && first_space < last_num) first_space++;

		while (blocks[last_num] == -1 && last_num > first_space) last_num--;

		if (first_space >= last_num) break;

		blocks[first_space] = blocks[last_num];
		blocks[last_num] = -1;

		// for (i = 0; i < n_blocks; i++) {
		// 	if (blocks[i] == -1) printf(".");
		// 	else printf("%d", blocks[i]);
		// }
		// printf("\n");
	}

	for (i = 0; i < n_blocks; i++) {
		if (blocks[i] == -1) printf(".");
		else printf("%d", blocks[i]);
	}
	printf("\n");

	unsigned long checksum = 0;
	for (int i = 0; i < blocks_len; i++)
		if (blocks[i] != -1)
			checksum += blocks[i] * i;
	printf("Checksum: %ld\n", checksum);
}
