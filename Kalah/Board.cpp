#include "Board.h"
#include "Side.h"
#include <vector>
Board::Board(int nHoles, int nInitialBeansPerHole) {
	if (nHoles > 0) {
		sideholes = nHoles;
	}
	else {
		sideholes = 1;
	}
	if (nInitialBeansPerHole >= 0) {
		startbeans = nInitialBeansPerHole;
	}
	else {
		startbeans = 0;
	}
	for (int i = 0; i < sideholes; i++) {
		s_side.push_back(startbeans);
	}
	for (int j = 0; j < sideholes; j++) {
		n_side.push_back(startbeans);
	}
	beantotal = 2 * startbeans * sideholes;
	n_pot = 0;
	s_pot = 0;
}
Board::Board(const Board& copy) {
	this->sideholes = copy.sideholes;
	this->beantotal = copy.beantotal;
	this->startbeans = copy.startbeans;
	this->n_pot = copy.n_pot;
	this->s_pot = copy.s_pot;
	for (int i = 0; i < copy.sideholes; i++) {
		this->n_side.push_back(copy.n_side[i]);
	}
	for (int i = 0; i < copy.sideholes; i++) {
		this->s_side.push_back(copy.s_side[i]);
	}
}
int Board::holes() const {
	return sideholes;
}
int Board::totalBeans() const {
	return beantotal;
}
int Board::beans(Side s, int hole) const {
	if (s == NORTH) {
		if (hole == 0) {
			return n_pot;
		}
		if ((hole > sideholes) || (hole < 0)) {
			return -1;
		}
		return n_side[hole - 1];
	}
	if (hole == 0) {
		return s_pot;
	}
	if ((hole > sideholes) || (hole < 0)) {
		return -1;
	}
	return s_side[hole - 1];
}
int Board::beansInPlay(Side s) const {
	int sidetotal = 0;
	if (s == NORTH) {
		for (std::vector<int>::const_iterator n_ptr = n_side.begin(); n_ptr != n_side.end(); n_ptr++) {
			sidetotal += (*n_ptr);
		}
		return sidetotal;
	}
	for (std::vector<int>::const_iterator s_ptr = s_side.begin(); s_ptr != s_side.end(); s_ptr++) {
		sidetotal += (*s_ptr);
	}
	return sidetotal;
}
bool Board::sow(Side s, int hole, Side& endSide, int& endHole) { // Board sow function
	int currbeans = beans(s, hole);								 // find the # of beans in chosen hole
	if ((currbeans == -1) || (currbeans == 0)) {				 // if the hole is out of range or empty	
		return false;											 // return false	
	}
	if (s == SOUTH) {											 // empty the hole	
		s_side[hole - 1] = 0;
	}
	else {
		n_side[hole - 1] = 0;
	}
	Side currSide = s;
	int currHole = hole;
	bool firstsow = true;										// is this the first time sowing a side?
	while (currbeans > 0) {										// as long as there are beans to sow
		if (currSide == NORTH) {								// if the North side hasn't been sown yet
			currHole = sowNorth(currHole, currbeans, firstsow);	// sow the north side
			if (currbeans > 0) {								// if there are still beans left after sowing the North
				if (s == NORTH) {								// if the current side playing is north
					currbeans--;
					n_pot++;
				}												// add one to North's pot
				if ((currbeans == 0) && (s == NORTH)) {						// if the last bean sown was in North's pot
					currHole = 0;							// the hole we end on is the 0th hole/North's pot	
				}
				else {
					currSide = SOUTH;						// if there are still more beans left
					currHole = 1;							// switch sides and start sowing from the first hole on south's side
				}
			}
		}
		else {
			currHole = sowSouth(currHole, currbeans, firstsow); // sow the south side
			if (currbeans > 0) {								// if there are still beans left after sowing the South
				if (s == SOUTH) {								// if the current side playing is south
					currbeans--;
					s_pot++;
				}												// add one to South's pot
				if ((currbeans == 0) && (s == SOUTH)) {						// if the last bean sown was in South's pot
					currHole = 0;							// the hole we end on is the 0th hole/South's pot
				}
				else {
					currSide = NORTH;						// if there are still more beans left
					currHole = sideholes;					// switch sides and start sowing from the first hole on north's side
				}
			}
		}
	}
	endSide = currSide;											// make the endSide the lastSide we sowed on
	endHole = currHole;											// make the endHole the hole where the last bean was placed
	return true;
}
int Board::sowNorth(int hole, int& beans, bool& firstsow) {		// sowNorth function, sows the North side of the Board
	int currhole;
	if (firstsow) {											// if this the first side we're sowing on
		currhole = hole - 1;									// skipping the first hole on the first sow
		firstsow = false;										// changing firstsow to false so we don't skip holes if we need to sow more sides
	}
	else {
		currhole = hole;										// if this isn't the first side, start sowing on the first hole
	}
	while (currhole > 0 && beans > 0) {							// while the holes are in range and we have positive beans
		n_side[currhole - 1]++;									// increment our current hole's beans		
		beans--;												// decrement our beans left to sow
		if (beans > 0) {										// if we still have beans
			currhole--;											// move to the next hole
		}
	}
	return currhole;											// return last hole sowed
}

int Board::sowSouth(int hole, int& beans, bool& firstsow) {     // sowSouth function, sows the North side of the Board
	int currhole;
	if (firstsow) {												// if this the first side we're sowing on
		currhole = hole + 1;									// skipping the first hole on the first sow
		firstsow = false;										// changing firstsow to false so we don't skip holes if we need to sow more sides
	}
	else {
		currhole = hole;										// if this isn't the first side, start sowing on the first hole
	}
	while (currhole <= sideholes && beans > 0) {					// while the holes are in range and we have positive beans
		s_side[currhole - 1]++;									// increment our current hole's beans	
		beans--;												// decrement our beans left to sow
		if (beans > 0) {										// if we still have beans
			currhole++;											// move to the next hole
		}
	}
	return currhole;											// return last hole sowed
}

bool Board::moveToPot(Side s, int hole, Side potOwner) {
	if ((hole < 1) || (hole > sideholes)) {
		return false;
	}
	int movebeans = 0;
	if (s == NORTH) {
		movebeans = n_side[hole - 1];
		n_side[hole - 1] = 0;
	}
	else {
		movebeans = s_side[hole - 1];
		s_side[hole - 1] = 0;
	}
	if (potOwner == NORTH) {
		n_pot += movebeans;
	}
	else {
		s_pot += movebeans;
	}
	return true;
}
bool Board::setBeans(Side s, int hole, int beans) { //should this work for pots
	if ((hole < 0) || (hole > sideholes)) {
		return false;
	}
	else if (beans < 0) {
		return false;
	}
	if (s == NORTH) {
		if (hole == 0) {
			n_pot = beans;
		}
		else {
			n_side[hole - 1] = beans;
		}
	}
	else {
		if (hole == 0) {
			s_pot = beans;
		}
		else {
			s_side[hole - 1] = beans;
		}
	}
	beantotal = s_pot + n_pot;
	for (std::vector<int>::iterator total_ptr = s_side.begin(); total_ptr != s_side.end(); total_ptr++) {
		beantotal += (*total_ptr);
	}
	for (std::vector<int>::iterator total_ptr = n_side.begin(); total_ptr != n_side.end(); total_ptr++) {
		beantotal += (*total_ptr);
	}
	return true;
}
