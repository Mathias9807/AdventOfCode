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

vector<double> close_calls;
vector<double> x_s;
vector<double> a_x_s;

int checkCase(FILE *f) {
	int ax, ay, bx, by, px, py;
	fscanf(f, "Button A: X+%d, Y+%d\n", &ax, &ay);
	fscanf(f, "Button B: X+%d, Y+%d\n", &bx, &by);
	fscanf(f, "Prize: X=%d, Y=%d\n", &px, &py);

	Vec a(ax, ay), b(bx, by), prize(px, py);
	// printf("A: {%f, %f}, B: {%f, %f}, Prize: {%f, %f}\n", a.x, a.y, b.x, b.y, prize.x, prize.y);

	// printf("if b = 1, a = %f\n", a(1.0, prize, a, b));

	// printf("Colinearity of A: %f\n", a.colinear(prize));
	// printf("a.x / b.x: %f\n", a.x / b.x);
	// printf("a.y / b.y: %f\n", a.y / b.y);
	// printf("B adjustment factor: %f\n", dot(Vec(-a.y, a.x), b) / a.colinear(prize));

	double x = -cross(a, prize) / cross(b, a);
	// printf("A colinearity adjustment x=%f\n", x);

	double aligned_target = dot(a.normalized(), prize - b * x);
	double a_x = aligned_target / a.length();
	// printf("A factor a_x=%f\n", a_x);

	x_s.push_back(x);
	a_x_s.push_back(a_x);

	if (ax * round(a_x) + bx * round(x) != px || ay * round(a_x) + by * round(x) != py || abs(a_x - round(a_x)) > 0.01 || x < 0 || a_x < 0) {
		// printf("Not integer\n");
		close_calls.push_back(abs(x - round(x)));
		return -1;
	}else {
		// printf("T - xB dot A: %f\n", dot(a.normalized(), (prize - b * x).normalized()));
		// printf("|T - xB| / |A| = %f\n", (prize - b * x).length() / a.length());
		// printf("Found a_x=%f, b_x=%f.\na_x a + b_x b =\t\t{%f, %f}.\nTarget =\t\t{%f, %f}\n", a_x, x, (a * a_x + b * x).x, (a * a_x + b * x).y, prize.x, prize.y);
		return 3 * round(a_x) + round(x);
	}
}

int main() {
	FILE *f = fopen("test-input.txt", "r");

	int sum = 0;
	int n_correct = 0;
	while (!feof(f)) {
		int cheapest = checkCase(f);
		if (cheapest == -1) {
			printf("Impossible\n\n");
			continue;
		}

		printf("Possible for %d\n\n", cheapest);
		sum += cheapest;
		n_correct++;
	}

	printf("Total sum: %d\n", sum);
	printf("Total correct: %d\n", n_correct);

	/* std::sort(close_calls.begin(), close_calls.end());
	for (int i = 0; i < 20; i++)
		printf("%f ", close_calls[i]);

	printf("\n");
	for (int i = 0; i < x_s.size(); i++)
		printf("x: %f, a_x: %f\n", x_s[i], a_x_s[i]); */

	return 0;
}
