#include "Side.h"
#include<vector>
#ifndef BOARD_H
#define BOARD_H
class Board {
public:
	Board(int nHoles, int nInitialBeansPerHole);
	Board(const Board& copy);
	int holes() const;
	int beans(Side s, int hole) const;
	int beansInPlay(Side s) const;
	int totalBeans() const;
	bool sow(Side s, int hole, Side& endSide, int& endHole);
	bool moveToPot(Side s, int hole, Side potOwner);
	bool setBeans(Side s, int hole, int beans);
private:
	int sowSouth(int hole, int& beans, bool& firstsow);
	int sowNorth(int hole, int& beans, bool& firstsow);
	std::vector<int> s_side;
	std::vector<int> n_side;
	int sideholes;
	int beantotal;
	int startbeans;
	int n_pot;
	int s_pot;
};
#endif
