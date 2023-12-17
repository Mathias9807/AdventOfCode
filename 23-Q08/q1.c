#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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

unsigned short nameToID(const char* name) {
	return (name[0] - 'A') << 10 | (name[1] - 'A') << 5 | (name[2] - 'A');
}

typedef struct {
	unsigned short left;
	unsigned short right;
} Node;

// ID-Map of nodes
Node nodes[1 << 15];
// size_t nodesLen = 0;

// Node* findNode(const char* name) {
// 	for (int i = 0; i < nodesLen; i++)
// 		if (strcmp(nodes[i].name, name) == 0) return nodes + i;
// 
// 	return NULL;
// }

void printNode(unsigned short id) {
	printf("ID: %d%d%d, left: %d%d%d, right: %d%d%d\n",
			id>>10, (id>>5)&0b11111, id&0b11111,
			nodes[id].left>>10, (nodes[id].left>>5)&0b11111, nodes[id].left&0b11111,
			nodes[id].right>>10, (nodes[id].right>>5)&0b11111, nodes[id].right&0b11111);
}

int main() {
	// Read instructions
	char inst[1000] = {0};
	size_t instLen = 0;
	int c = 0; // Need to be able to store EOF too
	while ((c = fgetc(stdin)) != '\n') inst[instLen++] = c;
	fgetc(stdin); // Skip newline

	do {
		if ((c = fgetc(stdin)) == EOF) break;
		ungetc(c, stdin);

		// Read name
		char str[3];
		for (int i = 0; (c = fgetc(stdin)) != ' '; i++) str[i] = c; 
		unsigned short id = nameToID(str);
		Node* node = nodes + id;
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

	// Step through map
	int steps = 0;
	unsigned short current = nameToID("AAA");
	while (current != nameToID("ZZZ"))
		current = inst[steps++ % instLen] == 'R' ? nodes[current].right : nodes[current].left;

	printf("Answer: %d\n", steps);

	return 0;
}

