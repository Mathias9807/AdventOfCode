#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <map>

struct vec {
	int x;
	int y;

	vec operator+(const vec& v) {
		return vec{x + v.x, y + v.y};
	}

	vec operator-(const vec& v) {
		return vec{x - v.x, y - v.y};
	}

	vec operator*(const int& s) {
		return vec{x * s, y * s};
	}
};

int width = 0;
int height = 0;
char *map, *backup;

vec robot;

void print_map() {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
			printf("%c", map[y * width + x]);
		printf("\n");
	}
}

void save_map() {
	memcpy(backup, map, width * height * sizeof(char));
}

void restore_map() {
	memcpy(map, backup, width * height * sizeof(char));
}

char lookup(vec v) {
	return map[v.y * width + v.x];
}

char next_dir(FILE *f) {
	char c = fgetc(f);
	while (c == '\n') c = fgetc(f);
	return c;
}

void push_tile(vec from, int n_objs, vec dir) {
	// printf("push(%d, {%d, %d})\n", n_objs, dir.x, dir.y);
	for (int i = n_objs - 1; i >= 0; i--) {
		vec tile = from + dir * i;
		vec target = tile + dir;
		// printf("moving {%d, %d} to {%d, %d}\n", target.x, target.y, tile.x, tile.y);
		map[target.y * width + target.x] = map[tile.y * width + tile.x];
	}
	map[from.y * width + from.x] = '.';
}

void push(int n_objs, vec dir) {
	// printf("push(%d, {%d, %d})\n", n_objs, dir.x, dir.y);
	for (int i = n_objs - 1; i >= 0; i--) {
		vec tile = robot + dir * i;
		vec target = tile + dir;
		// printf("moving {%d, %d} to {%d, %d}\n", target.x, target.y, tile.x, tile.y);
		map[target.y * width + target.x] = map[tile.y * width + tile.x];
	}
	map[robot.y * width + robot.x] = '.';
	robot = robot + dir;
	map[robot.y * width + robot.x] = '@';
}

bool push_crate(vec crate, vec dir) {
	// Try pushing the crate in dir
	
	// Horizontal case
	if (dir.y == 0) {
		vec right_coord = crate + vec{1, 0};

		vec next_vec = (dir.x > 0 ? right_coord : crate) + dir;
		char next_tile = lookup(next_vec);
		if (next_tile == '.') push_tile((dir.x > 0 ? crate : right_coord), 2, dir);
		if (next_tile == '#') return false;
		if (next_tile == '[' || next_tile == ']') {
			// we hit another crate, try pushing this one
			vec new_crate = next_tile == '[' ? next_vec : (next_vec - vec{1, 0});
			if (push_crate(new_crate, dir)) {
				push_tile((dir.x > 0 ? crate : right_coord), 2, dir);
			}else return false;
		}

	// Vertical case
	}else if (dir.x == 0) {
		// Early-check if left or right side is blocked
		if (lookup(crate + vec{0, 0} + dir) == '#') return false;
		if (lookup(crate + vec{1, 0} + dir) == '#') return false;

		for (int i = 0; i < 2; i++) {
			vec next_vec = crate + vec{i, 0} + dir;
			char next = lookup(next_vec);
			if (next == '.') continue;
			if (next == '#') return false;
			if (next == '[' || next == ']') {
				vec new_crate = next_vec;
				if (next == ']') new_crate.x--;

				if (!push_crate(new_crate, dir))
					return false;
			}
		}

		push_tile(crate + vec{0, 0}, 1, dir);
		push_tile(crate + vec{1, 0}, 1, dir);
	}

	return true;
}

bool move_dir(char dir_char) {
	// Take dir vector
	std::map<char, vec> dir_map{{'v', vec{0, 1}}, {'>', vec{1, 0}},
		{'<', vec{-1, 0}}, {'^', vec{0, -1}}};
	vec dir = dir_map[dir_char];

	if (dir.x == 0 && dir.y == 0) printf("Unknown dir!!! (%c)\n", dir_char);

	char next_tile = lookup(robot + dir);

	// See if next tile in dir is free, if it is, move
	if (next_tile == '.') {
		push(1, dir);
		return true;
	}

	// If not free, check if wall is blocking and if so break;
	if (next_tile == '#') {
		return false;
	}

	// If crate is blocking, try pushing it
	if (next_tile == '[' || next_tile == ']') {
		save_map();
		if (push_crate(robot + dir - vec{next_tile==']', 0}, dir))
			push(1, dir);
		else restore_map();
	}

	return true;
}

int main() {
	FILE *f = fopen("input.txt", "r");

	// Read map
	while (fgetc(f) != '\n') width++;
	rewind(f);

	char c;
	bool saw_nl = false;
	for (int i = 0; true; i++)
		if ((c = fgetc(f)) == '\n') {
			if (saw_nl) {
				height = i / width - 1;
				break;
			}else saw_nl = true;
		}else saw_nl = false;
	rewind(f);
	width *= 2;

	map = (char*) calloc(width * height, 1);
	backup = (char*) calloc(width * height, 1);
	printf("Width: %d, Height: %d\n", width, height);

	int x = 0, y = 0;
	while (true) {
		char c = fgetc(f);
		if (c == '\n') {
			y++;
			x = 0;
			if (fgetc(f) == '\n') break;

			fseek(f, -1, SEEK_CUR);
			continue;
		}

		if (c == '@') robot = vec{x, y};

		printf("Writing to (%d, %d) and (%d, %d)\n", x, y, x+1, y);
		switch (c) {
		case '.':
			map[y * width + x++] = c;
			map[y * width + x++] = c;
			break;
		case 'O':
			map[y * width + x++] = '[';
			map[y * width + x++] = ']';
			break;
		case '@':
			map[y * width + x++] = '@';
			map[y * width + x++] = '.';
			break;
		case '#':
			map[y * width + x++] = '#';
			map[y * width + x++] = '#';
			break;
		}
	}
	
	print_map();

	while (!feof(f)) {
		// Perform one step
		char dir = next_dir(f);
		if (dir == EOF) break;
		printf("Next_dir %c\n", dir);

		while (move_dir(dir) && false) {
			// printf("moved ");
			print_map();
			usleep(4E9);
		}
		// printf("\n");

		// print_map();
		// usleep(3E5);
	}

	int sum = 0;
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			if (map[y * width + x] == '[')
				sum += 100 * y + x;
	printf("Sum: %d\n", sum);

	return 0;
}
