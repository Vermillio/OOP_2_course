#pragma once

#include <string>
#include <iostream>
#include <vector>

using namespace std;

namespace seabattle {

	enum orient {
		ho = 0,
		ve = 1
	};

	enum letters {
		a = 0,
		b = 1,
		c = 2, 
		d = 3,
		e = 4,
		f = 5,
		g = 6,
		h = 7,
		i = 8,
		j =	9
	};

	struct ship {
		bool orientation;
		size_t pos_x;
		size_t pos_y;
		ship(bool _orientation, size_t _pos_x, size_t _pos_y) : orientation(_orientation), pos_x(_pos_x), pos_y(_pos_y) {};
	};

	struct cell {
		bool is_ship;
		bool is_checked;
		bool is_dead;
		cell() : is_ship(0), is_checked(0), is_dead(0) {};
	};

	class SeaBattleGame 
	{
	private:
		int SHIPS_NUM = 10;
		int FIELD_SIZE = 10;

		bool first_player_turn;
		size_t stepNum;

		int ships_left_1;
		int ships_left_2;
		vector<vector<cell>> field_1;
		vector<vector<cell>> field_2;

		bool isChecked(int target_x, int target_y);
		bool isShip(int target_x, int target_y);
		bool isDead(int target_x, int target_y);

		//0 - hit, 1 - dead
		bool just_killed(int target_x, int target_y);

		void mark_killed(int target_x, int target_y);
	public:

		void print_fields();
		int end_of_game();
		
		//ships descending by size
		bool set_fields(vector<ship> ships_1, vector<ship> ships_2);

		//0 - unable to do step, 1 - miss, 2 - hit, 3 - killed
		int step(int target_x, int target_y);

		SeaBattleGame() : first_player_turn(true), stepNum(0), ships_left_1(SHIPS_NUM), ships_left_2(SHIPS_NUM)
		{
			field_1.resize(FIELD_SIZE);
			field_2.resize(FIELD_SIZE);
			for (size_t i = 0; i < FIELD_SIZE; ++i)
			{
				field_1[i].resize(FIELD_SIZE);
				field_2[i].resize(FIELD_SIZE);
			}
		};
	};
}

bool seabattle::SeaBattleGame::isChecked(int x, int y)
{
	return (x >= 0 && x < FIELD_SIZE && y >= 0 && y < FIELD_SIZE &&
		((first_player_turn && field_2[x][y].is_checked) || (!first_player_turn && field_1[x][y].is_checked)));
}

bool seabattle::SeaBattleGame::isShip(int x, int y)
{
	return (x >= 0 && x < FIELD_SIZE && y >= 0 && y < FIELD_SIZE &&
		((first_player_turn && field_2[x][y].is_ship) || (!first_player_turn && field_1[x][y].is_ship)));
}

bool seabattle::SeaBattleGame::isDead(int x, int y)
{
	return (x >= 0 && x < FIELD_SIZE && y >= 0 && y < FIELD_SIZE &&
		((first_player_turn && field_2[x][y].is_dead) || (!first_player_turn && field_1[x][y].is_dead)));
}

bool seabattle::SeaBattleGame::just_killed(int x, int y)
{
	if (isShip(x, y) && isChecked(x, y))
	{
		if (isShip(x, y - 1) || isShip(x, y + 1))
		{
			int shift = 1;
			//check up
			while (isShip(x, y - shift) && isChecked(x, y - shift))
				++shift;
			if (isShip(x, y - shift) && !isChecked(x, y - shift))
				return false;

			shift = 1;
			//check down
			while (isShip(x, y + shift) && isChecked(x, y + shift))
				++shift;
			if (isShip(x, y + shift) && !isChecked(x, y + shift))
				return false;


			mark_killed(x, y);
			return true;
		}
		else if (isShip(x - 1, y) || isShip(x + 1, y))
		{
			int shift = 1;
			//check left
			while (isShip(x - shift, y) && isChecked(x - shift, y))
				++shift;
			if (isShip(x - shift, y) && !isChecked(x - shift, y))
				return false;

			shift = 1;
			//check right
			while (isShip(x + shift, y) && isChecked(x + shift, y))
				++shift;
			if (isShip(x + shift, y) && !isChecked(x + shift, y))
				return false;

			mark_killed(x, y);
			return true;
		}
		else {
			mark_killed(x, y);
			return true;
		}
	}
	else return false;
}

