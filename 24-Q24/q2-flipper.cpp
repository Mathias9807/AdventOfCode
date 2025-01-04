#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <map>
#include <random>
#include <algorithm>

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
	Wire* a;
	Wire* b;
	Wire* c;
	Operation op;
};

std::map<std::string, Wire> wires;

std::vector<Gate> gates;

bool worthwhile[] = {
	true, // y04 XOR x04 -> cwr
	true, // wbd AND hfr -> fhk
	true, // y30 XOR x30 -> jgk
	false, // bfd XOR mrk -> z02
	true, // y16 AND x16 -> twg
	true, // x31 XOR y31 -> nrj
	true, // x22 XOR y22 -> vrw
	false, // hmg XOR vws -> z21
	true, // mbj OR jwf -> bfg
	true, // y01 XOR x01 -> hfr
	true, // drq AND wrs -> fvh
	true, // y41 XOR x41 -> wdd
	false, // fvg XOR trw -> z10
	true, // y13 XOR x13 -> mrw
	true, // y34 XOR x34 -> nhv
	true, // nsf AND vwv -> dmw
	true, // y25 AND x25 -> fvp
	true, // knj AND nvk -> pjr
	true, // ctm AND dmn -> bnt
	false, // nrj XOR htv -> z31
	true, // y26 XOR x26 -> kfq
	true, // sbq AND hkg -> vsm
	false, // gss XOR vrw -> z22
	true, // y43 AND x43 -> rwv
	true, // spr OR qcw -> wnv
	true, // ngp OR kvv -> swr
	true, // y40 XOR x40 -> wsw
	false, // hht XOR bfg -> z06
	true, // vws AND hmg -> pmj
	true, // rfn OR sgs -> ncd
	true, // y08 AND x08 -> khp
	true, // x25 XOR y25 -> nvk
	true, // x19 XOR y19 -> mpm
	true, // dmw OR twg -> pvh
	false, // knj XOR nvk -> z25
	true, // fsq AND dqg -> cps
	true, // qsb OR vsm -> chr
	false, // sdc XOR fmv -> z08
	true, // y33 AND x33 -> hrv
	true, // sbq XOR hkg -> qsb
	false, // y00 XOR x00 -> z00
	false, // dgk XOR jgk -> z30
	true, // x03 AND y03 -> wqk
	true, // y17 XOR x17 -> rqq
	false, // rhc XOR qhr -> z09
	true, // bjd OR vcw -> hvw
	true, // x37 XOR y37 -> knp
	true, // knp AND pbm -> spn
	true, // kfq AND qgs -> z26
	true, // x39 AND y39 -> z39
	true, // x33 XOR y33 -> fsq
	true, // y10 AND x10 -> fsw
	true, // rqq XOR pvh -> wmp
	true, // y07 XOR x07 -> wkm
	true, // dbn OR vvq -> vcv
	true, // vcv AND mpm -> cqc
	false, // scb XOR wkm -> z07
	true, // y28 AND x28 -> kvv
	true, // fmk OR grc -> dqg
	false, // ctm XOR dmn -> z44
	true, // y24 XOR x24 -> jhj
	true, // y35 AND x35 -> bkk
	true, // x16 XOR y16 -> vwv
	false, // gvp XOR jdk -> z14
	true, // vfq AND wmp -> dbn
	true, // x29 XOR y29 -> pjv
	true, // cqn OR qvr -> hrk
	true, // wtc AND gdw -> mjd
	true, // x26 AND y26 -> vfk
	true, // vfs AND nrn -> bjd
	false, // cpd XOR hvw -> z28
	true, // mjd OR gnv -> psq
	true, // pgd OR qgn -> gvp
	true, // ggm AND tnh -> dmf
	true, // pvh AND rqq -> ffg
	true, // qjj AND jdm -> ckv
	true, // dgk AND jgk -> mfg
	true, // x23 AND y23 -> gnv
	true, // hvw AND cpd -> ngp
	true, // rcr OR wkv -> nsf
	true, // x36 AND y36 -> rbf
	true, // bkk OR gnp -> bgv
	false, // knp XOR pbm -> z37
	true, // y27 XOR x27 -> nrn
	true, // x22 AND y22 -> hsd
	true, // jdk AND gvp -> ffb
	true, // y21 XOR x21 -> hmg
	true, // sqj OR wvw -> tnh
	false, // vfq XOR wmp -> z18
	false, // tnh XOR ggm -> z43
	true, // jhj AND psq -> ptp
	true, // x18 XOR y18 -> vfq
	false, // ncd XOR jrp -> z32
	true, // x21 AND y21 -> mnt
	true, // x02 XOR y02 -> bfd
	true, // rjs AND cwp -> mcb
	false, // cwp XOR rjs -> z38
	true, // x32 AND y32 -> fmk
	true, // x05 XOR y05 -> sck
	true, // y03 XOR x03 -> nmb
	true, // y35 XOR x35 -> cpb
	true, // vcq AND bgv -> nfm
	true, // x05 AND y05 -> mbj
	true, // bnt OR mhg -> z45
	true, // ktq OR fhk -> mrk
	false, // hfr XOR wbd -> z01
	false, // dqg XOR fsq -> z33
	true, // cwr AND www -> cqn
	true, // y12 XOR x12 -> dnj
	true, // qnk OR mbb -> fmv
	false, // jhj XOR psq -> z24
	true, // y12 AND x12 -> rvj
	true, // bfd AND mrk -> hwt
	true, // gdm OR wqk -> www
	true, // cpb AND wnv -> gnp
	true, // y13 AND x13 -> pgd
	true, // khp OR jhv -> rhc
	true, // hsd OR bdv -> gdw
	true, // fmv AND sdc -> jhv
	false, // chr XOR wsw -> z40
	true, // sck AND hrk -> jwf
	true, // bng OR fsw -> jdm
	true, // x07 AND y07 -> mbb
	false, // gfc XOR wdd -> z41
	true, // kfq XOR qgs -> gvm
	true, // rvj OR pjk -> mmk
	true, // x29 AND y29 -> crg
	true, // x09 AND y09 -> csw
	true, // mcb OR cdq -> hkg
	true, // spn OR mbq -> rjs
	true, // trw AND fvg -> bng
	true, // y31 AND x31 -> rfn
	false, // sck XOR hrk -> z05
	false, // cwr XOR www -> z04
	false, // nhv XOR ptb -> z34
	true, // y20 AND x20 -> svn
	true, // nmb AND qkc -> gdm
	true, // htv AND nrj -> sgs
	false, // bgv XOR vcq -> z36
	false, // mpm XOR vcv -> z19
	true, // y41 AND x41 -> rbk
	true, // x24 AND y24 -> ngs
	true, // ckv OR gjc -> sfm
	true, // sfw OR hwt -> qkc
	true, // hrv OR cps -> ptb
	true, // fvp OR pjr -> qgs
	false, // jgs XOR ptj -> z15
	false, // swr XOR pjv -> z29
	true, // scb AND wkm -> qnk
	true, // x34 AND y34 -> qcw
	true, // x27 AND y27 -> vcw
	true, // x06 XOR y06 -> hht
	true, // x18 AND y18 -> vvq
	false, // mrw XOR mmk -> z13
	true, // y15 XOR x15 -> jgs
	true, // jfc OR fct -> gfc
	true, // ckj OR rbk -> ntc
	true, // x38 AND y38 -> cdq
	true, // vfk OR gvm -> vfs
	false, // wkt XOR ntc -> z42
	true, // crg OR kpw -> dgk
	true, // kmw OR ffb -> ptj
	true, // tfp OR qdd -> scb
	true, // y28 XOR x28 -> cpd
	true, // y00 AND x00 -> wbd
	true, // y40 AND x40 -> jfc
	false, // dnj XOR sfm -> z12
	true, // y38 XOR x38 -> cwp
	true, // x37 AND y37 -> mbq
	false, // nrn XOR vfs -> z27
	true, // y39 XOR x39 -> sbq
	true, // mmk AND mrw -> qgn
	true, // gss AND vrw -> bdv
	true, // fvh OR svn -> vws
	false, // qkc XOR nmb -> z03
	true, // qjj XOR jdm -> z11
	false, // cpb XOR wnv -> z35
	true, // x14 XOR y14 -> jdk
	true, // y09 XOR x09 -> qhr
	true, // x11 XOR y11 -> gjc
	true, // chr AND wsw -> fct
	true, // ptj AND jgs -> wkv
	true, // swr AND pjv -> kpw
	true, // x20 XOR y20 -> wrs
	true, // y44 AND x44 -> mhg
	true, // y19 AND x19 -> qhw
	true, // y42 AND x42 -> wvw
	true, // gfc AND wdd -> ckj
	true, // cqc OR qhw -> drq
	true, // y08 XOR x08 -> sdc
	true, // y42 XOR x42 -> wkt
	true, // y17 AND x17 -> pqv
	true, // qwj OR csw -> trw
	true, // ptb AND nhv -> spr
	true, // y44 XOR x44 -> dmn
	true, // rwv OR dmf -> ctm
	true, // y23 XOR x23 -> wtc
	true, // sfm AND dnj -> pjk
	true, // pmj OR mnt -> gss
	true, // hht AND bfg -> tfp
	false, // nsf XOR vwv -> z16
	true, // pqv OR ffg -> z17
	false, // gdw XOR wtc -> z23
	true, // x30 AND y30 -> fnf
	true, // nfm OR rbf -> pbm
	true, // x32 XOR y32 -> jrp
	true, // y11 AND x11 -> qjj
	true, // x04 AND y04 -> qvr
	true, // ncd AND jrp -> grc
	true, // wkt AND ntc -> sqj
	true, // x36 XOR y36 -> vcq
	true, // x01 AND y01 -> ktq
	true, // x15 AND y15 -> rcr
	true, // fnf OR mfg -> htv
	true, // y14 AND x14 -> kmw
	true, // x10 XOR y10 -> fvg
	true, // x06 AND y06 -> qdd
	false, // wrs XOR drq -> z20
	true, // y43 XOR x43 -> ggm
	true, // ngs OR ptp -> knj
	true, // rhc AND qhr -> qwj
	true, // y02 AND x02 -> sfw
};

