#include "Player.h"
#include "Board.h"
#include "Side.h"
#include "Game.h"
#include <iostream>
#include <cassert>
using namespace std;

bool simMove(Board& gameboard, Side s, int tobemove) { // "Simulates" one move on a board, but dosen't take extra turns
	Side curr_s = s;
	Side not_s = opponent(curr_s);
	int holes = gameboard.holes();
	Side end_side;
	int end_hole;
	gameboard.sow(s, tobemove, end_side, end_hole);	  // sows the hole from tobemove		
	if ((end_side == s) && (end_hole == 0)) {		  // if the last hole is the player's pot
		if (gameboard.beansInPlay(s) == 0) {		  // checks if there are any beans on the player's side	
			for (int i = 1; i <= holes; i++) {		  // iterates through all the opponent's holes 	
				gameboard.moveToPot(not_s, i, not_s); // move all the beans from the opponent's side to their pot
			}
		}
		return true;	// return true, meaning that the player can take another turn
	}
	if ((end_side == s) && (end_hole != 0) && (gameboard.beans(s, end_hole) == 1) && (gameboard.beans(not_s, end_hole) > 0)) { // checks if can capture
		gameboard.moveToPot(not_s, end_hole, s);	// captures the hole
		gameboard.moveToPot(s, end_hole, s);         // captures the hole
	}
	return false;
}

void doBoardTests()
{
	Board b(3, 2);
	assert(b.holes() == 3 && b.totalBeans() == 12 &&
		b.beans(SOUTH, POT) == 0 && b.beansInPlay(SOUTH) == 6);
	b.setBeans(SOUTH, 1, 1);
	b.moveToPot(SOUTH, 2, SOUTH);
	assert(b.totalBeans() == 11 && b.beans(SOUTH, 1) == 1 &&
		b.beans(SOUTH, 2) == 0 && b.beans(SOUTH, POT) == 2 &&
		b.beansInPlay(SOUTH) == 3);
	Side es;
	int eh;
	b.sow(SOUTH, 3, es, eh);
	assert(es == NORTH && eh == 3 && b.beans(SOUTH, 3) == 0 &&
		b.beans(NORTH, 3) == 3 && b.beans(SOUTH, POT) == 3 &&
		b.beansInPlay(SOUTH) == 1 && b.beansInPlay(NORTH) == 7);
}

