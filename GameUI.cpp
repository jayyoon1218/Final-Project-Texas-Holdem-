#include "GameUI.h"
#include "HelperTools.h"
#include "Game.h"
#include <algorithm>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdexcept>
#include "StateMachine.h"

#define RAISE_SIZE 33
#define WIN_SIZE 17
#define FOLD_SIZE 12
#define EXIT_SIZE 8

using namespace std;

GameUI* GameUI::getInstance()
{
	if (!instance)
	{
		instance = new GameUI();
	}
	return instance;
}

GameUI::~GameUI() {}

void GameUI::clearScreen()
{
	system("clear");
}

void GameUI::printWelcome() const
{
	cout << "******************************" << endl;
	cout << "********TEXAS HOLD'EM*********" << endl;
	cout << "******************************" << endl;
	cout << "Welcome to the Texas Hold'Em game.\nType \"HELP\" for commands or just start playing by writing \"PLAY\"!" << endl; 
}

void GameUI::printStart() const
{
	string start = "How many players do you want the game to have (2-6)?\n";
	cout << start << endl;
}

void GameUI::printTurn(vector<Player*> players, Player* humanPlayer, size_t pot, vector<Card*> cards, size_t dealer, size_t sb, size_t bb) const
{ 

	Game* game = Game::getInstance();
	
	GameUI::print("\n");

	cout << "Round #" << PreFlop::getInstance()->roundCounter << "\t\t";

	string role = "";
	for (size_t i = 0; i < players.size(); i++)
	{
		if (players[i]->getId() == dealer){
			role = " (D) ";
		} 
		else {
			role = " (BB) ";
		}

		cout << "Player " << players[i]->getId() << role << "(" << players[i]->getMoney() << "$)" << "\t";

		if (i == players.size()-1){
			cout << "\t pot (" << pot << "$) \n";
		}
	}
	
	for (size_t i = 0; i < players.size(); i++)
	{
		if (players[i]->getId() == dealer)
			cout << "DEALER" << "\t\t";
		else if (players[i]->getId() == sb && players.size() != 2)
			cout << "SB" << "\t\t";
		else if (players[i]->getId() == bb)
			cout << "BB" << "\t\t";
	}
	
	if (humanPlayer != NULL)
	{
		cout << "Your cards:" << endl;
		vector<Card*> humanCards = humanPlayer->getHand();
	
		for (size_t i = 0; i < humanCards.size(); i++)
		{
			cout << humanCards[i]->toString() << endl;
		}
	}
	
	if (cards.size() > 0)
	{
		GameUI::print("Table cards:\n");

		for (size_t i = 0; i < cards.size(); i++)
		{
			GameUI::print(cards[i]->toString());
		}
		vector<Card*> firstCards = players[0]->getHand();
		vector<Card*> secondCards = players[1]->getHand();
		for (size_t i = 0; i < firstCards.size(); i++)
		{
			GameUI::print(firstCards[i]->toString());
			GameUI::print("\t\t\t");
		}
		GameUI::print("\n");

	} 
	else if (players.size() > 1)
	{
		GameUI::print("Table cards:\n");
		GameUI::print("\t\t\t");
		vector<Card*> firstCards = players[0]->getHand();
		vector<Card*> secondCards = players[1]->getHand();
		for (size_t i = 0; i < firstCards.size(); i++)
		{
			GameUI::print(firstCards[i]->toString());
			GameUI::print("\t\t\t");
		}
		
		GameUI::print("\n");		
	}

	GameUI::print("\n");

}

void GameUI::printExit(size_t winner, int arr []) const
{
	cout << "Winner is Player " << winner << ". Player 0 hands won: " << arr[0] << ". Player 1 hands won: " << arr[1] << endl;
}

void GameUI::printInput() const
{
	cout << "Input: ";
}

void GameUI::print(string str) const
{
	Game* game = Game::getInstance();
	cout << str;
}

