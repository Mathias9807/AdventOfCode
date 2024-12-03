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
	fread(input, inputSize, inputSize, inputFile);

	printf("Input: %s\n", input);

	long sum = 0;

	regex mulReg("mul\\((\\d+),(\\d+)\\)");
	string inputStr(input);
	for (auto i = sregex_iterator(inputStr.begin(), inputStr.end(), mulReg); i != sregex_iterator(); ++i) {
		smatch match = *i;
		string param1 = match[1].str();
		string param2 = match[2].str();
		printf("Match: %s\n", match.str().c_str());
		printf("Group[1]: %s\n", param1.c_str());
		printf("Group[2]: %s\n", param2.c_str());

		int p1 = atoi(param1.c_str());
		int p2 = atoi(param2.c_str());
		sum += p1 * p2;
	}

	printf("Sum = %ld\n", sum);

	return 0;
}
