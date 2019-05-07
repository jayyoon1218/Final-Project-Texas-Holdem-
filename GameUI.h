#include "Game.h"
#include <iostream>
#include <string>
#include "thdefs.h"
#include "Player.h"

using namespace std;

class GameUI
{
public:
	static GameUI* getInstance();
	~GameUI();

	void clearScreen();
	
	void printWelcome() const;

	void printStart() const;

	void printTurn(vector<Player*> players, Player* humanPlayer, size_t pot, vector<Card*> cards, size_t dealer, size_t sb, size_t bb) const;

	void printExit(size_t winner, int arr []) const;

	void printInput() const;

	void printInputError() const;

	void printHelp() const;
	
	void printRoundWinner(vector<Player*> winners) const;

	void printWinner(size_t winner) const;

	void print(string) const;

	void readInput();

	bool confirm(string prompt) const;
	
	void setGameStart(size_t game);

	Command playerActionPrompt();

	void printAction(Player* player) const;

	void printOdds(int rowVal, int players, int handRowVal) const;
	
	Command getLatestInput() const;

	int getPlayerNum() const;
	
	int getTimesToPlay() const;
private:
	GameUI() : input(NONE), gameStart(0), playerNum(2), timesToPlay(1) {}
	static GameUI* instance;

	Command input;
	size_t gameStart;
	int playerNum;
	int timesToPlay;
};
