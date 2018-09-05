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

	const int padding_x = 2;
	const int padding_y = 1;
	int max_x;
	int max_y;

public:
	Display(const State &s);

	~Display();

	void print_board() const;
	void print_info() const;

	void set_error(std::string err);
	void set_message(std::string msg);

	void update();

	int x_offset() const;
	int y_offset() const;
	void newline() const;
	void move_with_padding(int y, int x) const;

	const Move get_player_move();
};
