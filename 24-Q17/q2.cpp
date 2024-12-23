#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <vector>

char* program;

#define num_type unsigned long

num_type search(num_type seed) {
	// Try each 3 bit grouping from left to right that satisfy the constraint
	int nums[] = {
		2,4,1,2,7,5,4,5,1,3,5,5,0,3,3,0
	};

	// We infer the number from the last output
	num_type A = seed;
	std::vector<num_type> matches = {seed};
	for (int n = sizeof(nums) / sizeof(int) - 1; n >= 0; n--) {
		// Try each possible three bit number and see which produces the correct number
		printf("looking for match for num %d, matches: { ", nums[n]);
		for (num_type m : matches) printf("%ld ", m);
		printf("}\n");
		std::vector<num_type> new_matches;
		bool found_num = false;
		for (num_type prev_match : matches) {
			printf("Trying with previous match: %ld\n", prev_match);
			for (int i = 0; i <= 0b111; i++) {
				num_type a = (prev_match << 3) | i;
				num_type b = (a % 8) ^ 2;
				num_type c = a >> b;
				b = b ^ c;
				b = b ^ 3;
				printf("group %d gives %ld, target %d\n", i, b % 8, nums[n]);
				if ((b % 8) == nums[n]) {
					found_num = true;
					A = a;
					printf("Found match for A=%ld and num %d\n", a, nums[n]);
					new_matches.push_back(a);
					// break;
				}
			}
		}
		if (!found_num) {
			printf("Couldn't find anything with seed %ld\n", seed);
			return -1;
		}
		matches = new_matches;
	}

	printf("Found full match\n");
	return A;
}

int main(int argc, char **argv) {
	// for (int i = 0; i < 0b1111111111; i++) {
	// 	int ret = search(i);
	// 	printf("search(%d) -> %d\n", i, ret);
	// 	if (ret > 0) break;
	// }

	unsigned long A = search(0);
	printf("search() -> %ld\n", A);

	// exit(0);

	FILE* f = fopen("input.txt", "r");

	program = (char*) calloc(1024, 1);

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

	// int invocation = 0;
	// if (argc > 1) {
	// 	sscanf(argv[1], "%d", &invocation);
	// }
	// printf("Invocation %d\n", invocation);
	// long test_size = 1000000000;

	// for (int a = invocation * test_size; a < (invocation + 1) * test_size; a++) {
	// for (unsigned long a = 0; a < 10000000; a++) {
		// regs[0] = 433 | (a << 9);

		// printf("\nA = %ld\n", a);
		int pc = 0;
		regs[0] = A;
		unsigned long ticks = 0;
		while (pc < p_index) {
			int operand = program[pc + 1];
			unsigned long combo = program[pc + 1] & 0b11;
			if (program[pc + 1] & 0b100)
				combo = regs[combo];

			switch (program[pc]) {
			case 0: // adv
				regs[0] = regs[0] / pow(2, combo);
				break;
			case 6: // bdv
				regs[1] = regs[0] / pow(2, combo);
				break;
			case 7: // cdv
				regs[2] = regs[0] / pow(2, combo);
				break;
			case 1: // bxl
				regs[1] = regs[1] ^ operand;
				break;
			case 2: // bst
				regs[1] = combo % 8;
				break;
			case 3: // jnz
				if (regs[0] != 0) {
					pc = operand;
					pc -= 2;
				}
				break;
			case 4: // bxc
				regs[1] = regs[1] ^ regs[2];
				break;
			case 5: // out
				// if (combo % 8 != program[ticks++])
				// 	pc = p_index;
				// if (ticks == p_index) {
				// 	// printf("A value is %ld\n", a);
				// }
				printf("%ld,", combo % 8);
				break;
			}
			pc += 2;
		// }
	}

	free(program);
	return 0;
}
