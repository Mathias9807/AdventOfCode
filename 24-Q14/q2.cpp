#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <chrono>
#include <thread>
#include <string.h>

using namespace std;


// int width = 11, height = 7;
int width = 101, height = 103;

class Vec;

double dot(Vec a, Vec b);

class Vec {
public:
	int x, y;

	Vec() : x{0}, y{0} {
	}

	Vec(int x, int y) : x{x}, y{y} {
	}

	Vec operator+(Vec const& v) {
		return Vec(x + v.x, y + v.y);
	}

	Vec operator-(Vec const& v) {
		return Vec(x - v.x, y - v.y);
	}

	Vec operator*(int const& s) {
		return Vec(s * x, s * y);
	}

	Vec operator/(Vec const& v) {
		return Vec(x / v.x, y / v.y);
	}

	bool operator==(Vec const& v) {
		return x == v.x && y == v.y;
	}

	bool operator!=(Vec const& v) {
		return x != v.x || y != v.y;
	}

	// double colinear(Vec const& v) {
	// 	return dot(Vec(-y, x).normalized(), v);
	// }

	double length() {
		return sqrt(x * x + y * y);
	}

	// Vec normalized() {
	// 	double len = length();
	// 	return Vec(x / len, y / len);
	// }
};

inline int positive_modulo(int i, int n) {
    return (i % n + n) % n;
}

// double dot(Vec a, Vec b) {
// 	return a.x * b.x + a.y * b.y;
// }
// 
// double cross(Vec a, Vec b) {
// 	return a.x * b.y - a.y * b.x;
// }

struct robot {
	Vec p;
	Vec v;
};

void print(vector<robot> robots) {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int n = 0;
			for (robot r : robots) n += r.p == Vec(x, y);
			printf("%c", n == 0 ? ' ' : '#');
		}
		printf("\n");
		// fflush(stdout);
	}
}

int main() {
	FILE *f = fopen("input.txt", "r");
	vector<robot> robots;

	while (!feof(f)) {
		robot r;
		fscanf(f, "p=%d,%d v=%d,%d\n", &r.p.x, &r.p.y, &r.v.x, &r.v.y);
		robots.push_back(r);
	}

	for (auto r : robots)
		printf("Robot p:%d,%d v:%d,%d\n", r.p.x, r.p.y, r.v.x, r.v.y);
	print(robots);

	char* map = (char*) calloc((width + 1) * height, sizeof(char));

	short *rows = (short*) calloc(height, sizeof(short));
	short *columns = (short*) calloc(width, sizeof(short));

	for (int i = 0; i < 1000000; i++) {
		if (i % 1000 == 0) fprintf(stderr, "%d cycles\n", i);

		memset(map, ' ', (width + 1) * height);
		for (int y = 0; y < height; y++) map[y * (width + 1) + width] = 0;

		memset(rows, 0, height * sizeof(short));
		memset(columns, 0, width * sizeof(short));

		for (auto &r : robots) {
			r.p.x = positive_modulo(r.p.x + r.v.x, width);
			r.p.y = positive_modulo(r.p.y + r.v.y, height);

			map[r.p.y * (width + 1) + r.p.x] = '#';
			rows[r.p.y]++;
			columns[r.p.x]++;
		}

		bool signal = false;
		short trigger = 31;
		for (int y = 0; y < height; y++)
			if (rows[y] > trigger) {
				// Are they in a continuous line?
				int x = 0;
			retry:
				for (; map[y * (width + 1) + x] != '#'; x++) ;

				int start_x = x;
				for (; x - start_x < 31; x++) {
					if (map[y * (width + 1) + x] != '#') {
						if (x >= width - 30) goto fail;

						goto retry;
					}
				}

				signal = true;
			fail:
				continue;
			}

		// for (int x = 0; x < width; x++) printf("col %d: %d\n", x, columns[x]);

		if (signal) {
			printf("After %d seconds\n", i + 1);
			// for (auto r : robots)
			// 	printf("Robot p:%d,%d v:%d,%d\n", r.p.x, r.p.y, r.v.x, r.v.y);
			// print(robots);
			for (int y = 0; y < height; y++)
				printf("%s\n", map + (y * (width + 1)));

			// std::this_thread::sleep_for(std::chrono::milliseconds(250));
		}
	}

	return 0;
}
