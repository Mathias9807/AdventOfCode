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

int region_borders(short* regions, int region) {
	int borders = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int dx = 1, dy = 0;
			for (int side = 0; side < 4; side++) {
				int nx = x + dx, ny = y + dy;
				if (regions[y*width+x] == region && regions[ny*width+nx] != region) {
					borders++;
				}

			next:
				int tmp = dx;
				dx = -dy;
				dy = tmp;
			}
		}
	}
	return borders;
}

bool already_a_side(bool *h_sides, bool *v_sides, int side, int x, int y, int dir, int region) {
	int dx = 1, dy = 0;
	for (int s = 0; s < side; s++) {
		int tmp = dx;
		dx = -dy;
		dy = tmp;
	}

	for (int i = 1; i < width + height; i++) {
		int n = i * dir;
		int c_x = x - dy * n;
		int c_y = y + dx * n;
		int coord = c_x + c_y * width;

		int c_n_x = x - dy * n + dx;
		int c_n_y = y + dx * n + dy;
		int n_coord = c_n_x + c_n_y * width;

		printf("\t\tChecking %d,%d\n", c_x, c_y);
		if (c_x < 0 || c_x >= width) break;
		if (c_y < 0 || c_y >= height) break;
		if (regions[coord] != region) break;

		if (c_n_x >= 0 && c_n_x < width && c_n_y >= 0 && c_n_y < height) {
			if (regions[n_coord] == region) break;
		}
		printf("\t\tStill a side\n");

		if (side % 2 == 0) { // Horizontal side
			if (h_sides[c_y * (width+1) + c_x + (side == 0)]) return true;
		}else { // Vertical side
			if (v_sides[c_x * (height+1) + c_y + (side == 1)]) return true;
		}

	}

	return false;
}

int region_sides(short* regions, int region) {
	printf("region_sides: region %d\n", region);
	// Same as region_borders except we should keep track of every match and see if we've already counted the same side
	bool *h_sides = (bool*) calloc((width + 1) * height, sizeof(bool));
	bool *v_sides = (bool*) calloc(width * (height + 1), sizeof(bool));
	int borders = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			printf("\tcoord: %d, %d\n", x, y);
			int dx = 1, dy = 0;
			for (int side = 0; side < 4; side++) {
				int nx = x + dx, ny = y + dy;

				bool in_region = regions[y*width+x] == region;

				bool at_border = false;
				if (nx < 0 || ny < 0 || nx >= width || ny >= height)
					at_border = true;
				else if (regions[ny*width+nx] != region)
					at_border = true;

				if (in_region && at_border) {
					printf("\tFound border (side %d)\n", side);
					// Check if we've seen this side before by shuffling along it
					if (already_a_side(h_sides, v_sides, side, x, y, 1, region)
						|| already_a_side(h_sides, v_sides, side, x, y, -1, region))
						goto next;

					// If not, increment and mark side
					printf("\t\t\tFound new side\n");
					borders++;
					if (side % 2 == 0) { // Horizontal side
						h_sides[y * (width+1) + x + (side == 0)] = true;
					}else { // Vertical side
						v_sides[x * (height+1) + y + (side == 1)] = true;
					}
				}

			next:
				int tmp = dx;
				dx = -dy;
				dy = tmp;
			}
		}
	}
	free(h_sides);
	free(v_sides);
	return borders;
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
	short *sides = (short*) calloc(REGIONS, sizeof(short));

	// Sum area
	// for (int i = 0; i < width * height; i++)
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			area[regions[y*width + x]]++;
		// area[regions[i]]++;

	// Count perimeter. (area * 4 - 2 * shared edges)
	for (int i = 1; i < next_region; i++)
		perim[i] = area[i] * 4 - 2 * shared_borders(regions, i);

	// Count sides
	for (int i = 1; i < next_region; i++)
		sides[i] = region_sides(regions, i);

	for (int i = 1; i < REGIONS; i++)
		if (area[i] != 0)
			printf("Area of region %d: %d\n", i, area[i]);

	for (int i = 1; i < REGIONS; i++)
		if (sides[i] != 0) {
			printf("Sides of region %d (%c): %d (area: %d, shared: %d, perimeter: %d)\n", i, map[plantMap[i]], sides[i], area[i], shared_borders(regions, i), perim[i]);

			// for (int y = 0; y < height; y++) {
			// 	for (int x = 0; x < width; x++) {
			// 		printf("%c", regions[y*width + x] == i ? 'X' : ' ');
			// 	}
			// 	printf("\n");
			// }
		}
	int price = 0;
	for (int i = 1; i < REGIONS; i++)
		if (perim[i] != 0)
			price += area[i] * sides[i];

	printf("Total price: %d\n", price);

	free(map);
	free(regions);
	free(area);
	free(perim);
	return 0;
}
