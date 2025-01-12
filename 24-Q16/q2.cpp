#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>
#include <utility>

int width = 0;
int height = 0;

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

	bool operator!=(const vec& v) {
		return x != v.x || y != v.y;
	}

	bool operator<(const vec& v) const {
		return (y * width + x) < (v.y * width + v.x);
	}

	bool operator>(const vec& v) const {
		return (y * width + x) > (v.y * width + v.x);
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
};

char *map;

vec S, E;

struct node {
	vec p;
	int dir;
	int cost;
	std::vector<node*> origin;

	node(vec p, int dir, int cost, std::vector<node*> o);
};
std::map<std::pair<vec, int>, node*> node_map;

node::node(vec p, int dir, int cost, std::vector<node*> o) : p{p}, dir{dir}, cost{cost}, origin{o} {
	node_map[{p, dir}] = this;
};

std::vector<node*> candidates;
std::vector<node*> checked;

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

node* node_lookup(vec v, int dir) {
	return node_map[{v, dir}];
}

// Cost to rotate from dir to dir
int rot_cost(int from, int to) {
	int local_rot = abs((to - from) % 4);
	return local_rot == 3 ? 1 : local_rot;
}

void add_neighbours(node *n) {
	// Take step neighbour
	int i = n->dir;
	vec dir = {1, 0};
	for (int j = 0; j < i; j++, dir = dir.rotate()) ;
	vec can_pos = n->p + dir;
	node* can_node = node_lookup(can_pos, i);
	char candidate = lookup(can_pos);

	int cost = n->cost + 1;

	// If we're looking at an already checked node, see if the new cost
	// is equal to the previous one. Then add this as an alternate route
	if (can_node) {
		printf("re-hit a node we've already added {%s} from dir %d, new cost %d, old cost %d\n",
				can_pos.to_str().c_str(), i, cost,
				node_map[{can_pos, i}]->cost);
		if (cost == can_node->cost) {
			node_map[{can_pos, i}]->origin.push_back(n);
		}

	}else if (candidate == '.' || candidate == 'E') {
		node *neigh = new node(can_pos, i, cost, {n});
		// printf("Found neighbour: %s\n", neigh->p.to_str().c_str());
		candidates.push_back(neigh);
		if (candidate != 'E')
			map[neigh->p.y * width + neigh->p.x] = 'o';
	}

	// Rotate in-place neighbour
	dir = {1, 0};
	for (int i = 0; i < 4; i++, dir = dir.rotate()) {
		if (i == n->dir) continue; // Skip the direction we're staring at

		vec can_pos = n->p;
		vec look_pos = n->p + dir;
		node* can_node = node_lookup(can_pos, i);
		char candidate = lookup(look_pos);

		int cost = n->cost + rot_cost(n->dir, i) * 1000;

		// If we're looking at an already checked node, see if the new cost
		// is equal to the previous one. Then add this as an alternate route
		if (can_node) {
			printf("re-hit a node we've already added {%s} from dir %d, new cost %d, old cost %d\n",
					can_pos.to_str().c_str(), i, cost,
					node_map[{can_pos, i}]->cost);
			if (cost == can_node->cost) {
				node_map[{can_pos, i}]->origin.push_back(n);
			}

		}else if (candidate == '.' || candidate == 'E') {
			node *neigh = new node(can_pos, i, cost, {n});
			// printf("Found neighbour: %s\n", neigh->p.to_str().c_str());
			candidates.push_back(neigh);
			if (candidate != 'E')
				map[neigh->p.y * width + neigh->p.x] = 'o';
		}
	}
}

void print_candidates() {
	for (auto n : candidates) {
		printf("Candidate node: {%s, %d, %d, %ld}\n", n->p.to_str().c_str(), n->dir, n->cost, (long) (void*) n->origin.size());
	}
}

int reverse_cost(node *from, node *to) {
	// Reverse sum up the cost of the path, if we turned around +1000
	int turn_penalty = 0;
	if (from->dir != to->dir) turn_penalty += 1000;
	if (from->dir == -to->dir) turn_penalty += 1000;
	return 1 + turn_penalty;
}

void mark_path(node *n, char** path, char** passes) {
	printf("Node %ld: p: {%s}\n", (long) n, n->p.to_str().c_str());
	while (n->p != S) {
		path[n->p.y][n->p.x] = 'X';
		passes[n->p.y][n->p.x]++;

		node *old_n = n;
		n = n->origin[0];
		printf("	number of origin nodes: %ld (first node: %ld)\n", n->origin.size(), (long) n->origin[0]);
		for (auto m = n->origin.begin() + 1; m < n->origin.end(); m++) {
			mark_path(*m, path, passes);
		}
		printf("Node %ld: p: {%s}\n", (long) n, n->p.to_str().c_str());

		// if (old_n->p == n->p) passes[old_n->p.y][old_n->p.x]--;
	}
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

	// A* algorithm
	// Start by adding all candidate nodes
	node S_node = {S, 0, 0, {}};
	add_neighbours(&S_node);

	std::vector<node*> best_paths;

	while (candidates.size() != 0) {
		print_candidates();

		// Sort the candidates by highest score
		sort(candidates.begin(), candidates.end(), [](const node *a, const node *b) {
			return a->cost > b->cost;
		});
		printf("sorted\n");

		print_candidates();

		// Evaluate cheapest node (last node)

		node& n = *candidates.back();
		// If we've already found a path to the goal but this node has a higher cost, exit
		// if (best_paths.size() > 0 && n.cost > best_paths[0]->cost)
		// 	break;

		// Check if any is the goal node regardless of direction, then end
		// (Might not be accurate, there could be a faster node we haven't found yet)
		// printf("n.p: %s, E: %s\n", n.p.to_str().c_str(), E.to_str().c_str());
		if (n.p == E) {
			print_map();
			printf("Found path, cost: %d\n", n.cost);
			best_paths.push_back(&n);
			candidates.pop_back();
			checked.push_back(&n);
			continue;
		}

		// If not the goal, add to checked and add neighbours as candidates
		checked.push_back(&n);
		candidates.pop_back();
		add_neighbours(&n);
		map[n.p.y * width + n.p.x] = 'x';

		printf("Processed best candidate: {%s, %d, %d, %ld}\n", n.p.to_str().c_str(), n.dir, n.cost, (long) (void*) n.origin.size());
		print_candidates();

		print_map();
		fflush(stdout);
		usleep(1E4);
	}

	printf("%ld paths\n", best_paths.size());
	for (auto n : best_paths)
		printf("%d score\n", n->cost);

	// Trace the winning nodes backwards to see where they walked
	printf("Node S %ld: p: {%s}\n", (long) &S_node, S_node.p.to_str().c_str());
	printf("Vec E {%s}\n", E.to_str().c_str());
	char* passes[height];
	for (int y = 0; y < height; y++) passes[y] = (char*) calloc(width, 1);
	for (auto end : best_paths) {
		char* path[height];
		for (int y = 0; y < height; y++) {
			path[y] = (char*) calloc(width + 1, 1);
			memset(path[y], ' ', width);
		}

		node *n = end;
		mark_path(n, path, passes);

		// printf("Path of cost %d\n", cost);
		for (int y = 0; y < height; y++) {
			printf("%s\n", path[y]);
			free(path[y]);
		}
	}

	int n_tiles = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			printf("%d", passes[y][x]);
			if (passes[y][x] > 0) n_tiles++;
		}
		printf("\n");
	}

	printf("\n%d tiles are part of at least one of the best paths\n", n_tiles);

	return 0;
}
