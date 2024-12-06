#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Vec {
public:
	int x, y;

	Vec(int x, int y) : x{x}, y{y} {
	}

	Vec operator+(Vec& v) {
		return Vec(x + v.x, y + v.y);
	}

	bool operator==(Vec& v) {
		return x == v.x && y == v.y;
	}
};

bool haltsWhenObstructed(vector<string> map, Vec guard, Vec dir, Vec obstruct) {
	for (int i = 0; i < 10000; i++) {
		// map[guard.y][guard.x] = 'X';

		Vec guardNext = guard + dir;

		if (guardNext.y >= map.size()) return true;
		if (guardNext.x >= map[0].size()) return true;

		bool isObstructed = map[guardNext.y][guardNext.x] == '#' || guardNext == obstruct;
		if (isObstructed) {
			dir = Vec(-dir.y, dir.x);
			continue;
		}else {
			guard = guardNext;
			continue;

		}
	}

	return false;
}

int main() {
	vector<string> map;

	string line;
	while (true) {
		getline(cin, line);
		if (!line.size()) break;

		map.push_back(line);
	}

	Vec guard = Vec(0, 0);
	Vec dir = Vec(0, 0);
	for (auto l = map.begin(); l < map.end(); l++) {
		cout << *l << endl;

		if ((guard.x = l->find('^')) != string::npos) { dir = Vec( 0,-1); break;}
		if ((guard.x = l->find('>')) != string::npos) { dir = Vec( 1, 0); break;}
		if ((guard.x = l->find('<')) != string::npos) { dir = Vec(-1, 0); break;}
		if ((guard.x = l->find('v')) != string::npos) { dir = Vec( 0, 1); break;}

		guard.y++;
	}

	printf("x=%d, y=%d\n", guard.x, guard.y);

	int loops = 0;
	for (int x = 0; x < map[0].size(); x++) {
		for (int y = 0; y < map.size(); y++) {
			Vec obstruction = Vec(x, y);

			if (obstruction == guard) continue;

			if (map[obstruction.y][obstruction.x] != '.') continue;

			if (!haltsWhenObstructed(map, guard, dir, obstruction))
				loops++;
		}
	}

	cout << "# obstructions that lead to loops: " << loops << endl;

	return 0;
}
