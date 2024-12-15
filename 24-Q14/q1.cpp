#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

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
			printf("%c", n == 0 ? '.' : n + '0');
		}
		printf("\n");
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

	int time = 100;
	for (auto &r : robots) {
		r.p.x = positive_modulo(r.p.x + r.v.x * time, width);
		r.p.y = positive_modulo(r.p.y + r.v.y * time, height);
	}

	printf("After %d seconds\n", time);
	for (auto r : robots)
		printf("Robot p:%d,%d v:%d,%d\n", r.p.x, r.p.y, r.v.x, r.v.y);
	print(robots);

	long sum[2][2] = {0};
	for (robot r : robots) {
		if (r.p.x < (double) (width - 1) / 2) {
			if (r.p.y < (double) (height - 1) / 2)
				sum[0][0]++;
			else if (r.p.y > (double) (height - 1) / 2)
				sum[0][1]++;
		}else if (r.p.x > (double) (width - 1) / 2) {
			if (r.p.y < (double) (height - 1) / 2)
				sum[1][0]++;
			else if (r.p.y > (double) (height - 1) / 2)
				sum[1][1]++;
		}
	}

	printf("Quadrant %d=%ld\n", 0, sum[0][0]);
	printf("Quadrant %d=%ld\n", 1, sum[1][0]);
	printf("Quadrant %d=%ld\n", 2, sum[0][1]);
	printf("Quadrant %d=%ld\n", 3, sum[1][1]);

	printf("Safety factor: %ld\n", sum[0][0] * sum[1][0] * sum[0][1] * sum[1][1]);

	return 0;
}
