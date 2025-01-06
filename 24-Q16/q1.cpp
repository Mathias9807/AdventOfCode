#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

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

	bool operator==(const vec& v) {
		return x == v.x && y == v.y;
	}

	vec rotate() {
		return {-y, x};
	}

	string to_str() {
		std::ostringstream stream;
		stream << "vec{";
		stream << x << ", " << y;
		stream << "}";
		return stream.str();
	}
};

int width = 0;
int height = 0;
char *map;

vec S, E;

struct node {
	vec p;
	int dir;
	int cost;
	node *origin;

	node(vec p, int dir, int cost, node *o) : p{p}, dir{dir}, cost{cost}, origin{o} {}
};

vector<node*> candidates;
vector<node*> checked;

void print_map() {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
			printf("%c", map[y * width + x]);
		printf("\n");
	}
}

char lookup(vec v) {
	return map[v.y * width + v.x];
}

// Cost to rotate from dir to dir
int rot_cost(int from, int to) {
	int local_rot = abs((to - from) % 4);
	return local_rot == 3 ? 1 : local_rot;
}

void add_neighbours(node *n) {
	vec dir = {1, 0};
	for (int i = 0; i < 4; i++, dir = dir.rotate()) {
		char candidate = lookup(n->p + dir);
		if (candidate == '.' || candidate == 'E') {
			int cost = n->cost + 1 + rot_cost(n->dir, i) * 1000;
			node *neigh = new node(n->p + dir, i, cost, n);
			// printf("Found neighbour: %s\n", neigh->p.to_str().c_str());
			candidates.push_back(neigh);
			map[neigh->p.y * width + neigh->p.x] = 'o';
		}
	}
}

void print_candidates() {
	for (auto n : candidates) {
		printf("Candidate node: {%s, %d, %d, %ld}\n", n->p.to_str().c_str(), n->dir, n->cost, (long) (void*) n->origin);
	}
}

int main() {
	FILE *f = fopen("input.txt", "r");

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

	// A* algorithm
	// Start by adding all candidate nodes
	node S_node = {S, 0, 0, nullptr};
	add_neighbours(&S_node);

	while (candidates.size() != 0) {
		// print_candidates();

		// Sort the candidates by highest score
		sort(candidates.begin(), candidates.end(), [](const node *a, const node *b) {
			return a->cost > b->cost;
		});
		// printf("sorted\n");

		// print_candidates();

		// Evaluate cheapest node (last node)

		node& n = *candidates.back();
		// Check if any is the goal node, then end
		// (Might not be accurate, there could be a faster node we haven't found yet)
		// printf("n.p: %s, E: %s\n", n.p.to_str().c_str(), E.to_str().c_str());
		if (n.p == E) {
			print_map();
			printf("Found path, cost: %d\n", n.cost);
			exit(0);
		}

		// If not the goal, add to checked and add neighbours as candidates
		checked.push_back(&n);
		candidates.pop_back();
		add_neighbours(&n);
		map[n.p.y * width + n.p.x] = 'x';

		// print_candidates();

		// print_map();
		// fflush(stdout);
		usleep(1E3);
	}

	// while (!feof(f)) {
	// 	// Perform one step
	// 	char dir = next_dir(f);
	// 	if (dir == EOF) break;
	// 	printf("Next_dir %c\n", dir);

	// 	while (move_dir(dir) && false) {
	// 		// printf("moved ");
	// 		print_map();
	// 		usleep(4E9);
	// 	}
	// 	// printf("\n");

	// 	// print_map();
	// 	// usleep(3E5);
	// }

	// int sum = 0;
	// for (int y = 0; y < height; y++)
	// 	for (int x = 0; x < width; x++)
	// 		if (map[y * width + x] == '[')
	// 			sum += 100 * y + x;
	// printf("Sum: %d\n", sum);

	return 0;
}
