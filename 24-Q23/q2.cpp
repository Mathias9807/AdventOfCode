#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

std::map<std::string, std::vector<std::string>> conns;

int main() {
	FILE *f = fopen("input.txt", "r");
	while (!feof(f)) {
		char a[3] = { 0 };
		char b[3] = { 0 };
		a[0] = fgetc(f); a[1] = fgetc(f);
		if (a[0] == EOF) break;
		fgetc(f);
		b[0] = fgetc(f); b[1] = fgetc(f);
		fgetc(f);
		printf("%s - %s\n", a, b);
		conns[std::string(a)].push_back(std::string(b));
		conns[std::string(b)].push_back(std::string(a));
	}
	fclose(f);

	for (auto& key : conns)
		std::sort(conns[key.first].begin(), conns[key.first].end());

	// Look for the largest fully connected subset by picking a node a and placing it in a list of nodes
	// For each neighbour, check if it connects to each member of the list and if so, add it as well
	// Once every neighbour of a has been checked, save the list if it is larger than the previous set of nodes
	std::vector<std::string> best_set;
	for (auto& key : conns) {
		std::vector<std::string> set = {key.first};

		printf("Key: %s\n", key.first.c_str());

		// Check neighbours, starting from some index
		for (int i = 0; i < key.second.size(); i++) {
			std::vector<std::string> test_set(set);
			for (auto b = key.second.begin() + i; b < key.second.end(); b++) {
				// Check if this neighbour can be added to the set
				bool allThere = true;
				for (std::string &node : test_set) {
					if (std::find(conns[node].begin(), conns[node].end(), *b) == conns[node].end())
						allThere = false;
				}
				if (allThere) test_set.push_back(*b);
			}
			printf("Set: %s\n", key.first.c_str());
			for (auto s : test_set) {
				printf("\t%s\n", s.c_str());
			}
			if (test_set.size() > best_set.size())
				best_set = test_set;
		}
	}

	printf("Best set: ");
	std::sort(best_set.begin(), best_set.end());
	for (auto s : best_set) {
		printf("%s,", s.c_str());
	}
	printf("\n");
}
