#pragma once

#include "neutrino.h"

class AI {
	const State &state;

	struct best_move_t {
		Move move;
		Player winner;
		int depth;
	};

	best_move_t get_best_move_helper(State &s, int depth, int max_depth) const;
	bool is_better(best_move_t &b1, best_move_t &b2, Player current_player) const;

public:
	
	AI(const State &s);

	Move get_best_move(int max_depth) const;
};
