import std.stdio;
import std.string;

int findDigit(string text) {
	if (text[0] >= '0' && text[0] <= '9')
		return text[0] - '0';

	string[] digits = ["zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"];
	for (int i = 0; i < digits.length; i++) {
		if (text.startsWith(digits[i]))
			return i;
	}

	return -1;
}

void main() {
	File file = File("input.txt", "r");

	int[] numbers;
	while (!file.eof()) {
		string line = file.readln();
		int num = 0;

		// Find last digit
		for (int i = 0; i < line.length; i++) {
			int digit = findDigit(line[i..$]);
			if (digit != -1)
				num = digit;
		}

		// Add first digit
		for (int i = 0; i < line.length; i++) {
			int digit = findDigit(line[i..$]);
			if (digit != -1) {
				num = num + digit * 10;
				break;
			}
		}

		numbers ~= num;
	}


	int sum = 0;
	foreach (int n; numbers) sum += n;

	writeln("Answer: ", sum);
}

