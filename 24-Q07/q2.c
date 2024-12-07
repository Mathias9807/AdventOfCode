#include "def.h"

#define MAX_LINE 1000

#define num unsigned long

num parseEquation(List* operands) {
	char* line = calloc(MAX_LINE, sizeof(char));
	char* readPtr = fgets(line, MAX_LINE, stdin);
	if (!readPtr) return 0;

	num n;
	sscanf(line, "%ld: ", &n);

	while (*readPtr++ != ' ' && *readPtr) ;

	while (true) {
		num operand = 0;
		char c;
		for (c = 0; (c = *readPtr) >= '0' && c <= '9'; readPtr++) operand = operand * 10 + c - '0';

		num* value = calloc(1, sizeof(num));
		*value = operand;
		ListAdd(operands, value);

		if (c != ' ') break;
		readPtr++;
	}

	return n;
}

bool hasSolution(num lh, num cur, ListEntry* operands) {
	if (operands == NULL)
		return lh == cur;

	if (cur > lh)
		return false;

	// If there is 1(or more) operands left, check both cases
	if (hasSolution(lh, cur + *(num*) operands->value, operands->next))
		return true;
	else if (hasSolution(lh, cur * *(num*) operands->value, operands->next))
		return true;

	else {
		num a = cur;
		num b = *(num*) operands->value;
		num value = a * (num) pow(10, ceil(log10((double) b + 1))) + b;

		if (hasSolution(lh, value, operands->next))
			return true;
	}

	return false;
}

int main() {
	num sum = 0;

	while (!feof(stdin)) {
		List operands = { 0 };
		num n = parseEquation(&operands);

		if (n == 0 && operands.size == 0) break;

		printf("%ld: ", n);
		for (ListEntry* iter = operands.first; iter; iter = iter->next)
			printf("%ld ", *(num*) iter->value);
		printf("\n");

		if (hasSolution(n, 0, operands.first)) {
			// printf("Found solution!\n");
			sum += n;
		}
	}

	printf("Sum: %ld\n", sum);

	return 0;
}
