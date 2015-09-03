#pragma once

#include <string>

#include <ncurses.h>

#include "neutrino.h"

class Display {
	const State &state;
	int cursor_x = 0;
	int cursor_y = 0;
	std::string error = "";
	std::string message = "";

public:
	Display(const State &s);

	~Display();

	void print_board() const;
	void print_info() const;

	void set_error(std::string err);
	void set_message(std::string msg);

	void update() const;

	const Move get_player_move();
};
