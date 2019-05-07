#include "StateMachine.h"
#include "GameStates.h"
#include "Game.h"
#include "GameUI.h"
#include "HelperTools.h"
#include "State.h"
using namespace std;
void State::executeRound()
{
	Game* game = Game::getInstance();

	vector<Player*> players = game->getActivePlayers();

	size_t id = game->getNextPlayerId(game->getBigBlindId()); 
	size_t endId = id;
	bool first = true;

	Command lastCommand;

	while (1) 
	{
		if (id == endId && !first)
		{
			break;
		}

		lastCommand = game->getPlayerById(id)->playTurn(); 

		if (lastCommand ==  RAISE) {
			endId = id;
		}
		if (game->getActivePlayers().size() == 1)
		{
			StateMachine::getInstance()->changeState(Win::getInstance());

			return;
		}


		id = game->getNextPlayerId(id);
	}
}
