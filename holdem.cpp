#include "Game.h"
#include <cstdio>
#include <iostream>
#include "GameUI.h"
#include "HelperTools.h"
#include "StateMachine.h"
#include "thdefs.h"
#include <unistd.h>

using namespace std;

int main()
{

	GameUI* gameUI = GameUI::getInstance();
	gameUI->clearScreen();
	gameUI->printWelcome();

        while (gameUI->getLatestInput() != PLAY)
        {
                gameUI->printInput();
                gameUI->readInput();
                if (gameUI->getLatestInput() == HELP)
                        gameUI->printHelp();
                else if (gameUI->getLatestInput() != PLAY)
                        gameUI->printInputError();
	}
        gameUI->printStart();
        while (gameUI->getLatestInput() != NUMS)
        {
                gameUI->printInput();
                gameUI->readInput();
                if (gameUI->getLatestInput() != NUMS)
			gameUI->printInputError();
        }
	if (gameUI->getPlayerNum() == 2)
	{
		gameUI->printInput();
		gameUI->readInput();
	}

	gameUI->clearScreen();
	
	int stopper = gameUI->getTimesToPlay();

	int p1Wins = 0;
	int p2Wins = 0;

	for (int index = 0; index < stopper; index++)
	{
		Game* game = 0;
		StateMachine* stateMachine = 0;

		game = Game::startGame(gameUI->getPlayerNum(), 5000, 50, 100);
		
		stateMachine = StateMachine::getInstance();

		gameUI->printTurn(game->getPlayers(), game->getPlayerById(0), game->getPot(), game->getTable(), game->getDealerId(), game->getSmallBlindId(), game->getBigBlindId());
		
		int checker = 0;
		while (checker < 1)
		{
			gameUI->printTurn(game->getPlayers(), game->getPlayerById(0), game->getPot(), game->getTable(), game->getDealerId(), game->getSmallBlindId(), game->getBigBlindId());
		        gameUI->printWinner(game->getPlayers()[0]->getId());
			int arr [2] = { Win::getInstance()->handsWon[0], Win::getInstance()->handsWon[1]};
		        gameUI->printExit(game->getPlayers()[0]->getId(), arr);
			if (game->getPlayers()[0]->getId() == 0)
			{		
				p1Wins++;
			}
			else
			{
				p2Wins++;
				checker = 1;
		        }

		}

	}

	cout << "PLayer 0 wins: " << p1Wins << endl;
	cout << "PLayer 1 wins: " << p2Wins << endl;

}

