#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

int width = 0;
int height = 0;
char *map;

struct vec {
	int x;
	int y;

	vec operator+(const vec& v) {
		return vec{x + v.x, y + v.y};
	}

	void operator+=(const vec& v) {
		x += v.x;
		y += v.y;
	}

	vec operator-(const vec& v) {
		return vec{x - v.x, y - v.y};
	}

	vec operator*(const int& s) {
		return vec{x * s, y * s};
	}

	bool const operator==(const vec& v) const {
		return x == v.x && y == v.y;
	}

	bool const operator!=(const vec& v) const {
		return x != v.x || y != v.y;
	}

	bool const operator<(const vec& v) const {
		return (x + y * width) < (v.x + v.y * width);
	}

	vec rotate() {
		return {-y, x};
	}

	std::string to_str() {
		std::ostringstream stream;
		stream << "vec{";
		stream << x << ", " << y;
		stream << "}";
		return stream.str();
	}

	static vec from_dir(int dir) {
		vec v = {1, 0};
		for (int i = 0; i < dir; i++)
			v = v.rotate();
		return v;
	}
};

vec S, E;

char map_get(vec v) {
	return map[v.y * width + v.x];
}

void print_map() {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
			printf("%c", map[y * width + x]);
		printf("\n");
	}
}

struct loc;

struct path {
	int cost;
	loc* dest;
};

struct loc {
	vec p;
	int dir;
	path left, right, straight;
};

std::map<std::pair<vec, int>, loc> graph;

void add_node(vec p, int dir) {
	loc& main = graph[{ p, dir }];
	if (main.p == vec{0, 0}) {
		main = loc{p, dir};
	}

	int left_dir = (dir + 1) % 4;
	loc& left = graph[{ p, left_dir }];
	if (graph[{ p, left_dir }].p == vec{0, 0}) {
		left = loc{p, left_dir};
		main.left = path{1000, &left};
		add_node(p, left_dir);
	}else {
		main.left = path{1000, &left};
	}

	int right_dir = (dir + 3) % 4;
	loc& right = graph[{ p, right_dir }];
	if (graph[{ p, right_dir }].p == vec{0, 0}) {
		right = loc{p, right_dir};
		main.right = path{1000, &right};
		add_node(p, right_dir);
	}else {
		main.right = path{1000, &right};
	}

	if (main.straight.dest != nullptr) return;

	// Add the straight dir
	// Walk forward until we hit an intersection
	vec walk = p;
	vec v_dir = vec::from_dir(dir);
	while (true) {
		walk += v_dir;
		main.straight.cost++;
		if (map_get(walk) == '#') break;
		printf("(%s, %d) walked to %s\n", p.to_str().c_str(), dir, walk.to_str().c_str());
		vec l_side = walk + vec::from_dir((dir + 1) % 4);
		vec r_side = walk + vec::from_dir((dir + 3) % 4);
		if (map_get(l_side) == '.' || map_get(r_side) == '.') {
			// Handle intersection
			main.straight.dest = &graph[{walk, dir}];
			add_node(walk, dir);
			break;
		}
	}
}

std::vector<vec> visited;
std::map<loc*, int> cache;
int cheapest_path(vec start, int dir, int cum_cost) {
	int min = 999999999;
	loc& l = graph[{ start, dir }];

	int& cached = cache[&l];
	if (cached) return cached;

	// Don't continue down this path if it's already been visited
	if (std::find(visited.begin(), visited.end(), start) != visited.end())
		return 999999999;

	if (cum_cost < 50000) {
		for (int i = 0; i < visited.size(); i++)
			printf(" ");
		printf("cheapest_path: %s, %d, %d\n", start.to_str().c_str(), dir, cum_cost);
	}
	if (start == E) {
		min = cum_cost;
		goto leave;
	}

	if (cum_cost > 110000) {
		min = 9999999;
		goto leave;
	}

	visited.push_back(start);
	if (l.left.dest && l.left.dest->straight.dest) {
		loc& turn_left = *l.left.dest;
		int cost = cheapest_path(turn_left.straight.dest->p, turn_left.dir, cum_cost + l.left.cost + turn_left.straight.cost);
		if (cost < min) min = cost;
	}
	if (l.right.dest && l.right.dest->straight.dest) {
		loc& turn_right = *l.right.dest;
		int cost = cheapest_path(turn_right.straight.dest->p, turn_right.dir, cum_cost + l.right.cost + turn_right.straight.cost);
		if (cost < min) min = cost;
	}
	if (l.straight.dest) {
		int cost = cheapest_path(l.straight.dest->p, dir, cum_cost + l.straight.cost);
		if (cost < min) min = cost;
	}
	visited.pop_back();

	if (min < 999999 && (cached == 0 || cached > min)) {
		cached = min;
	}

leave:
	return min;
}

int main() {
	FILE *f = fopen("input-4.txt", "r");

	// Read map
	while (fgetc(f) != '\n') width++;
	rewind(f);

	char c;
	for (int i = 0; true; i++)
		if (fgetc(f) == EOF) {
			height = i / width - 1;
			break;
		}
	rewind(f);

	map = (char*) calloc(width * height, 1);
	printf("Width: %d, Height: %d\n", width, height);

	int x = 0, y = 0;
	while (!feof(f)) {
		char c = fgetc(f);
		if (c == '\n') {
			y++;
			x = 0;
			if (fgetc(f) == '\n' || feof(f)) break;

			fseek(f, -1, SEEK_CUR);
			continue;
		}

		if (c == 'S') S = vec{x, y};
		if (c == 'E') E = vec{x, y};

		map[y * width + x++] = c;
	}
	
	print_map();

	printf("Mapping path from S\n");
	add_node(S, 0);
	for (auto pair : graph) {
		loc l = pair.second;
		printf("<loc p=%s d=%d>\n", l.p.to_str().c_str(), l.dir);
		map[l.p.y * width + l.p.x] = 'x';
	}

	print_map();

	printf("Lowest cost: %d\n", cheapest_path(S, 0, 0));

	return 0;
}
