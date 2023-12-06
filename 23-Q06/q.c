#include <stdio.h>
#include <math.h>

int main() {
	// Test input
	// const double t[] = {7, 15, 30};
	// const double d[] = {9, 40, 200};
	// 6A
	// const double t[] = {61, 70, 90, 66};
	// const double d[] = {643, 1184, 1362, 1041};
	// 6B test input
	// const double t[] = {71530};
	// const double d[] = {940200};
	// 6B
	const double t[] = {61709066};
	const double d[] = {643118413621041};

	int nRaces = sizeof(t) / sizeof(double);

	int product = 1;
	for (int i = 0; i < nRaces; i++) {
		// nLösningar = {
		// 	IF t²/4 < d: 0
		// 	IF t²/4 = d och t/2 inte är jämt: 0
		// 	Annars: 1 + floor(t/2 + sqrt(t²/4 - d)) - ceil(t/2 - sqrt(t²/4 - d))
		// }

		// Use d+1 because we need to actually beat the record, not just match it
		double sqrtExpr = t[i] * t[i] / 4 - (d[i] + 1);
		if (sqrtExpr < 0) {
			product *= 0;
			continue;
		}
		if (sqrtExpr == 0 && t[i] / 2 == 0.0) {
			product *= 0;
			continue;
		}

		product *= 1 + floor(t[i]/2 + sqrt(sqrtExpr)) - ceil(t[i]/2 - sqrt(sqrtExpr));
		printf("%f to %f\n", ceil(t[i]/2 - sqrt(sqrtExpr)), floor(t[i]/2 + sqrt(sqrtExpr)));
	}

	printf("Answer: %d\n", product);
}