void GameUI::printInputError() const
{
	if (gameStart == 0)
	{
		cout << "Type \"PLAY\"to play or commands with \"HELP\"" << endl;
	}
	else if (gameStart == 1)
	{
		cout << "Please type 2,3,..,6" << endl;
	}
	else if (gameStart > 1)
	{
		cout << "Please type a valid command, get commands with \"HELP\"" << endl;
	}
}

void GameUI::printHelp() const
{
	string help = "The general commands are: PLAY, QUIT, and HELP. Game commands are: RAISE, CALL, FOLD and ODDS.\n";
	cout << help << endl;
}

void GameUI::printRoundWinner(vector<Player*> winners) const
{

	Game* game = Game::getInstance();

	vector<Player*> active = Game::getInstance()->getActivePlayers();
	bool showHands = active.size() > 1;

	if (showHands)
	{
		for (size_t i = 0; i < active.size(); i++)
		{
			Player* p = active[i];
			vector<Card*> hand = p->getHand();
			vector<Card*> cards = p->getCards();
			int eq = calculateHandValue(cards)[1];
			eq = eqToInt(eq);
			string outcome;
			switch(eq)
			{
			case 9:
				outcome = "Straight-Flush";
				break;
			case 8:
				outcome = "Four of a kind";
				break;
			case 7:
				outcome = "Full house";
				break;
			case 6:
				outcome = "Flush";
				break;
			case 5:
				outcome = "Straight";
				break;
			case 4:
				outcome = "Three of a kind";
				break;
			case 3:
				outcome = "Two pairs";
				break;
			case 2:
				outcome = "One pair";
				break;
			case 1:
				outcome = "High card";
				break;
			}
		}

	}

	GameUI::print("\n");

	if (winners.size() > 1)
	{
		GameUI::print("Winners of this round are: ");
		for (size_t i = 0; i < winners.size(); i++)
		{
			GameUI::print(winners[i]->getName());
			if (i == winners.size() - 1)
				GameUI::print("!");
			else if (i == winners.size() - 2)
				GameUI::print(" and ");
			else
				GameUI::print(", ");
		}
	}
	else
	{
		GameUI::print(winners[0]->getName() + " wins this round!");
	}
}

void GameUI::printWinner(size_t winner) const
{
	Game* game = Game::getInstance();
	cout << "The winner of the game is PLAYER " << winner <<", congratulations!\n" << endl;
}

void GameUI::readInput()
{
	string kInput;
	cin >> kInput;
	transform(kInput.begin(), kInput.end(), kInput.begin(), ::toupper);
	cout << gameStart << endl;
	input = NONE;
		
	if (gameStart == 3){
		stringstream ss(kInput);
		ss >> timesToPlay;
		gameStart++;		
	}

	if (gameStart == 1)
	{
		if (kInput.compare("2") == 0 || kInput.compare("3") == 0 || kInput.compare("4") == 0 || kInput.compare("5") == 0|| kInput.compare("6") == 0)
		{
			stringstream ss(kInput);
			ss >> playerNum;
			gameStart++;
			input = NUMS;
		}
	}

	if (kInput.compare("PLAY") == 0)
	{
		input = PLAY;
		gameStart++;
	}

	if (kInput.compare("HELP") == 0)
	{
		input = HELP;
	}

	if (kInput.compare("QUIT") == 0)
	{
		cout << "Are you sure you want to quit (Y/N)?" << endl;
		cin >> kInput;
		transform(kInput.begin(), kInput.end(), kInput.begin(), ::toupper);

			if (kInput.compare("Y") == 0 || kInput.compare("YES") == 0)
			{
				cout << "BYE BYE!!";
				delete Game::getInstance();
				delete GameUI::getInstance();
				delete StateMachine::getInstance();
				
				exit(0);
			}
			if (kInput.compare("N") == 0 || kInput.compare("NO") == 0)
			{
				cout << "Lets keep playing then" << endl;
				input = EMPTY;
			}
			else  
			{
				cout << "Please type Y/N" << endl;
			}
		}
	}

	if (kInput == "CALL")
	{
		input = CALL;
	}

	if (kInput == "RAISE")
	{
		input = RAISE;
	}

	if (kInput == "FOLD")
	{
		input = FOLD;
	}

	if (kInput == "ODDS")
	{
		input = ODDS;
	}

}

