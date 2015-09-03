#pragma once

#include "neutrino.h"

class AI {
public:
	struct best_move_t {
		Move move;
		Player winner;
		int depth;
	};
	
private:
	const State &state;

	best_move_t get_best_move_helper(State &s, int depth, int max_depth) const;
	bool is_better(best_move_t &b1, best_move_t &b2, Player current_player) const;

public:
	
	AI(const State &s);

	AI::best_move_t get_best_move(int max_depth) const;
};
