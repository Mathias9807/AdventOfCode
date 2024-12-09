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

	int cur_id = id - 1;
	while (true) {
		for (i = 0; i < n_blocks; i++) {
			if (blocks[i] == -1) printf(".");
			else printf("%d", blocks[i]);
		}
		printf("\n");

		int file_index = n_blocks - 1;
		for (; blocks[file_index] != cur_id; file_index--) ;

		int file_size = 0;
		for (; blocks[file_index] == cur_id; file_index--, file_size++) ;
		printf("file index: %d, file size: %d\n", file_index, file_size);

		int space_index = 0;
		while (space_index < file_index) {
			for (int i = space_index; i < space_index + file_size; i++)
				if (blocks[i] != -1)
					goto try_next;

			for (int i = 0; i < file_size; i++) {
				blocks[space_index + i] = cur_id;
				blocks[file_index + i + 1] = -1;
			}
			goto next_file;

		try_next:
			space_index++;
		}
	next_file:

		cur_id--;
		if (cur_id < 0) break;
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
