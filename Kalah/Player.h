#include"Side.h"
#include<string>
#include<vector>
#ifndef PLAYER_H
#define PLAYER_H
class Board;
class Player {
public:
	Player(std::string name);
	std::string name() const;
	virtual bool isInteractive() const;
	virtual int chooseMove(const Board& b, Side s) const = 0;
	virtual ~Player();
private:
	std::string m_name;
};
class HumanPlayer : public Player {
public:
	HumanPlayer(std::string name);
	virtual bool isInteractive() const;
	virtual int chooseMove(const Board& b, Side s) const;
	virtual ~HumanPlayer();
};
class BadPlayer : public Player {
public:
	BadPlayer(std::string name);
	virtual int chooseMove(const Board& b, Side s) const;
	virtual ~BadPlayer();
};
class SmartPlayer : public Player {
public:
	SmartPlayer(std::string name);
	virtual int chooseMove(const Board& b, Side s) const;
	virtual ~SmartPlayer();
private:
	int evaltree(Side currside, Board& currstate, int depth) const;
	int heuristic(Board& state) const;
	int compare(int eval1, int eval2, Side currside) const;
};
#endif
