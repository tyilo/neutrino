#include "macros.h"
#include "display.h"

using namespace std;

char piece_char(PieceType type) {
	switch(type) {
		case Empty:
			return ' ';
		case WhitePiece:
			return 'W';
		case BlackPiece:
			return 'B';
		case Neutrino:
			return 'N';
	}
	__builtin_unreachable();
}

Display::Display(const State &s) : state(s) {
	initscr();
	start_color();

	// Arrow keys produces one character
	keypad(stdscr, TRUE);
	// Enter produces '\r'
	nonl();
	// Don't echo printable characters
	noecho();

	// "White" squares
	init_pair(1, COLOR_BLACK, COLOR_GREEN);

	// Error text
	init_pair(2, COLOR_RED, COLOR_BLACK);

	getmaxyx(stdscr, max_y, max_x);

	update();
}

Display::~Display() {
	endwin();
}

int Display::x_offset() const {
	int board_width = (2 * padding_x + 1) * 5;
	return max_x / 2 - board_width / 2;
}

int Display::y_offset() const {
	int board_height = (2 * padding_y + 1) * 5;
	return max_y / 2 - board_height / 2;
}

void Display::newline() const {
	addch('\n');
	for (int i = 0; i < x_offset(); i++) {
		addch(' ');
	}
}

void Display::print_board() const {
	for(int y = 0; y < 5; y++) {
		for(int oy = -padding_y; oy <= padding_y; oy++) {
			for(int x = 0; x < 5; x++) {
				for(int ox = -padding_x; ox <= padding_x; ox++) {
					bool white = (y + x) % 2;
					if(white) {
						attron(COLOR_PAIR(1));
					}

					/*
					bool left = x == 0 && ox == -padding_x;
					bool right = x == 4 && ox == padding_x;
					bool top = y == 0 && oy == -padding_y;
					bool bottom = y == 4 && oy == padding_y;
					*/

					if (ox == 0 && oy == 0) {
						addch(piece_char(state.board[y][x]));
					} else {
						addch(' ');
					}
					if(white) {
						attroff(COLOR_PAIR(1));
					}
				}
			}
			newline();
		}
	}
}

void Display::print_info() const {
	Player winner = state.get_winner();

	if(winner != None) {
		printw("Turn %zu: %s won the game!\n", state.turn, player_name(winner));
	} else {
		Player player = state.get_current_player();

		const char *name = player_name(player);
		printw("Turn %zu: %s to play ", state.turn, name);

		if(state.should_neutrino_be_moved()) {
			printw("Neutrino");
		} else {
			printw("%s piece", name);
		}

		printw(".\n");
	}
}

void Display::set_error(string err) {
	error = err;
}

void Display::set_message(string msg) {
	message = msg;
}

void Display::update() {
	getmaxyx(stdscr, max_y, max_x);

	move(y_offset(), x_offset());

	print_board();
	newline();
	print_info();

	if(!error.empty()) {
		newline();
		attron(COLOR_PAIR(2));
		printw("%s\n", error.c_str());
		attroff(COLOR_PAIR(2));
	}

	if(!message.empty()) {
		printw("\n%s\n", message.c_str());
	}

	move_with_padding(cursor_y, cursor_x);
	refresh();
}

void Display::move_with_padding(int y, int x) const {
	auto scale = [](int z, int padding){
		return (2 * padding + 1) * z + padding;
	};
	move(y_offset() + scale(y, padding_y), x_offset() + scale(x, padding_x));
}

const Move Display::get_player_move() {
	Move m;

	PieceType piece = state.get_piece_to_be_moved();

	bool from_chosen = false;

	move_with_padding(cursor_y, cursor_x);

	int c;
	while((c = getch())) {
		refresh();
		switch(c) {
			case KEY_UP:
				cursor_y--;
				break;
			case KEY_DOWN:
				cursor_y++;
				break;
			case KEY_LEFT:
				cursor_x--;
				break;
			case KEY_RIGHT:
				cursor_x++;
				break;
			case '\r':
			case KEY_ENTER:
				if(!from_chosen) {
					if(state.board[cursor_y][cursor_x] != piece) {
						break;
					}
					m.from_x = cursor_x;
					m.from_y = cursor_y;
					from_chosen = true;

					int c = inch();
					addch(c ^ A_STANDOUT);
				} else {
					m.to_x = cursor_x;
					m.to_y = cursor_y;
					goto player_input_done;
				}
				break;
			case KEY_RESIZE:
				clear();
				update();
				break;
			default:
				continue;
		}

		set_error("");

		cursor_x = CLAMP(0, cursor_x, 4);
		cursor_y = CLAMP(0, cursor_y, 4);
		move_with_padding(cursor_y, cursor_x);
	}
player_input_done:

	return m;
}
