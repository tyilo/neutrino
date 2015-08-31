#include <iostream>

#include <unistd.h>

#include "neutrino.h"
#include "ai.h"
#include "display.h"
#include "macros.h"

using namespace std;

Move ai_get_move(State &s, Display &d) {
	sleep(1);
	return AI(s).get_best_move(6);
}

Move player_get_move(State &s, Display &d) {
	return d.get_player_move();
}

typedef typeof(player_get_move) *move_func;

void play_game(move_func white_move, move_func black_move) {
	State s = State();
	Display d = Display(s);

	while(s.get_winner() == None) {
		move_func f = (s.turn % 4 <= 1)? white_move: black_move;

		Move m;
		bool next_move_invalid = false;

		do {
			if(!next_move_invalid) {
				next_move_invalid = true;
			} else {
				d.set_error("Invalid move!");
			}

			d.update();

			m = f(s, d);
		} while(!s.is_valid_move(m));

		s.perform_move(m);
	}

	d.update();

	getch();
}

void choose_players(move_func *white_move, move_func *black_move) {
	cout << "1) Human vs Human\n";
	cout << "2) Human vs AI\n";
	cout << "3) AI vs Human\n";
	cout << "4) AI vs AI\n";
	cout << "\n";

	int choice;
	bool first = true;
	do {
		if(first) {
			first = false;
		} else {
			cerr << "Please enter a valid number.\n";
		}
		cout << "Select players (White vs Black): ";
		do {
			errno = 0;
			cin.clear();
			cin >> choice;
		} while(cin.eof() && errno == EINTR);

		if(!cin.good()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	} while(!BETWEEN(1, choice, 4));

	*white_move = choice <= 2? &player_get_move: &ai_get_move;
	*black_move = choice % 2 == 1? &player_get_move: &ai_get_move;
}

int main(int argc, const char * argv[]) {
	move_func white_move, black_move;
	while(true) {
		choose_players(&white_move, &black_move);
		play_game(white_move, black_move);
	}

	return 0;
}
