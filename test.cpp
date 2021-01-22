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
	S.insert(s);
	cout << "0: 1" << endl;

	int i = 0;
	while (!S.empty()) {
		set<State> S_new;

		for (const auto & s : S) {
			if (s.get_winner() != None) continue;
			for (auto m : s.get_possible_moves()) {
				State s2 = s;
				s2.perform_move(m);
				S_new.insert(s2);
			}
		}

		S = S_new;
		i++;
		cout << i << ": " << S.size() << endl;
	}
}
