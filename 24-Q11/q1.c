#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct stone {
	unsigned long n;
	struct stone *next;
} stone;

void applyRules(stone *stones) {
	stone *s = stones;
	while (s) {
		int digits = log10(s->n) + 1;

		if (s->n == 0) {
			s->n = 1;

		}else if (digits % 2 == 0) {
			stone *new_s = calloc(1, sizeof(stone));
			new_s->next = s->next;
			s->next = new_s;

			// printf("split %ld into ", s->n);
			new_s->n = s->n % (int) pow(10, (int) (digits / 2));
			s->n = s->n / (int) pow(10, (int) (digits / 2));
			// printf("%ld and %ld\n", s->n, new_s->n);

			s = new_s;

		}else {
			s->n *= 2024;
		}

		s = s->next;
	}
}

int main() {
	int blinks = 25;

	stone *stones = NULL, *prev_stone = NULL;
	while (!feof(stdin)) {
		unsigned long n = 0;
		if (scanf("%ld", &n) != 1) break;

		stone *s = calloc(1, sizeof(stone));
		s->n = n;
		if (stones)
			prev_stone->next = s;
		else
			stones = s;
		prev_stone = s;
	}

	for (int i = 0; i < blinks; i++) {
		// for (stone *iter = stones; iter; iter = iter->next)
		// 	printf("%ld ", iter->n);
		// printf("\n");

		applyRules(stones);
	}

	// for (stone *iter = stones; iter; iter = iter->next)
	// 	printf("%ld ", iter->n);
	// printf("\n");

	int count = 0;
	for (stone *iter = stones; iter; iter = iter->next) count++;

	printf("Total after %d blinks: %d\n", blinks, count);

	return 0;
}
