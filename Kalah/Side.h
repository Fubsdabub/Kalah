#ifndef SIDE_H
#define SIDE_H
class Board;
enum Side { NORTH, SOUTH };

const int NSIDES = 2;
const int POT = 0;

inline
Side opponent(Side s)
{
	return Side(NSIDES - 1 - s);
}
bool simMove(Board& gameboard, Side s, int tobemove); // custom function, simMove header

#endif
