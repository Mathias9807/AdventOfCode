#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;

class Vec;

double dot(Vec a, Vec b);

class Vec {
public:
	double x, y;

	Vec() : x{0}, y{0} {
	}

	Vec(double x, double y) : x{x}, y{y} {
	}

	Vec operator+(Vec const& v) {
		return Vec(x + v.x, y + v.y);
	}

	Vec operator-(Vec const& v) {
		return Vec(x - v.x, y - v.y);
	}

	Vec operator*(double const& s) {
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

	double colinear(Vec const& v) {
		return dot(Vec(-y, x).normalized(), v);
	}

	double length() {
		return sqrt(x * x + y * y);
	}

	Vec normalized() {
		double len = length();
		return Vec(x / len, y / len);
	}
};

double dot(Vec a, Vec b) {
	return a.x * b.x + a.y * b.y;
}

double cross(Vec a, Vec b) {
	return a.x * b.y - a.y * b.x;
}

// aV + bW = T
// Want to see if T is colinear with either V or W.
// Colinear if dot(rot(V, 90), T) = 0

unsigned long checkCase(FILE *f) {
	long ax, ay, bx, by, px, py;
	fscanf(f, "Button A: X+%ld, Y+%ld\n", &ax, &ay);
	fscanf(f, "Button B: X+%ld, Y+%ld\n", &bx, &by);
	fscanf(f, "Prize: X=%ld, Y=%ld\n", &px, &py);
	px += 10000000000000;
	py += 10000000000000;

	Vec a(ax, ay), b(bx, by), prize(px, py);
	// printf("A: {%f, %f}, B: {%f, %f}, Prize: {%f, %f}\n", a.x, a.y, b.x, b.y, prize.x, prize.y);

	// printf("if b = 1, a = %f\n", a(1.0, prize, a, b));

	// printf("Colinearity of A: %f\n", a.colinear(prize));
	// printf("a.x / b.x: %f\n", a.x / b.x);
	// printf("a.y / b.y: %f\n", a.y / b.y);
	// printf("B adjustment factor: %f\n", dot(Vec(-a.y, a.x), b) / a.colinear(prize));

	double x = -cross(a, prize) / cross(b, a);
	// printf("test 1: x=%f, xl=%ld\n", cross(b, a), (bx * ay - by * ax));
	long xl = -(ax * py - ay * px) / (bx * ay - by * ax);
	printf("A colinearity adjustment x=%f, xl=%ld\n", x, xl);

	double aligned_target = dot(a.normalized(), prize - b * x);
	double a_x = aligned_target / a.length();
	long a_xl = (px - xl * bx) / ax;
	printf("A factor a_x=%ld\n", a_xl);

	if (ax * a_xl + bx * xl != px || ay * a_xl + by * xl != py || xl < 0 || a_xl < 0) {
		// printf("Not integer\n");
		return -1;
	}else {
		// printf("T - xB dot A: %f\n", dot(a.normalized(), (prize - b * x).normalized()));
		// printf("|T - xB| / |A| = %f\n", (prize - b * x).length() / a.length());
		// printf("Found a_x=%f, b_x=%f.\na_x a + b_x b =\t\t{%f, %f}.\nTarget =\t\t{%f, %f}\n", a_x, x, (a * a_x + b * x).x, (a * a_x + b * x).y, prize.x, prize.y);
		return 3 * round(a_x) + round(x);
	}
}

int main() {
	FILE *f = fopen("input.txt", "r");

	unsigned long sum = 0;
	int n_correct = 0;
	while (!feof(f)) {
		unsigned long cheapest = checkCase(f);
		if (cheapest == -1) {
			printf("Impossible\n\n");
			continue;
		}

		printf("Possible for %d\n\n", cheapest);
		sum += cheapest;
		n_correct++;
	}

	printf("Total sum: %ld\n", sum);
	printf("Total correct: %d\n", n_correct);

	return 0;
}
