#pragma once

#include <vector>
#include <utility>

#include <string.h>

enum PieceType : char {
	Empty,
	WhitePiece,
	BlackPiece,
	Neutrino
};

enum Player : char {
	None,
	White,
	Black
};

struct Move {
	int from_x;
	int from_y;
	int to_x;
	int to_y;

	bool operator==(const Move &other) const {
		return memcmp(this, &other, sizeof(*this)) == 0;
	}
};

PieceType piece_for_player(Player p);
Player opposite_player(Player p);
const char *player_name(Player p);

class State {
public:
	PieceType board[5][5] = {{Empty}};
	size_t turn = 0;

	State();

	bool is_empty(int x, int y) const;

	const std::vector<Move> get_possible_moves(int x, int y) const;

	const Player get_current_player() const;

	bool should_neutrino_be_moved() const;

	PieceType get_piece_to_be_moved() const;

	const std::pair<int, int> get_neutrino_position() const;

	const std::vector<Move> get_possible_moves() const;

	Player get_winner() const;
	Player get_loser() const;

	bool is_valid_move(const Move &move) const;
	void perform_move(const Move &move);
	void undo_move(const Move &move);
};
