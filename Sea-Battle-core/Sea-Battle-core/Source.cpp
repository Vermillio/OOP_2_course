#include "SeaBattle.h"
#include <iostream>

using namespace seabattle;
using namespace std;

int main()
{
	vector<ship> ships_1 = {
		ship(ve, 1, a),
		ship(ho, 0, f),
		ship(ho, 4, f),
		ship(ve, 5, a),
		ship(ve, 6, i),
		ship(ho, 8, a),
		ship(ho, 4, d),
		ship(ho, 9, c),
		ship(ho, 1, h),
		ship(ho, 9, i)
	};

	vector<ship> ships_2 = {
		ship(ve, 1, a),
		ship(ho, 0, f),
		ship(ho, 4, f),
		ship(ve, 5, a),
		ship(ve, 6, i),
		ship(ho, 8, a),
		ship(ho, 4, d),
		ship(ho, 9, c),
		ship(ho, 1, h),
		ship(ho, 9, i)
	};

	SeaBattleGame game;
	game.set_fields(ships_1, ships_2);
	int x, y;
	while (!game.end_of_game())
	{
		cin >> x;
		cin >> y;
		game.step(x, y);
		system("cls");
		game.print_fields();
	}
	int winner = game.end_of_game();
	if (winner == 1)
		cout << "Player 1 won.";
	else cout << "Player 2 won.";
	system("pause");
	return 0;
}