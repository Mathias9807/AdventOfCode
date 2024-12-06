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
};

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

	int steps = 0;
	while (true) {
		for (auto l = map.begin(); l < map.end(); l++)
			cout << *l << endl;
		cout << endl;

		if (map[guard.y][guard.x] != 'X') steps++;
		map[guard.y][guard.x] = 'X';

		Vec guardNext = guard + dir;

		if (guardNext.y >= map.size()) break;
		if (guardNext.x >= map[0].size()) break;

		if (map[guardNext.y][guardNext.x] != '#') {
			guard = guardNext;
			continue;

		}else {
			dir = Vec(-dir.y, dir.x);
			continue;
		}
	}

	cout << "Unique steps: " << steps << endl;

	return 0;
}
