#include "Side.h"
#include "Board.h"
#ifndef GAME_H
#define GAME_H
class Board;
class Player;
class Game {
public:
	Game(const Board& b, Player* south, Player* north);
	void display() const;
	void status(bool& over, bool& hasWinner, Side& winner) const;
	bool move(Side s);
	void play();
	int beans(Side s, int hole) const;
private:
	Board* gameboard = nullptr;
	Player* s_player = nullptr;
	Player* n_player = nullptr;
};

#endif
