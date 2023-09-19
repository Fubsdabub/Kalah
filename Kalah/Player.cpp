#include"Side.h"
#include"Player.h"
#include"Board.h"
#include<limits>
#include<climits>
#include<iostream>
Player::Player(std::string name) :m_name(name) {}

std::string Player::name() const { return m_name; }

bool Player::isInteractive() const { return false; }

HumanPlayer::HumanPlayer(std::string name) : Player(name) {}

bool HumanPlayer::isInteractive() const { return true; }

int HumanPlayer::chooseMove(const Board& b, Side s) const { // HumanPlayer Choose Move Function: takes in the player input, checks if its in range
	int validspot = -1;									   // and checks if there are beans in that hole. and keeps prompting the user if the hole 		
	int beansinspot = 0;								   // selected is invalid	
	while ((validspot == -1) || (beansinspot == 0)) {
		std::cout << "Select a hole, " << this->Player::name() << ": ";
		std::cin >> validspot;
		if ((validspot < 1) || (validspot > b.holes())) {	 // checks if the hole is in range	
			validspot = -1;
			std::cout << "The hole number must be from 1 to " << b.holes() << "." << std::endl;
		}
		else {
			beansinspot = b.beans(s, validspot);
			if (beansinspot == 0) {                      // checks if there are beans in the hole   
				std::cout << "There are no beans in that hole." << std::endl;
			}
		}
	}
	return validspot;
}
Player::~Player() {}
HumanPlayer::~HumanPlayer() {}
BadPlayer::~BadPlayer() {}
SmartPlayer::~SmartPlayer() {}

BadPlayer::BadPlayer(std::string name) : Player(name) {}

int BadPlayer::chooseMove(const Board& b, Side s) const { // BadPlayer Choose Move Function: Essentially picks the right most valid spot to move from
	int validspot = -1;
	int beansinspot = 0;
	int numholes = b.holes();
	for (int i = numholes; i >= 1; i--) {
		beansinspot = b.beans(s, i);
		if (beansinspot > 0) {
			validspot = i;
			break;
		}
	}
	return validspot;
}
SmartPlayer::SmartPlayer(std::string name) : Player(name) {}
int SmartPlayer::chooseMove(const Board& b, Side s) const { // SmartPlayer Choose Move Function
	if (b.beansInPlay(s) == 0) {							// If there are no holes that have beans, return -1
		return -1;
	}
	Side currside = s;
	int bestopt;
	if (s == SOUTH) {										// if the SmartPlayer is playing south
		bestopt = INT_MIN;									// set the best option/value to the minimum
	}
	else {													// if the SmartPlayer is playing north
		bestopt = INT_MAX;									// set the best option/value to the maximum
	}
	int bestmove = -1;										// bestmove is set to -1 by default
	int curropt;
	Side oppside = opponent(currside);
	int holes = b.holes();
	for (int i = 1; i <= holes; i++) {						// lopp to iterate through all the possible holes
		if (b.beans(s, i) > 0) {							// if there are beans in a hole
			Board copy(b);									// make a copy of the current board
			bool repeat = simMove(copy, currside, i);		// simulate moving from the current hole on the board copy
			if (repeat) {									// if that move leads to the SmartPlayer being able to take another turn
				curropt = evaltree(currside, copy, 0);		// send to the evaltree (basically the minimax function) starting with the copy of the board, the starting side, and depth of 0 as the turn isn't over
			}
			else {											// else
				curropt = evaltree(oppside, copy, 1);		// send to the evaltree/minimax function calling the opposite side, same copy of the board, and a depth of 1
			}
			if ((curropt > bestopt) && (s == SOUTH)) {		// if the value returned by the evaltree is greater than the best options value & its for south (which should be true for the first iteration)
				bestopt = curropt;							// change the best hole's value to the current hole's value	
				bestmove = i;								// the best hole to move from is now the current hole
			}
			else if ((curropt < bestopt) && (s == NORTH)) {	// if the value returned by the evaltree is less than the best option value & the side is north (Which should be true the first iteration)
				bestopt = curropt;							// change the best hole's value to the current hole's value	
				bestmove = i;								// the best hole to move from is now the current hole
			}
		}
	}
	return bestmove;										// return the best hole to move from for SmartPlayer
}
int SmartPlayer::evaltree(Side currside, Board& currstate, int depth) const {
	if (depth == 3) {										// if the tree reaches a depth of 3 or its max depth
		return heuristic(currstate);						// returns the heuristic of the current board
	}
	if (currstate.beansInPlay(currside) == 0) {				// if there is no hole with beans to play from
		return heuristic(currstate);						// return the heuristic of the current board
	}
	Side oppside = opponent(currside);
	int currbeans = 0;
	int curr_val = heuristic(currstate);
	int next_val;
	int holes = currstate.holes();
	for (int i = 1; i <= holes; i++) {                      // iterate through all the holes
		currbeans = currstate.beans(currside, i);			// 
		if (currbeans > 0) {								// if there are beans in the hole	
			Board copy(currstate);							// create a copy of the current board
			bool repeat = simMove(copy, currside, i);			// simulate a Move from that hole
			if (repeat) {									// if that move lets the side take another turn
				next_val = evaltree(currside, copy, depth);	// recur with the same side, copy of the board, and unchanged depth as their turn isn't over yet
			}
			else {
				next_val = evaltree(oppside, copy, depth + 1); // else recur for the opposite side
			}
			curr_val = compare(curr_val, next_val, currside);	// compare the evaluations of the next turn and the current turn 
		}
	}
	return curr_val;	// return the better turn for the current side
}
int SmartPlayer::heuristic(Board& state) const { // heuristic/evaluation of the board state
	int south_pot = state.beans(SOUTH, POT);	// find how many beans are in the south pot
	int north_pot = state.beans(NORTH, POT);	// find how many beans are in the north pot
	return south_pot - north_pot;
}
int SmartPlayer::compare(int eval1, int eval2, Side currside) const {
	if (currside == SOUTH) {	// if the side is SOUTH
		if (eval1 > eval2) {	// find the greater of the two values
			return eval1;		// if eval1 is greater, return that
		}
		else {
			return eval2;		// else return eval2
		}
	}
	else {						// else, with the side being NORTH	
		if (eval1 > eval2) {	// find the lesser of the two values
			return eval2;		// if eval2 is lesser, return that
		}
		else {					// else return eval1
			return eval1;
		}
	}
}