unsigned long run_configuration(std::vector<Gate>& gates) {
	while (true) {
		// Iterate all gates and see which one can be updated
		bool none_updated = true;
		for (Gate& g : gates) {
			if (g.c->state != S_UNDEFINED) continue;
			if (g.a->state == S_UNDEFINED || g.b->state == S_UNDEFINED) continue;

			switch (g.op) {
			case AND:
				// printf("AND:ing %s and %s into %s\n", g.a.name.c_str(), g.b.name.c_str(), g.c.name.c_str());
				g.c->state = g.a->state & g.b->state;
				break;
			case OR:
				// printf("OR:ing %s and %s into %s\n", g.a.name.c_str(), g.b.name.c_str(), g.c.name.c_str());
				g.c->state = g.a->state | g.b->state;
				break;
			case XOR:
				// printf("XOR:ing %s and %s into %s\n", g.a.name.c_str(), g.b.name.c_str(), g.c.name.c_str());
				g.c->state = g.a->state ^ g.b->state;
				break;
			}
			none_updated = false;
		}
		if (none_updated) break;
	}

	// for (auto pair : wires)
	// 	printf("\t%s: %d\n", pair.first.c_str(), pair.second.state);

	unsigned long num = 0;
	for (int i = 99; i >= 0; i--) {
		char wire[4] = {0};
		sprintf(wire, "z%02d", i);
		if (wires[std::string(wire)].state == S_UNDEFINED) continue;
		num = num << 1 | (wires[std::string(wire)].state & 1);
	}
	return num;
}

