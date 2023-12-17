#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

bool isDigit(int n) {
	return n >= '0' && n <= '9';
}

int readNextNumber() {
	int num = 0;
	char nextChar;
	while (isDigit(nextChar = fgetc(stdin))) {
		num = num * 10 + (nextChar - '0');
	}
	return num;
}

int nameToID(const char* name) {
	return name[0] << 16 | name[1] << 8 | name[2];
}

typedef struct {
	int id;
	int left;
	int right;
} Node;

// ID-Map of nodes
Node* nodes;
size_t nodesLen = 0;

Node* addNode(Node node) {
	nodes[nodesLen++] = node;
	return nodes + nodesLen - 1;
}

Node* findNode(int id) {
	for (int i = 0; i < nodesLen; i++)
		if (nodes[i].id == id) return nodes + i;

	return NULL;
}

void printNode(int id) {
	printf("ID: %c%c%c, left: %c%c%c, right: %c%c%c\n",
		id>>16, (id>>8)&255, id&255,
		findNode(id)->left>>16, (findNode(id)->left>>8)&255, findNode(id)->left&255,
		findNode(id)->right>>16, (findNode(id)->right>>8)&255, findNode(id)->right&255);
}

int stepsForPath(int start, const char* inst, size_t instLen, int* finalNode) {
	// Step through map
	int steps = 0;
	int current = start;
	do
		current = inst[steps++ % instLen] == 'R' ? findNode(current)->right : findNode(current)->left;
	while ((findNode(current)->id&255) != 'Z');
	if (finalNode) *finalNode = current;
	return steps;
}

int main() {
	nodes = calloc(1000, sizeof(Node));

	// Read instructions
	char inst[1000] = {0};
	size_t instLen = 0;
	int c = 0; // Need to be able to store EOF too
	while ((c = fgetc(stdin)) != '\n') inst[instLen++] = c;
	if (instLen == 0) return -1;
	fgetc(stdin); // Skip newline

	int tracks[10];
	size_t tracksLen = 0;
	do {
		if ((c = fgetc(stdin)) == EOF) break;
		ungetc(c, stdin);

		// Read name
		char str[3] = {0};
		for (int i = 0; (c = fgetc(stdin)) != ' '; i++) str[i] = c; 
		int id = nameToID(str);
		printf("ID: %d\n", id);
		if (str[2] == 'A') tracks[tracksLen++] = id;
		Node* node = addNode((Node){.id = id});
		fgetc(stdin);
		fgetc(stdin);
		fgetc(stdin);

		// Read first arg
		for (int i = 0; (c = fgetc(stdin)) != ','; i++) str[i] = c; 
		node->left = nameToID(str);

		// Read second arg
		fgetc(stdin);
		for (int i = 0; (c = fgetc(stdin)) != ')'; i++) str[i] = c; 
		node->right = nameToID(str);
		c = fgetc(stdin);

		printNode(id);

	}while (c != EOF);

	// // Step through map
	// int steps = 0;
	// while (true) {
	// 	for (int i = 0; i < tracksLen; i++)
	// 		if ((tracks[i] & 255) != 'Z') goto dontBreak;
	// 	break;
	// dontBreak:

	// 	for (int i = 0; i < tracksLen; i++)
	// 		tracks[i] = inst[steps % instLen] == 'R' ? findNode(tracks[i])->right : findNode(tracks[i])->left;
	// 	steps++;
	// 	printf("Steps: %d\n", steps);
	// }

	for (int i = 0; i < tracksLen; i++) {
		int stopID;
		int steps = stepsForPath(tracks[i], inst, instLen, &stopID);
		int loopSteps = stepsForPath(stopID, inst, instLen, NULL);
		printf("Steps for track %d: %d, steps to loop: %d\n", i + 1, steps, loopSteps);
	}

	// printf("Answer: %d\n", steps);

	return 0;
}

