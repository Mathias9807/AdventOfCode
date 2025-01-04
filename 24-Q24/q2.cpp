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

const char* op_str(Operation op) {
	switch (op) {
	case AND:
		return "AND";
	case OR:
		return "OR";
	case XOR:
		return "XOR";
	}
	return "<null op>";
}

struct Wire {
	std::string name;
	char state = S_UNDEFINED;
};

struct Gate {
	Wire& a;
	Wire& b;
	Wire& c;
	Operation op;

public:
	void print();
};

std::map<std::string, Wire> wires;

std::vector<Gate> gates;

std::map<std::string, std::string> aliases;

void Gate::print() {
	printf("%s\t<- %s %s %s\n", aliases[c.name].c_str(), aliases[a.name].c_str(), op_str(op), aliases[b.name].c_str());
}

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
			// printf("Added wire %s: %d\n", w.name.c_str(), w.state);
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
			// printf("Added gate %s %d %s -> %s\n", g.a.name.c_str(), g.op, g.b.name.c_str(), g.c.name.c_str());
		}else break;
	}
	fclose(f);

	for (auto& pair : wires) {
		aliases[pair.second.name] = pair.second.name;
	}

	// Label the XOR bit sums and the AND carry bits
	for (Gate g : gates) {
		if ((g.a.name[0] == 'x' || g.a.name[0] == 'y')
				&& (g.b.name[0] == 'x' || g.b.name[0] == 'y')
				&& (g.c.name[0] != 'z')) {
			if (g.op == XOR) {
				aliases[g.c.name] = "xor" + g.a.name.substr(1, 3);
			}
			if (g.op == AND) {
				aliases[g.c.name] = "and" + g.a.name.substr(1, 3);
			}
		}
	}

	// Label all the AND:ed XOR:s
	for (Gate g : gates) {
		if (g.op == AND && (aliases[g.a.name].substr(0, 3) == "xor" || aliases[g.b.name].substr(0, 3) == "xor")) {
			Wire* w = &g.a;
			if (aliases[g.b.name].substr(0, 3) == "xor") w = &g.b;
			aliases[g.c.name] = "andedxor" + aliases[w->name].substr(3, 5);
		}
	}

	// mrk <- and01 OR andedxor01
	// Label all the carry bits
	for (Gate g : gates) {
		if (g.op == OR
				// Check that both start with and
				&& (aliases[g.a.name].substr(0, 3) == "and" && aliases[g.b.name].substr(0, 3) == "and")
				// but one is an andedxor
				&& (aliases[g.a.name].substr(0, 8) == "andedxor" || aliases[g.b.name].substr(0, 8) == "andedxor")) {
			Wire* w = &g.a;
			if (aliases[g.b.name].substr(0, 8) == "andedxor") w = &g.b;
			aliases[g.c.name] = "carry" + aliases[w->name].substr(8, 10);
		}
	}


	// Label the sums of the carry bit and xor:ed bit sum
	// for (Gate g : gates) {
	// 	if ((g.c.name[0] == 'z') && g.op == XOR
	// 			&& (aliases[g.a.name].substr(0,3) == "xor" || aliases[g.b.name].substr(0, 3) == "xor")) {
	// 		Wire carry = g.a;
	// 		if (aliases[g.a.name].substr(0, 3) == "xor") carry = g.b;
	// 		aliases[carry.name] = "carry" + g.c.name.substr(1, 3);
	// 	}
	// }

	for (Gate g : gates)
		g.print();

	return 0;
}