int main() {
	srand(time(nullptr));

	FILE* f = fopen("input.txt", "r");
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
		size_t len = 0;
		getline(&line, &len, f);
		std::smatch matches;
		std::string line_str(line);
		if (std::regex_search(line_str, matches, gate_re)) {
			wires.insert({matches[1], {matches[1], S_UNDEFINED}});
			wires.insert({matches[3], {matches[3], S_UNDEFINED}});
			wires.insert({matches[4], {matches[4], S_UNDEFINED}});
			Gate g = {&wires[matches[1]], &wires[matches[3]], &wires[matches[4]], AND};
			if (matches[2] == "AND") g.op = AND;
			if (matches[2] == "OR")  g.op = OR;
			if (matches[2] == "XOR") g.op = XOR;
			gates.push_back(g);
			printf("Added gate %s %d %s -> %s\n", g.a->name.c_str(), g.op, g.b->name.c_str(), g.c->name.c_str());
		}else break;
	}
	fclose(f);

	unsigned long target = 49521014360658;

	std::map<std::string, Wire> wires_bkp(wires);

	// Try switching every possible pair of pairs until we find the correct two
	std::vector<Gate> nu_gates(gates);
	for (int a = 0; a < gates.size(); a++) {
		if (worthwhile[a] == false) continue;
		printf("a: %d\n", a);

		for (int b = a; b < gates.size(); b++) {
			if (worthwhile[b] == false) continue;
			printf("b: %d\n", b);

			for (int c = b; c < gates.size(); c++) {
				if (worthwhile[c] == false) continue;
				printf("c: %d\n", c);

				for (int d = c; d < gates.size(); d++) {
					if (worthwhile[d] == false) continue;

					for (int flip = 1; flip < 4; flip++) {
						for (auto pair : wires)
							wires[pair.first].state = wires_bkp[pair.first].state;

						std::vector<int> picks = {a, b, c, d};
						// printf("picks: ");
						// for (auto p : picks) printf("%d ", p);
						// printf("\n");

						// Flip pairs
						int first = picks[0];
						int second_i = flip;
						int second = picks[second_i];
						picks.erase(picks.begin() + second_i);
						picks.erase(picks.begin() + 0);
						Wire* tmp = nu_gates[first].c;
						Wire* tmp2 = nu_gates[second].c;
						nu_gates[first].c = tmp2;
						nu_gates[second].c = tmp;

						tmp = nu_gates[picks[0]].c;
						tmp2 = nu_gates[picks[1]].c;
						nu_gates[picks[0]].c = tmp2;
						nu_gates[picks[1]].c = tmp;

						// printf("tried to flip %d with %d and %d with %d\n", first, second, picks[0], picks[1]);

						unsigned long num = run_configuration(nu_gates);

						// printf("Number: %ld\n", num);
						if (num == target) return 0;

						nu_gates[a].c = gates[a].c;
						nu_gates[b].c = gates[b].c;
						nu_gates[c].c = gates[c].c;
						nu_gates[d].c = gates[d].c;
					}
				}
			}
		}
	}

	return 0;
}