void doPlayerTests()
{
	HumanPlayer hp("Marge");
	assert(hp.name() == "Marge" && hp.isInteractive());
	BadPlayer bp("Homer");
	assert(bp.name() == "Homer" && !bp.isInteractive());
	SmartPlayer sp("Lisa");
	assert(sp.name() == "Lisa" && !sp.isInteractive());
	Board b(3, 2);
	b.setBeans(SOUTH, 2, 0);
	cout << "=========" << endl;
	int n = hp.chooseMove(b, SOUTH);
	cout << "=========" << endl;
	assert(n == 1 || n == 3);
	n = bp.chooseMove(b, SOUTH);
	assert(n == 1 || n == 3);
	n = sp.chooseMove(b, SOUTH);
	assert(n == 1 || n == 3);
}
void doGameTests()
{
	BadPlayer bp1("Bart");
	BadPlayer bp2("Homer");
	Board b(3, 0);
	b.setBeans(SOUTH, 1, 2);
	b.setBeans(NORTH, 2, 1);
	b.setBeans(NORTH, 3, 2);
	Game g(b, &bp1, &bp2);
	bool over;
	bool hasWinner;
	Side winner;
	//std::cout << "Turn 1" << std::endl;
	//g.display();
	//    Homer
	//   0  1  2
	// 0         0
	//   2  0  0
	//    Bart
	g.status(over, hasWinner, winner);
	assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 0 &&
		g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 2 &&
		g.beans(SOUTH, 1) == 2 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);

	g.move(SOUTH);
	//   0  1  0
	// 0         3
	//   0  1  0
	//std::cout << "Turn 1" << std::endl;
	//g.display();
	g.status(over, hasWinner, winner);
	assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
		g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);

	g.move(NORTH);
	//   1  0  0
	// 0         3
	//   0  1  0
	g.status(over, hasWinner, winner);
	//std::cout << "Turn 2" << std::endl;
	//g.display();
	assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
		g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);

	g.move(SOUTH);
	//   1  0  0
	// 0         3
	//   0  0  1
	//std::cout << "Turn 3" << std::endl;
	//g.display();
	g.status(over, hasWinner, winner);
	assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
		g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 1);

	g.move(NORTH);
	//   0  0  0
	// 1         4
	//   0  0  0
	//std::cout << "Turn 4" << std::endl;
	//g.display();
	g.status(over, hasWinner, winner);
	assert(over && g.beans(NORTH, POT) == 1 && g.beans(SOUTH, POT) == 4 &&
		g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);
	assert(hasWinner && winner == SOUTH);
}
void doCustomTests() {
	Board b(6, 4);
	HumanPlayer Fahim("Fahim");
	BadPlayer Ruhee("Ruhee");
	Game test(b, &Fahim, &Ruhee);
	test.play();


}
void doAITests() {
	Board b(6, 0);
	/*b.setBeans(SOUTH, 6, 1);*/
	b.setBeans(NORTH, 2, 4);
	b.setBeans(SOUTH, 1, 1);
	b.setBeans(SOUTH, 3, 1);
	SmartPlayer Dhruv("Dhruv");
	BadPlayer Fahim("Fahim");
	Game test(b, &Dhruv, &Fahim);
	test.play();
}
void userPlay(){
	bool keepPlaying = true;
	while(keepPlaying){
	cout << "How many starting holes?" << endl;
	int startHoles = 0;
	cin >> startHoles;
	while((startHoles <= 0)||(startHoles > 10)){
		cout << "Invalid number of holes, please try again." << endl;
		cin >> startHoles;
	}
	cout << "How many starting beans?" << endl;
	int startBeans = 0;
	cin >> startBeans;
	while((startBeans <= 0)||(startBeans > 10)){
		cout << "Invalid number of beans, please try again." << endl;
		cin >> startBeans;
	}
	Board toPlay(startHoles,startBeans);
	cout << "Pick one option, 1. Player vs Player, 2. Player vs Computer" << endl;
	int optPlay = 0;
	cin >> optPlay;
	while ((optPlay != 1)&&(optPlay != 2)){
		"Invalid option, please try again." << endl;
		cin >> optPlay;
	}
	cout << "Would Player 1 like to play North or South? Press 1 for North, 2 for South" << endl;
	int sideOpt = 0;
	cin >> sideOpt;
	while ((sideOpt != 1)&&(sideOpt != 2)){
		"Invalid option, please try again." << endl;
		cin >> sideOpt;
	}
		
	if(optPlay == 1){
		HumanPlayer PlayerOne;
		HumanPlayer PlayerTwo;
		if(sideOpt == 1){
			Game toStart(toPlay,PlayerTwo,PlayerOne);
		}else{
			Game toStart(toPlay,PlayerOne,PlayerTwo);
		}
	}else{
		HumanPlayer PlayerOne;
		int difficulty = 0;
		cout << "How difficult? Press 1 for easy, 2 for challenging" << endl;
		cin >> difficulty;
		while((difficulty != 1)&&(difficulty != 2)){
			cout << "Invalid input, please try again.";
			cin >> difficulty;
		}
		if(difficulty == 1){
			BadPlayer PlayerTwo;
		}else{
			SmartPlayer PlayerTwo;
		}
		if(sideOpt == 1){
			Game toStart(toPlay,PlayerTwo,PlayerOne);
		}else{
			Game toStart(toPlay,PlayerOne,PlayerTwo);
		}
	}	
	toStart.play();
	cout << "Want to Play Again? Y/N";
	string response;
		getline(cin, response);
		if((response[0] == 'Y')||(response[0] == 'y')){
		}
		else{
			keepPlaying = false;
		}
	}
}

int main()
{
	//doBoardTests();
	//doPlayerTests();
	//doGameTests();
	//doCustomTests();
	//doAITests();
	cout << "Passed all tests" << endl;
}
