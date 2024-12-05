// Vi har en lista med tal. Talen måste uppfylla vissa krav på ordning

#include "def.h"

#define MAX_LINE 1000


typedef struct {
	int dep;
	int num;
} dependency;

List parseRules() {
	char* line = calloc(MAX_LINE, sizeof(char));

	List deps = { 0 };
	while (true) {
		char* readLine = fgets(line, MAX_LINE, stdin);
		if (!readLine || *readLine == '\n') break;

		dependency *dep = calloc(1, sizeof(dependency));
		int matches = sscanf(line, "%d|%d", &dep->dep, &dep->num);

		printf("Rule: %d -> %d\n", dep->dep, dep->num);

		if (matches != 2) break;

		ListAdd(&deps, dep);
	}

	free(line);
	return deps;
}

List parseNumbers() {
	char* line = calloc(MAX_LINE, sizeof(char));

	List numbers = { 0 };
	char* readLine = fgets(line, MAX_LINE, stdin);
	if (!readLine || *readLine == '\n') return numbers;

	char* lPtr = line;
	while (true) {
		int *number = calloc(1, sizeof(int));
		int matches = sscanf(lPtr, "%d", number);

		if (matches != 1) break;

		// Skip to next letter
		while (*lPtr != '\n' && *lPtr >= '0' && *lPtr <= '9') lPtr++;

		ListAdd(&numbers, number);

		if (*lPtr == '\n') break;
		else lPtr++;
	}

	free(line);
	return numbers;
}

bool verifyNumbers(List *numbers, List rules) {
	printf("Checking list [ ");
	for (ListEntry* le = numbers->first; le; le = le->next) printf("%d ", *(int*) le->value);
	printf("]\n");

	for (ListEntry* lNum = numbers->first; lNum->next; lNum = lNum->next) {
		int num = *(int*) lNum->value;
		for (ListEntry* lRule = rules.first; lRule; lRule = lRule->next) {
			dependency *rule = (dependency*) lRule->value;
			if (rule->num != num) continue;

			printf("Checking number %d against rule %d->%d\n", num, rule->dep, rule->num);
			for (ListEntry* lDep = lNum->next; lDep; lDep = lDep->next) {
				if (*(int*) lDep->value == rule->dep)
					return false;
			}
		}
	}

	return true;
}

void printNode(ListEntry* le) {
	printf("ListEntry %#x\n", le);
	if (le->prev) printf("\tprev: %#x\n", le->prev);
	if (le->prev) printf("\tprev: %#x (%d)\n", le->prev, *(int*) le->prev->value);
	else printf("\tprev: NULL\n");
	if (le->value) printf("\tvalue: %d\n", *(int*) le->value);
	else printf("\tvalue: NULL\n");
	if (le->next) printf("\tnext: %#x (%d)\n", le->next, *(int*) le->next->value);
	else printf("\tnext: NULL\n");
}

bool correctList(List *numbers, List rules) {
	printf("Correcting list [ ");
	for (ListEntry* le = numbers->first; le; le = le->next) printf("%d ", *(int*) le->value);
	printf("]\n");

	int index = 0;
	for (ListEntry* lNum = numbers->first; lNum && lNum->next; index++, lNum = lNum->next) {
		int num = *(int*) lNum->value;
		for (ListEntry* lRule = rules.first; lRule; lRule = lRule->next) {
			dependency *rule = (dependency*) lRule->value;
			if (rule->num != num) continue;

			// printf("Checking number %d against rule %d->%d\n", num, rule->dep, rule->num);
			int depIndex = index + 1;
			for (ListEntry* lDep = lNum->next; lDep; lDep = lDep->next, depIndex++) {
				if (*(int*) lDep->value == rule->dep) {
					printf("\tFound broken rule\n");

					if (num == 98 && index == 3 && depIndex == 7) {
						printf("Here\n");
					}

					printf("\tRemoving number %d from index %d and placing at %d\n", num, index, depIndex);
					printf("Before removal\n");
					// printNode(lNum);
					int* value = lNum->value;
					lNum = lNum->next;
					ListRemove(numbers, index);
					printf("After removal, before insert\n");
					// printNode(lNum);
					ListEntry* newNode = ListInsert(numbers, value, depIndex - 1);
					printf("After insert\n");
					// printNode(lNum);
					// printNode(newNode);

					printf("List after adjustment [ ");
					for (ListEntry* le = numbers->first; le; le = le->next) printf("%d ", *(int*) le->value);
					printf("]\n");

					return true;
				}
			}
		}
	}

	return false;
}

int main() {
	List rules = parseRules();

	int sum = 0;
	while (true) {
		List numbers = parseNumbers();
		if (numbers.size == 0) break;

		printf("\n\n\n\n\n\n\n\n");

		// if (verifyNumbers(numbers, rules)) {
		// 	printf("Correct!\n");
		// 	ListEntry* mid = numbers.first;
		// 	for (int i = 0; i < numbers.size / 2; i++)
		// 		mid = mid->next;
		// 	printf("Mid: %d\n", *(int*) mid->value);
		// 	sum += *(int*) mid->value;
		// }else
		// 	printf("False!\n");

		// Correct list until nothing needs changing
		bool neededFix = false;
		while (correctList(&numbers, rules) == true) neededFix = true;

		if (!neededFix) continue;

		// Find mid
		ListEntry* mid = numbers.first;
		for (int i = 0; i < numbers.size / 2; i++)
			mid = mid->next;
		printf("Mid: %d\n", *(int*) mid->value);
		sum += *(int*) mid->value;
	}

	printf("Sum: %d\n", sum);

	return 0;
}
