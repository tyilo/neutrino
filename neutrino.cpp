#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

#include <unistd.h>
#include <ncurses.h>

#include "macros.h"
#include "neutrino.h"

using namespace std;

PieceType piece_for_player(Player p) {
	switch(p) {
		case White:
			return WhitePiece;
		case Black:
			return BlackPiece;
		default:
			throw "No piece for player None!";
	}
}

Player opposite_player(Player p) {
	switch(p) {
		case None:
			return None;
		case White:
			return Black;
		case Black:
			return White;
	}
}

const char *player_name(Player p) {
	switch(p) {
		case None:
			return "None";
		case White:
			return "White";
		case Black:
			return "Black";
	}
}

#define INCDIR(dir) (((dir) >= 4)? ((dir) + 1): (dir))
#define DIRX(dir) ((INCDIR(dir) % 3) - 1)
#define DIRY(dir) ((INCDIR(dir) / 3) - 1)

State::State() {
	for(int x = 0; x < 5; x++) {
		board[0][x] = WhitePiece;
		board[4][x] = BlackPiece;
	}
	board[2][2] = Neutrino;
}

bool State::is_empty(int x, int y) const {
	return 0 <= x && x < 5 && 0 <= y && y < 5 && board[y][x] == Empty;
}

const vector<Move> State::get_possible_moves(int x, int y) const {
	vector<Move> moves;

	for(int dir = 0; dir < 8; dir++) {
		int dx = DIRX(dir);
		int dy = DIRY(dir);
		int nx = x + dx;
		int ny = y + dy;

		if(is_empty(nx, ny)) {
			while(is_empty(nx + dx, ny + dy)) {
				nx += dx;
				ny += dy;
			}

			moves.push_back({x, y, nx, ny});
		}
	}

	return moves;
}

const Player State::get_current_player() const {
	return turn % 4 <= 1? White: Black;
}

bool State::should_neutrino_be_moved() const {
	return turn % 2 == 0;
}

PieceType State::get_piece_to_be_moved() const {
	return should_neutrino_be_moved()? Neutrino: piece_for_player(get_current_player());
}

const pair<int, int> State::get_neutrino_position() const {
	for(int x = 0; x < 5; x++) {
		for(int y = 0; y < 5; y++) {
			if(board[y][x] == Neutrino) {
				return {x, y};
			}
		}
	}

	throw "Should not be reachable.";
}

const vector<Move> State::get_possible_moves() const {
	vector<Move> moves;

	for(int x = 0; x < 5; x++) {
		for(int y = 0; y < 5; y++) {
			PieceType piece = board[y][x];
			if(piece != get_piece_to_be_moved()) {
				continue;
			}

			if(piece == Neutrino) {
				auto &ms = get_possible_moves(x, y);
				moves.insert(moves.end(), BEGIN_END(ms));
			} else {
				State state = *this;

				int homerow = piece == WhitePiece? 0: 4;
				auto &ms = get_possible_moves(x, y);

				for(auto &m : ms) {
					bool full = true;
					state.perform_move(m);
					for(int x = 0; x < 5; x++) {
						if(state.board[homerow][x] != piece) {
							full = false;
						}
					}
					state.undo_move(m);
					if(!full) {
						moves.push_back(m);
					}
				}
			}
		}
	}

	return moves;
}

Player State::get_winner() const {
	for(int y = 0; y < 5; y += 4) {
		for(int x = 0; x < 5; x++) {
			if(board[y][x] == Neutrino) {
				return y == 0? Black: White;
			}
		}
	}

	if(get_possible_moves().empty()) {
		return turn % 4 <= 1? Black: White;
	}

	return None;
}

Player State::get_loser() const {
	return opposite_player(get_winner());
}

bool State::is_valid_move(const Move &move) const {
	for(auto &m : get_possible_moves(move.from_x, move.from_y)) {
		if(m == move) {
			return true;
		}
	}
	return false;
}

void State::perform_move(const Move &move) {
	swap(board[move.from_y][move.from_x], board[move.to_y][move.to_x]);
	turn++;
}

void State::undo_move(const Move &move) {
	perform_move(move);
	turn -= 2;
}
