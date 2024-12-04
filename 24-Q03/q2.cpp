#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <regex>

using namespace std;

int main() {
	FILE* inputFile = fopen("./q1-input.txt", "rb");

	// Get file size
	fseek(inputFile, 0, SEEK_END);
	size_t inputSize = ftell(inputFile);
	fseek(inputFile, 0, SEEK_SET);

	char* input = (char*) calloc(inputSize + 1, 1);
	fread(input, inputSize, 1, inputFile);

	printf("Input: %s\n", input);

	bool mulEnabled = true;
	long sum = 0;

	regex cmdReg("(mul|do|don\'t)\\(((\\d+),(\\d+))?\\)");

	string inputStr(input);
	for (auto i = sregex_iterator(inputStr.begin(), inputStr.end(), cmdReg); i != sregex_iterator(); ++i) {
		smatch match = *i;
		printf("\nMatch: %s\n", match.str().c_str());

		string cmd = match[1].str();
		printf("Command: %s\n", cmd.c_str());
		if (cmd == "mul") {
			if (mulEnabled == false) continue;

			string param1 = match[3].str();
			string param2 = match[4].str();
			printf("Group[1]: %s\n", param1.c_str());
			printf("Group[2]: %s\n", param2.c_str());

			int p1 = atoi(param1.c_str());
			int p2 = atoi(param2.c_str());
			sum += p1 * p2;

		}else if (cmd == "do") {
			mulEnabled = true;

		}else if (cmd == "don\'t") {
			mulEnabled = false;
		}

	}

	printf("Sum = %ld\n", sum);

	return 0;
}
