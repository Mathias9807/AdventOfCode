#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define MAX_LINE 1000

typedef struct {
	int x;
	int y;
} coord;

int main() {
	FILE* input = fopen("input.txt", "r");

	fseek(input, 0, SEEK_END);
	size_t in_len = ftell(input);
	fseek(input, 0, SEEK_SET);

	char* data = calloc(in_len + 1, 1);
	char* write_ptr = fgets(data, in_len, input);
	size_t stride = strlen(data);
	int width = stride - 1;
	int height = in_len / stride;

	write_ptr += stride;
	while (fgets(write_ptr, in_len, input)) write_ptr += stride;
	fclose(input);

	char char_types['z' + 1] = {0};
	for (int line = 0; line * stride < in_len; line++)
		for (int x = 0; x < stride - 1; x++)
			if (data[line * stride + x] != '.')
				char_types[data[line * stride + x]]++;

	int highest_freq = 0;
	for (int i = 0; i < 'z' + 1; i++)
		if (char_types[i] > highest_freq)
			highest_freq = char_types[i];

	printf("Num 0's: %d\n", char_types['0']);
	printf("Num A's: %d\n", char_types['A']);

	// A list of all coordinates for all frequencies of signals
	int bucket_size = highest_freq * sizeof(coord);
	coord* buckets = calloc('z' + 1, bucket_size);
	memset(buckets, -1, ('z' + 1) * bucket_size);

	// Save coordinates
	for (int line = 0; line * stride < in_len; line++)
		for (int x = 0; x < stride - 1; x++) {
			char c = data[line * stride + x];
			if (c == '.') continue;

			int i_bucket = c * highest_freq;
			while (buckets[i_bucket].x != -1) i_bucket++;

			buckets[i_bucket].x = x;
			buckets[i_bucket].y = line;
		}

	printf("Coord of first 0: %d, %d\n", buckets['0' * highest_freq + 0].x, buckets['0' * highest_freq + 0].y);
	printf("Coord of second 0: %d, %d\n", buckets['0' * highest_freq + 1].x, buckets['0' * highest_freq + 1].y);

	// For every letter (frequency)
	for (char c = 0; c <= 'z'; c++) {
		// Check for every node
		for (int b_i = 0; b_i < char_types[c]; b_i++) {
			coord node = buckets[c * highest_freq + b_i];

			// And every pair of this node and another of the same frequency
			for (int bb_i = 0; bb_i < char_types[c]; bb_i++) {
				if (b_i == bb_i) continue;
				coord resonant = buckets[c * highest_freq + bb_i];

				// Find smallest n where node + (resonent - node) * n is in bounds
				// Find smallest n where node + (resonent - node) * n >= 0,0
				float left_n_x = (float) -node.x / (resonant.x - node.x);
				float top_n_y = (float) -node.y / (resonant.y - node.y);
				float right_n_x = (float) (width - 1 - node.x) / (resonant.x - node.x);
				float bottom_n_y = (float) (height - 1 - node.y) / (resonant.y - node.y);
				int small_n_x = fmin(left_n_x, right_n_x);
				int small_n_y = fmin(top_n_y, bottom_n_y);
				int smallest_n = fmax(small_n_x, small_n_y);

				int big_n_x = fmax(left_n_x, right_n_x);
				int big_n_y = fmax(top_n_y, bottom_n_y);
				int big_n = fmin(big_n_x, big_n_y);

				for (int n = smallest_n; n < big_n; n++) {
					// if (n == 0 || n == 1) continue;

					// Check if the antinode coord is in bounds
					coord anti = {
						node.x + n * (resonant.x - node.x),
						node.y + n * (resonant.y - node.y)
					};

					printf("Antinode for %c: %d, %d\n", c, anti.x, anti.y);
					data[anti.y * stride + anti.x] = '#';
				}
			}
		}
	}

	printf("%s\n", data);

	int antinodes = 0;
	for (int i = 0; i < in_len; i++)
		if (data[i] == '#')
			antinodes++;

	printf("%d unique antinode locations\n", antinodes);

	return 0;
}
