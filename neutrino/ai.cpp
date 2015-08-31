#include "ai.h"
#include "neutrino.h"

AI::AI(const State &s) : state(s) {
}

bool AI::is_better(best_move_t &b1, best_move_t &b2, Player current_player) const {
	if(b1.winner != b2.winner) {
		if(b1.winner == current_player) {
			return false;
		}
		if(b2.winner == current_player) {
			return true;
		}
		return b2.winner == None;
	} else {
		return b2.winner == current_player? b2.depth < b1.depth: b2.depth > b1.depth;
	}
}

AI::best_move_t AI::get_best_move_helper(State &s, int depth, int max_depth) const {
	Player player = s.get_current_player();
	Player opp_player = opposite_player(player);

	if(s.get_winner() != None) {
		throw "Can't get best move on finished game!";
	}

	auto &moves = s.get_possible_moves();

	best_move_t best_move = {moves[0], opp_player, depth};

	for(auto &m : moves) {
		s.perform_move(m);

		Player winner = s.get_winner();

		if(winner == player) {
			best_move = {m, player, depth};
			goto best_move_return_and_undo;
		}

		if(winner == None) {
			if(depth == max_depth) {
				best_move = {m, None, depth};
				goto best_move_return_and_undo;
			} else {
				best_move_t b = get_best_move_helper(s, depth + 1, max_depth);
				if(is_better(best_move, b, player)) {
					best_move = {m, b.winner, b.depth};
				}
			}
		}

		s.undo_move(m);
	}

	return best_move;

best_move_return_and_undo:
	s.undo_move(best_move.move);

	return best_move;
}

Move AI::get_best_move(int max_depth) const {
	State copy = state;
	return get_best_move_helper(copy, 0, max_depth).move;
}
