#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

std::map<std::string, std::vector<std::string>> conns;

int tripairToInt(std::string a, std::string b, std::string c) {
	if (b > c) b.swap(c);
	if (a > b) a.swap(b);
	if (b > c) b.swap(c);
	int ai = 30 * a[0] + a[1];
	int bi = 30 * b[0] + b[1];
	int ci = 30 * c[0] + c[1];
	int base = 30 * 30;
	return base*base*ai + base*bi + ci;
}

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

	int sets = 0;
	std::map<int, bool> seen;
	for (auto& key : conns) {
		// Look for keys that start with a t
		if (key.first.c_str()[0] != 't') continue;

		printf("Key: %s\n", key.first.c_str());
		// Iterate all connected nodes
		for (std::string &a : key.second) {
			// Look if any of this nodes neighbours are also in top node neighbour list
			for (std::string &b : conns[a])
				if (!seen[tripairToInt(key.first, a, b)]
						&& std::find(key.second.begin(), key.second.end(), b) != key.second.end()) {
					printf("Tri connection: %s-%s-%s\n", key.first.c_str(), a.c_str(), b.c_str());
					seen[tripairToInt(key.first, a, b)] = true;
					sets++;
				}
		}
	}
	printf("Found %d sets\n", sets);
}