void GameUI::setGameStart(size_t game)
{
	gameStart = game;
}

Command GameUI::playerActionPrompt()
{
	Game* game = Game::getInstance();
	Player* player = game->getPlayerById(0);
	cout << "\nYour status\n";
	cout << "Money: $" << player->getMoney() << "\tBet: $" << player->getBet() << endl;

	size_t raiseCost = game->getRaiseCost();
	size_t callCost = game->getCallCost();

	Command com;

	cout << "Please enter command (RAISE($" << raiseCost << "), CALL($" << callCost << "), FOLD or ODDS)!"<< endl;
	printInput();
	readInput();
	com = getLatestInput();
	if (com == EMPTY)
		continue;
	else if (com == ODDS)
	{
		vector<int> cards = calculateHandValue(player->getCards());
		vector<int> hand = calculateHandValue(player->getHand());
		printOdds(cards[0], game->getActivePlayers().size(), hand[0]);
	}
	else if (com != RAISE && com != CALL && com != FOLD)
	{
		print("Invalid command!");
	}	
	else
	{
		ask = false;
	}
	
	return com;
}

void GameUI::printAction(Player* player) const
{
	stringstream ss;

	ss << "Player " << player->getId();

	if (player->getMoney() == 0)
	{
		ss << " is all in";
	}
	else
	{
		switch (player->getLastCommand())
		{
		case RAISE:
			ss << " raises";
			break;
		case CALL:
			ss << " calls";
			break;
		case CHECK:
			ss << " checks";
			break;
		case FOLD:
			ss << " folds";
			break;
		case NONE:
			ss << " has folded";
			break;
		default:
			break;
		}
	}
	ss << " ($" << player->getBet() << "/$" << player->getMoney() << ")";

	string str;
	getline(ss, str);
	str = str + "\n";
	print(str);

}

void GameUI::printOdds(int rowVal, int players, int handRowVal) const
{
	vector<double> playerOdds = calculateOdds(rowVal, players, handRowVal);
	cout << "The odds:" << std::endl;
	cout << "*****************************************" << endl;
	cout << "\tWin:\t\t " << (playerOdds[0] - fmod(playerOdds[0], 0.01)) << "%" << endl;
	cout << "\tStraight-Flush:  " << (playerOdds[1] - fmod(playerOdds[1], 0.01)) << "%" << endl;
	cout << "\tFour of a kind:  " << (playerOdds[2] - fmod(playerOdds[2], 0.01)) << "%" << endl;
	cout << "\tFull House:\t " << (playerOdds[3] - fmod(playerOdds[3], 0.01)) << "%" << endl;
	cout << "\tFlush:\t\t " << (playerOdds[4] - fmod(playerOdds[4], 0.01)) << "%" << endl;
	cout << "\tStraight:\t " << (playerOdds[5] - fmod(playerOdds[5], 0.01)) << "%" << endl;
	cout << "\tThree of a kind: " << (playerOdds[6] - fmod(playerOdds[6], 0.01)) << "%" << endl;
	cout << "\tTwo pairs:\t " << (playerOdds[7] - fmod(playerOdds[7], 0.01)) << "%" << endl;
	cout << "\tOne pair:\t " << (playerOdds[8] - fmod(playerOdds[8], 0.01)) << "%" << endl;
	cout << "\tHigh card:\t " << (playerOdds[9] - fmod(playerOdds[9], 0.01)) << "%" << endl;
	cout << "*****************************************" << endl;

}

Command GameUI::getLatestInput() const
{
	return input;
}

int GameUI::getPlayerNum() const
{
	return playerNum;
}

int GameUI::getTimesToPlay() const
{
	return timesToPlay;
}
