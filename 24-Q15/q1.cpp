#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <map>

struct vec {
	int x;
	int y;

	vec operator+(const vec& v) {
		return vec{x + v.x, y + v.y};
	}

	vec operator*(const int& s) {
		return vec{x * s, y * s};
	}
};

int width = 0;
int height = 0;
char *map;

vec robot;

void print_map() {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
			printf("%c", map[y * width + x]);
		printf("\n");
	}
}

char lookup(vec v) {
	return map[v.y * height + v.x];
}

char next_dir(FILE *f) {
	char c = fgetc(f);
	while (c == '\n') c = fgetc(f);
	return c;
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

bool move_dir(char dir_char) {
	// Take dir vector
	std::map<char, vec> dir_map{{'v', vec{0, 1}}, {'>', vec{1, 0}},
		{'<', vec{-1, 0}}, {'^', vec{0, -1}}};
	vec dir = dir_map[dir_char];

	if (dir.x == 0 && dir.y == 0) printf("Unknown dir!!! (%c)\n", dir_char);

	int n_objs = 1;
	while (true) {
		// See if next tile in dir is free, if it is, move
		if (lookup(robot + dir * n_objs) == '.') {
			push(n_objs, dir);
			break;
		}

		// If not free, check if wall is blocking and if so break;
		if (lookup(robot + dir * n_objs) == '#') {
			return false;
		}

		// If crate is blocking, check what's beyond the crate
		if (lookup(robot + dir * n_objs) == 'O') {
			n_objs++;
		}
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

	map = (char*) calloc(width * height, 1);

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

		map[y * width + x++] = c;
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
		// usleep(4E4);
	}

	int sum = 0;
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			if (map[y * width + x] == 'O')
				sum += 100 * y + x;
	printf("Sum: %d\n", sum);

	return 0;
}