void seabattle::SeaBattleGame::mark_killed(int x, int y)
{

	if (first_player_turn)
	{
		field_2[x][y].is_dead = true;
		//mark down
		int shift = 1;
		while (isShip(x, y + shift))
		{
			field_2[x][y + shift].is_dead = true;
			++shift;
		}
		//mark up
		shift = 1;
		while (isShip(x, y - shift))
		{
			field_2[x][y - shift].is_dead = true;
			++shift;
		}
		//mark left
		shift = 1;
		while (isShip(x + shift, y))
		{
			field_2[x + shift][y].is_dead = true;
			++shift;
		}
		//mark right
		shift = 1;
		while (isShip(x - shift, y))
		{
			field_2[x - shift][y].is_dead = true;
			++shift;
		}
	}
	else {
		field_1[x][y].is_dead = true;

		//mark down
		int shift = 1;
		while (isShip(x, y + shift))
		{
			field_1[x][y + shift].is_dead = true;
			++shift;
		}
		//mark up
		shift = 1;
		while (isShip(x, y - shift))
		{
			field_1[x][y - shift].is_dead = true;
			++shift;
		}
		//mark left
		shift = 1;
		while (isShip(x + shift, y))
		{
			field_1[x + shift][y].is_dead = true;
			++shift;
		}
		//mark right
		shift = 1;
		while (isShip(x - shift, y))
		{
			field_1[x - shift][y].is_dead = true;
			++shift;
		}
	}
}

void seabattle::SeaBattleGame::print_fields()
{
	for (int i = 0; i < FIELD_SIZE; ++i)
	{
		cout << "|";
		for (int j = 0; j < FIELD_SIZE; ++j)
		{
			if (field_1[j][i].is_ship)
			{
				if (field_1[j][i].is_checked)
					if (field_1[j][i].is_dead)
						cout << "#";
					else cout << "+";
				else cout << "o";
			}
			else {
				if (field_1[j][i].is_checked)
					cout << ".";
				else cout << "?";
			}
		}
		cout << "|" << endl;
	}
	cout << endl;
	cout << endl;
	for (int i = 0; i < FIELD_SIZE; ++i)
	{
		cout << "|";
		for (int j = 0; j < FIELD_SIZE; ++j)
		{
			if (field_2[j][i].is_ship)
			{
				if (field_2[j][i].is_checked)
					if (field_2[j][i].is_dead)
						cout << "#";
					else cout << "+";
				else cout << "o";
			}
			else {
				if (field_2[j][i].is_checked)
					cout << ".";
				else cout << "?";
			}
		}
		cout << "|" << endl;
	}
	cout << endl;
}

int seabattle::SeaBattleGame::end_of_game()
{
	return	ships_left_1 == 0 ? 1 :
		ships_left_2 == 0 ? 2 : 0;
}

bool seabattle::SeaBattleGame::set_fields(vector<ship> ships_1, vector<ship> ships_2)
{
	if (ships_1.size() != SHIPS_NUM || ships_2.size() != SHIPS_NUM)
		return false;

	//init field_1
	int cur_sz = 4;
	int ships_of_cur_sz = 1;
	int ships_of_cur_sz_left = 1;
	for (auto ship : ships_1)
	{
		if (ship.orientation == ve)
			for (int j = 0; j < cur_sz; ++j)
			{
				field_1[ship.pos_x][ship.pos_y + j].is_ship = true;
			}
		else
			for (int j = 0; j < cur_sz; ++j)
			{
				field_1[ship.pos_x + j][ship.pos_y].is_ship = true;
			}
		--ships_of_cur_sz_left;
		if (ships_of_cur_sz_left == 0)
		{
			--cur_sz;
			ships_of_cur_sz_left = ++ships_of_cur_sz;
		}
	}

	//init field_2
	cur_sz = 4;
	ships_of_cur_sz = 1;
	ships_of_cur_sz_left = 1;
	for (auto ship : ships_2)
	{
		if (ship.orientation == ve)
			for (int j = 0; j < cur_sz; ++j)
			{
				field_2[ship.pos_x][ship.pos_y + j].is_ship = true;
			}
		else
			for (int j = 0; j < cur_sz; ++j)
			{
				field_2[ship.pos_x + j][ship.pos_y].is_ship = true;
			}
		--ships_of_cur_sz_left;
		if (ships_of_cur_sz_left == 0)
		{
			--cur_sz;
			ships_of_cur_sz_left = ++ships_of_cur_sz;
		}
	}

	return true;
}

int seabattle::SeaBattleGame::step(int target_x, int target_y)
{
	if (target_x < 0 || target_x>9 || target_y < 0 || target_y>9)
		return 0;
	if (!isShip(target_x, target_y))
	{
		if (isChecked(target_x, target_y))
			return 0;

		if (first_player_turn)
			field_2[target_x][target_y].is_checked = 1;
		else
			field_1[target_x][target_y].is_checked = 1;
		first_player_turn = !first_player_turn;
		++stepNum;
		return 1;
	}
	else
	{
		if (isChecked(target_x, target_y))
			return 0;

		if (first_player_turn)
			field_2[target_x][target_y].is_checked = 1;
		else
			field_1[target_x][target_y].is_checked = 1;
		bool killed = just_killed(target_x, target_y);
		if (killed)
		{
			if (first_player_turn)
				--ships_left_2;
			else
				--ships_left_1;
		}

		first_player_turn = !first_player_turn;
		++stepNum;
		return 2;
	}
}
