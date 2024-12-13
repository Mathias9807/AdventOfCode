#include <stdio.h>
#include <stdlib.h>

#define REGIONS 1024

int width, height;

char *map;
short *regions;

void paintbucket(int x, int y, int region) {
	// printf("paintbucket: %d, %d, region=%d\n", x, y, region);
	if (x < 0 || x >= width) return;
	if (y < 0 || y >= height) return;
	regions[y*width + x] = region;

	int dx = 1, dy = 0;
	for (int dir = 0; dir < 4; dir++) {
		int coord = (y + dy) * width + x + dx;
		if (x + dx < 0 || x + dx >= width) goto next;
		if (y + dy < 0 || y + dy >= height) goto next;
		if (regions[coord] == 0 && map[coord] == map[y*width + x])
			paintbucket(x + dx, y + dy, region);

	next:
		int tmp = dx;
		dx = -dy;
		dy = tmp;
	}
}

int neighbouring_region(int x, int y) {
	if (x < 0 || x >= width) return 0;
	if (y < 0 || y >= height) return 0;
	if (regions[y*width + x]) return regions[y*width + x];

	int dx = 1, dy = 0;
	for (int dir = 0; dir < 4; dir++) {
		char plant = map[(y) * width + x];

		int coord = (y + dy) * width + x + dx;
		if (x + dx < 0 || x + dx >= width) goto next;
		if (y + dy < 0 || y + dy >= height) goto next;

		if (map[coord] == plant && regions[coord]) {
			return regions[coord];
		}

	next:
		int tmp = dx;
		dx = -dy;
		dy = tmp;
	}

	return 0;
}

int shared_borders(short* regions, int region) {
	// printf("shared_borders: region %d\n", region);
	// Iterate all borders on the map, first vertical, then horizontal borders
	int borders = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width - 1; x++) {
			if (regions[y*height + x] == region && regions[y*height + x + 1] == region)
				borders++;
		}
	}

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height - 1; y++) {
			if (regions[y*height + x] == region && regions[(y+1)*height + x] == region)
				borders++;
		}
	}
	// printf("\tfound %d borders\n", borders);
	return borders;
}

int main() {
	FILE *f = fopen("input.txt", "r");

	width = 0;
	for (; fgetc(f) != '\n'; width++) ;
	rewind(f);

	fseek(f, 0, SEEK_END);
	height = ftell(f) / (width + 1);
	rewind(f);

	printf("w: %d, h: %d\n", width, height);

	map = (char*) calloc(width * height, sizeof(char));

	int x = 0, y = 0;
	while (!feof(f)) {
		char c = fgetc(f);
		if (c == '\n') {
			x = 0;
			y++;
		}else if (c == EOF) break;
		else map[y * width + x++] = c;
	}
	fclose(f);

	regions = (short*) calloc(width * height, sizeof(short));
	short *plantMap = (short*) calloc(REGIONS, sizeof(short));

	// Convert the plant map to a region map
	int next_region = 1;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int neighbour = neighbouring_region(x, y);
			if (neighbour) {
				regions[y * width + x] = neighbour;
				paintbucket(x, y, neighbour);
			}else {
				regions[y * width + x] = next_region++;
				paintbucket(x, y, next_region - 1);
				plantMap[next_region - 1] = y * width + x;
			}
		}
	}

	// for (int y = 0; y < height; y++) {
	// 	for (int x = 0; x < width; x++)
	// 		printf("%02d ", regions[y * width + x]);
	// 	printf("\n");
	// }

	short *area = (short*) calloc(REGIONS, sizeof(short));
	short *perim = (short*) calloc(REGIONS, sizeof(short));

	// Sum area
	// for (int i = 0; i < width * height; i++)
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			area[regions[y*width + x]]++;
		// area[regions[i]]++;

	// Count perimeter. (area * 4 - 2 * shared edges)
	for (int i = 1; i < next_region; i++)
		perim[i] = area[i] * 4 - 2 * shared_borders(regions, i);

	for (int i = 1; i < REGIONS; i++)
		if (area[i] != 0)
			printf("Area of region %d: %d\n", i, area[i]);

	for (int i = 1; i < REGIONS; i++)
		if (perim[i] != 0) {
			printf("Perimeter of region %d (%c): %d (area: %d, shared: %d)\n", i, map[plantMap[i]], perim[i], area[i], shared_borders(regions, i));

			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					printf("%c", regions[y*width + x] == i ? 'X' : ' ');
				}
				printf("\n");
			}
		}
	int price = 0;
	for (int i = 1; i < REGIONS; i++)
		if (perim[i] != 0)
			price += area[i] * perim[i];

	printf("Total price: %d\n", price);

	free(map);
	free(regions);
	free(area);
	free(perim);
	return 0;
}
