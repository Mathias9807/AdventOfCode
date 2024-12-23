#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

char* program;

int main(int argc, char **argv) {
	FILE* f = fopen("input.txt", "r");

	program = calloc(1024, 1);

	unsigned long regs[3];
	fscanf(f, "Register A: %ld\n", regs + 0);
	fscanf(f, "Register B: %ld\n", regs + 1);
	fscanf(f, "Register C: %ld\n", regs + 2);
	printf("Registers %ld, %ld, %ld\n",
			regs[0], regs[1], regs[2]);

	fscanf(f, "Program: ");
	int p_index = 0;
	while (!feof(f)) {
		int opcode = 0, operand = 0;
		int matches = fscanf(f, "%d,%d", &opcode, &operand);
		if (matches < 2) break;
		fgetc(f);

		printf("opcode: %d, operand: %d\n", opcode, operand);
		program[p_index++] = opcode;
		program[p_index++] = operand;
	}

	int pc = 0;
	unsigned long ticks = 0;
	while (pc < p_index) {
		int operand = program[pc + 1];
		unsigned long combo = program[pc + 1] & 0b11;
		if (program[pc + 1] & 0b100)
			combo = regs[combo];

		switch (program[pc]) {
		case 0: // adv
			printf("A(%ld) >> %ld\n", regs[0], combo);
			regs[0] = regs[0] / pow(2, combo);
			printf(" -> A(%ld)\n", regs[0]);
			break;
		case 6: // bdv
			printf("A(%ld) >> %ld\n", regs[0], combo);
			regs[1] = regs[0] / pow(2, combo);
			printf(" -> B(%ld)\n", regs[1]);
			break;
		case 7: // cdv
			printf("A(%ld) >> %ld\n", regs[0], combo);
			regs[2] = regs[0] / pow(2, combo);
			printf(" -> C(%ld)\n", regs[2]);
			break;
		case 1: // bxl
			printf("B(%ld) ^ %d\n", regs[1], operand);
			regs[1] = regs[1] ^ operand;
			printf(" -> B(%ld)\n", regs[1]);
			break;
		case 2: // bst
			printf("B(%ld) = %ld\n", regs[1], combo % 8);
			regs[1] = combo % 8;
			printf(" -> B(%ld)\n", regs[1]);
			break;
		case 3: // jnz
			if (regs[0] != 0) {
				pc = operand;
				pc -= 2;
			}
			break;
		case 4: // bxc
			printf("B(%ld) ^ C(%ld)\n", regs[1], regs[2]);
			regs[1] = regs[1] ^ regs[2];
			printf(" -> B(%ld)\n", regs[1]);
			break;
		case 5: // out
			if (program[pc + 1] & 0b100)
				printf("out %c(%ld)\n", 'A' + (program[pc + 1] & 0b11), combo % 8);
			else
				printf("out %ld\n", combo % 8);
			break;
		}
		pc += 2;
	}

	free(program);
	return 0;
}
