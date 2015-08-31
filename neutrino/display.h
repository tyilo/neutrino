#pragma once

#include <ncurses.h>

#include "neutrino.h"

class Display {
	const State &state;
	int cursor_x = 0;
	int cursor_y = 0;
	const char *error = NULL;

public:
	Display(const State &s);

	~Display();

	void print_board() const;
	void print_info() const;

	void set_error(const char *err);

	void update() const;

	const Move get_player_move();
};
