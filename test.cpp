#include "neutrino.h"
#include <bits/stdc++.h>
using namespace std;

bool operator<(const State & s1, const State & s2) {
	int v = memcmp(s1.board, s2.board, sizeof s1.board);
	return v < 0 || (v == 0 && s1.turn % 4 < s2.turn % 4);
}

int main() {
	State s;
	set<State> S;
	queue<pair<int, State>> Q;
	Q.push({0, s});
	S.insert(s);

	int pd = -1;
	while (!Q.empty()) {
		auto [d, s] = Q.front();
		Q.pop();

		if (pd != d) {
			assert(d == pd + 1);

			cout << d << ": " << S.size() << endl;

			pd = d;
		}

		if (s.get_winner() != None) continue;

		for (auto &m : s.get_possible_moves()) {
			State s2 = s;
			s2.perform_move(m);

			auto [_, inserted] = S.insert(s2);
			if (inserted) {
				Q.push({d + 1, s2});
			}
		}
	}
}
