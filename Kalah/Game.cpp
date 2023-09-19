#include "Game.h"
#include "Side.h"
#include <iostream>
#include "Board.h"
#include "Player.h"
Game::Game(const Board& b, Player* south, Player* north) {
	gameboard = new Board(b);
	s_player = south;
	n_player = north;
}
void Game::display() const {
	int holes = gameboard->holes();
	for (int j = 0; j < 2 * holes; j++) {
		std::cout << " ";
	}
	std::cout << n_player->name() << std::endl;
	std::cout << " ";
	for (int i = 1; i <= holes; i++) {
		std::cout << " [" << gameboard->beans(NORTH, i) << "] ";
	}
	std::cout << std::endl;
	std::cout << gameboard->beans(NORTH, POT);
	for (int i = 0; i < 5 * holes; i++) {
		std::cout << " ";
	}
	std::cout << gameboard->beans(SOUTH, POT) << std::endl;
	std::cout << " ";
	for (int i = 1; i <= holes; i++) {
		std::cout << " [" << gameboard->beans(SOUTH, i) << "] ";
	}
	std::cout << std::endl;
	for (int j = 0; j < 2 * holes; j++) {
		std::cout << " ";
	}
	std::cout << s_player->name() << std::endl;
}
void Game::status(bool& over, bool& hasWinner, Side& winner) const {
	if ((gameboard->beansInPlay(SOUTH) == 0) || (gameboard->beansInPlay(NORTH) == 0)) {
		over = true;
	}
	else {
		over = false;
	}
	if (over) {
		int s_pot = gameboard->beans(SOUTH, POT);
		int n_pot = gameboard->beans(NORTH, POT);
		if (s_pot > n_pot) {
			hasWinner = true;
			winner = SOUTH;
		}
		else if (s_pot == n_pot) {
			hasWinner = false;
		}
		else {
			hasWinner = true;
			winner = NORTH;
		}
	}
}
bool Game::move(Side s) {
	bool is_over, winner;
	Side s_win;
	int lasthole;
	Side not_s = opponent(s);
	status(is_over, winner, s_win); // checks the current status of the game
	if (!is_over) {					// if the game isn't over yet
		Player* currplayer;			// 
		Player* opp;				//
		if (s == SOUTH) {			// if the current side is south
			currplayer = s_player;		   // 
			opp = n_player;				   //
			lasthole = gameboard->holes(); // south's last hole is the max hole of the board (i.e. hole 6 on a side of 6 holes)
		}
		else {						// if the current side is north
			currplayer = n_player;	//
			opp = s_player;			//
			lasthole = 1;			// north's last hole is the min/first hole of the board
		}
		int curr_pot = gameboard->beans(s, POT);
		int tobemove = currplayer->chooseMove(*gameboard, s); // ask the current player for their move choice
		if (tobemove == -1) {
			return false;
		}
		if (!currplayer->isInteractive()) {
			std::cout << currplayer->name() << " chooses hole " << tobemove << std::endl;
		}
		Side end_side;
		int end_hole = 0;
		gameboard->sow(s, tobemove, end_side, end_hole); // sow the board based on the move the player chose
		while ((end_side == s) && (end_hole == 0)) {	 // if the last hole the player sows on is their pot
			std::cout << currplayer->name() << " gets to take another turn." << std::endl;
			display();									 // display the board again	
			if (gameboard->beansInPlay(s) == 0) {		 // if there are beans on their side
				std::cout << currplayer->name() << " has no beans left to sow." << std::endl;
				std::cout << "Sweeping remaining beans into " << opp->name() << "'s pot." << std::endl;
				for (int i = 1; i <= gameboard->holes(); i++) {	// go through all the opponent's holes
					gameboard->moveToPot(not_s, i, not_s);			// move any/all of the beans from the opponent's holes to their pot
				}
				break;
			}
			tobemove = currplayer->chooseMove(*gameboard, s); // ask the current player for their move choice
			if (!currplayer->isInteractive()) {
				std::cout << currplayer->name() << " chooses hole " << tobemove << std::endl;
			}
			gameboard->sow(s, tobemove, end_side, end_hole);  // sow the board based on the move the player chose	
		}
		if ((end_side == s) && (end_hole != 0) && (beans(s, end_hole) == 1) && (beans(not_s, end_hole) > 0)) { //if the last hole that was sowed was previously empty, on the player's side, and hole across is not empty
			gameboard->moveToPot(not_s, end_hole, s); // move/capture the hole across by moving its beans to the current player's pot
			gameboard->moveToPot(s, end_hole, s);	  // move the beans from the last hole sowed to the current player's pot	
		}
		return true; // return true. the move was done
	}
	else {
		for (int i = 1; i <= gameboard->holes(); i++) { // if either side is empty
			gameboard->moveToPot(not_s, i, not_s);		// sweep all the beans on both sides into their respective pots
			gameboard->moveToPot(s, i, s);				//
		}
		return false;
	}
}
int Game::beans(Side s, int hole) const {
	return gameboard->beans(s, hole);
}
void Game::play() {
	bool over = false;
	Side Winner;
	bool hasWinner = false;
	status(over, hasWinner, Winner);					// check the status of the game
	bool notInteractive = ((!s_player->isInteractive()) && (!n_player->isInteractive()));
	while (!over) {										// while the game isn't over
		display();										// display the board	
		move(SOUTH);									// move for SOUTH
		status(over, hasWinner, Winner);				// check the status of the game after SOUTH's move
		display();										// display the new board
		move(NORTH);									// move for NORTH
		status(over, hasWinner, Winner);				// check the status of the game after NORTH's move
		if (notInteractive) {							// if both players are computers, ask the user to press enter
			std::cout << "Press ENTER to continue" << std::endl;
			std::cin.ignore(10000, '\n');
		}
	}
	display();											// display the final state of the board
	if (hasWinner) {
		if (Winner == SOUTH) {
			std::cout << "The winner is " << s_player->name() << std::endl;
		}
		else {
			std::cout << "The winner is " << n_player->name() << std::endl;
		}
	}
	else {
		std::cout << "The game is over, there is no winner." << std::endl;
	}


}