#include <stdio.h>

#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <map>

#define S_OFF 0
#define S_ON 1
#define S_UNDEFINED 2

enum Operation {
	AND,
	OR,
	XOR
};

struct Wire {
	std::string name;
	char state = S_UNDEFINED;
};

struct Gate {
	Wire& a;
	Wire& b;
	Wire& c;
	Operation op;
};

std::map<std::string, Wire> wires;

std::vector<Gate> gates;

int main() {
	FILE* f = fopen("input-corrected3.txt", "r");
	std::regex line_re{"([a-zA-Z0-9]+): ([01])\n"};
	while (!feof(f)) {
		char *line = nullptr;
		size_t len;
		getline(&line, &len, f);
		std::smatch matches;
		std::string lineStr(line);
		if (std::regex_search(lineStr, matches, line_re)) {
			Wire w;
			w.name = matches[1];
			w.state = atoi(matches[2].str().c_str());
			wires[w.name] = w;
			printf("Added wire %s: %d\n", w.name.c_str(), w.state);
		}else break;
	}

	std::regex gate_re{"([a-zA-Z0-9]+) (AND|OR|XOR) ([a-zA-Z0-9]+) -> ([a-zA-Z0-9]+)\n"};
	while (!feof(f)) {
		char *line = nullptr;
		size_t len;
		getline(&line, &len, f);
		std::smatch matches;
		std::string line_str(line);
		if (std::regex_search(line_str, matches, gate_re)) {
			wires.insert({matches[1], {matches[1], S_UNDEFINED}});
			wires.insert({matches[3], {matches[3], S_UNDEFINED}});
			wires.insert({matches[4], {matches[4], S_UNDEFINED}});
			Gate g = {wires[matches[1]], wires[matches[3]], wires[matches[4]], AND};
			if (matches[2] == "AND") g.op = AND;
			if (matches[2] == "OR")  g.op = OR;
			if (matches[2] == "XOR") g.op = XOR;
			gates.push_back(g);
			printf("Added gate %s %d %s -> %s\n", g.a.name.c_str(), g.op, g.b.name.c_str(), g.c.name.c_str());
		}else break;
	}
	fclose(f);

	while (true) {
		// Iterate all gates and see which one can be updated
next_iter:
		bool none_updated = true;
		for (Gate& g : gates) {
			if (g.c.state != S_UNDEFINED) continue;
			if (g.a.state == S_UNDEFINED || g.b.state == S_UNDEFINED) continue;

			switch (g.op) {
			case AND:
				printf("AND\t%s\t%s\t%s\n", g.a.name.c_str(), g.b.name.c_str(), g.c.name.c_str());
				g.c.state = g.a.state & g.b.state;
				goto next_iter;
			case OR:
				printf("OR\t%s\t%s\t%s\n", g.a.name.c_str(), g.b.name.c_str(), g.c.name.c_str());
				g.c.state = g.a.state | g.b.state;
				goto next_iter;
			case XOR:
				printf("XOR\t%s\t%s\t%s\n", g.a.name.c_str(), g.b.name.c_str(), g.c.name.c_str());
				g.c.state = g.a.state ^ g.b.state;
				goto next_iter;
			}
			none_updated = false;
		}
		if (none_updated) break;
	}

	for (auto pair : wires)
		printf("\t%s: %d\n", pair.first.c_str(), pair.second.state);

	unsigned long num = 0;
	for (int i = 99; i >= 0; i--) {
		char wire[4] = {0};
		sprintf(wire, "z%02d", i);
		if (wires[std::string(wire)].state == S_UNDEFINED) continue;
		num = num << 1 | (wires[std::string(wire)].state & 1);
	}
	printf("Number: %ld\n", num);

	return 0;
}
