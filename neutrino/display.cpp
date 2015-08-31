#include "macros.h"
#include "display.h"

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

	update();
}

Display::~Display() {
	endwin();
}

void Display::print_board() const {
	for(int y = 0; y < 5; y++) {
		for(int x = 0; x < 5; x++) {
			bool white = (y + x) % 2;
			if(white) {
				attron(COLOR_PAIR(1));
			}
			addch(piece_char(state.board[y][x]));
			if(white) {
				attroff(COLOR_PAIR(1));
			}
		}
		addch('\n');
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

void Display::set_error(const char *err) {
	error = err;
}

void Display::update() const {
	move(0, 0);

	print_board();
	addch('\n');
	print_info();

	if(error) {
		addch('\n');
		attron(COLOR_PAIR(2));
		printw("%s\n", error);
		attroff(COLOR_PAIR(2));
	}

	move(cursor_y, cursor_x);
	refresh();
}

const Move Display::get_player_move() {
	Move m;

	PieceType piece = state.get_piece_to_be_moved();

	bool from_chosen = false;

	move(cursor_y, cursor_x);

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
			default:
				continue;
		}

		set_error(NULL);

		cursor_x = CLAMP(0, cursor_x, 4);
		cursor_y = CLAMP(0, cursor_y, 4);
		move(cursor_y, cursor_x);
	}
player_input_done:
	
	return m;
}
