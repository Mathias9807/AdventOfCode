#include "def.h"

#define MAX_LINE 1000


long parseEquation(List* operands) {
	char* line = calloc(MAX_LINE, sizeof(char));
	char* readPtr = fgets(line, MAX_LINE, stdin);
	if (!readPtr) return 0;

	long n;
	sscanf(line, "%ld: ", &n);

	while (*readPtr++ != ' ' && *readPtr) ;

	while (true) {
		long operand = 0;
		char c;
		for (c = 0; (c = *readPtr) >= '0' && c <= '9'; readPtr++) operand = operand * 10 + c - '0';

		long* value = calloc(1, sizeof(long));
		*value = operand;
		ListAdd(operands, value);

		if (c != ' ') break;
		readPtr++;
	}

	return n;
}

bool hasSolution(long lh, long cur, ListEntry* operands) {
	if (operands == NULL)
		return lh == cur;

	// If there is 1(or more) operands left, check both cases
	if (hasSolution(lh, cur + *(long*) operands->value, operands->next))
		return true;
	else if (hasSolution(lh, cur * *(long*) operands->value, operands->next))
		return true;

	return false;
}

int main() {
	long sum = 0;

	while (!feof(stdin)) {
		List operands = { 0 };
		long n = parseEquation(&operands);

		if (n == 0 && operands.size == 0) break;

		printf("%ld: ", n);
		for (ListEntry* iter = operands.first; iter; iter = iter->next)
			printf("%ld ", *(long*) iter->value);
		printf("\n");

		if (hasSolution(n, 0, operands.first))
			sum += n;
	}

	printf("Sum: %ld\n", sum);

	return 0;
}
