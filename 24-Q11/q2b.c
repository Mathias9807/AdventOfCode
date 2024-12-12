#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Räkna antalet stenar som vi får efter n repetitioner av reglerna på start stenarna
//
// Reglerna kan ändra talet som står på stenarna.
// Reglerna kan splitta en sten i två.
// Reglerna kommer aldrig minska antalet stenar.
// Reglerna verkar på stenar individuellt, stenar före eller efter den nuvarande stenen påverkar inte stenen själv
//
// Därmed kan t.ex stenarna i inputen räknas en efter en. Allt behöver inte processas samtidigt.
//
// Stenar är fungible, en sten med talet 0 kommer gå genom samma process som alla andra stenar med talet 0 gör. Om vi redan vet vad som händer med en sten med talet 0 n steg fram i tiden så behöver vi inte återberäkna det.
//
// Det enda som spelar roll är antalet stenar, inte talen på stenarna. Antalet stenar ändras endast när regel 3 triggas (talet på en sten har ett jämnt antal siffror). Uppgiften är samma som att hitta hur många gånger regel 3 triggas efter n iterationer.
//
// n * 2024
// n * (2E3 + 0E2 + 2E1 + 4E0)
// n 2E3 + n 0E2 + n 2E1 + n 4E0
//
// Det är svårt att hitta slutna formler för beteendet på stenarna. Vi måste snarare minimera antalet gånger reglerna måste evalueras. Troligtvis kommer alla små tal dyka upp gång på gång så svaren för de bör cachas.
//
// Den mest naiva cache-strategin är att för vissa värden av stenar, spara sekvensen av stentillväxt i någon struktur så att man senare kan kolla upp värdet direkt om samma sten kommer igen.

typedef struct stone {
	unsigned long n;
	unsigned long mul;
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
			new_s->mul = s->mul;
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

// Identify duplicate stones and merge them
void optimizeStones(stone *stones) {
	// printf("optimizing\n");
	int i = 0;
	for (stone *s = stones; s; s = s->next, i++) {
		stone *prev = s;
		int j = i + 1;
		for (stone *b = s->next; b; b = b->next, j++) {
			// Check if s and b have the same number
			// If so, delete b and increase s->mul
			if (s->n == b->n) {
				// printf("stone %d has same number (%ld) as stone %d, merging\n", i, s->n, j);
				s->mul += b->mul;
				prev->next = b->next;
				free(b);
				b = prev;
				// printf("stone %d {n: %ld, mul: %ld}\n", i, s->n, s->mul);
			}
			prev = b;
		}
	}
}

unsigned long countStones(stone *stones) {
	unsigned long count = 0;
	for (stone *iter = stones; iter; iter = iter->next) count += iter->mul;
	return count;
}

int main() {
	int blinks = 75;

	stone *stones = NULL, *prev_stone = NULL;
	while (!feof(stdin)) {
		unsigned long n = 0;
		if (scanf("%ld", &n) != 1) break;

		stone *s = calloc(1, sizeof(stone));
		s->n = n;
		s->mul = 1;
		if (stones)
			prev_stone->next = s;
		else
			stones = s;
		prev_stone = s;
	}

	// stone_group group = { stones, 1 };

	for (int i = 0; i < blinks; i++) {
		printf("Tick: %d\n", i);

		applyRules(stones);

		optimizeStones(stones);

		// stone_group *cur_group = &group;
		// while (cur_group) {
		// 	applyRules(cur_group->first);
		// 	optimizeGroup(cur_group);
		// 	cur_group = cur_group->next;
		// }

		printf("\n");
	}

	unsigned long count = countStones(stones);

	printf("Total after %d blinks: %ld\n", blinks, count);

	return 0;
}
