#include <stdexcept>
#include "Game.h"
#include "AiPlayer.h"
#include "HumanPlayer.h"
#include "StateMachine.h"

using namespace std;

Game* Game::instance;

Game::Game(size_t numOfPlayers, size_t moneyAmount,size_t smallBlind, size_t bigBlind) : smallBlind(smallBlind), bigBlind(bigBlind), highestRaise(0)
{
	srand (time(NULL));
	deck = new Deck();
	deck->shuffle();
	players.push_back(new HumanPlayer(0, moneyAmount));
	for (size_t i = 1; i < numOfPlayers; i++)
	{
		Player* p = new AiPlayer(i, moneyAmount);
		players.push_back(p);
	}
	dealerId = players[rand() % players.size()]->getId();
	updateDealer();
}

Game* Game::getInstance()
{
	if (!instance) 
	{
		instance = new Game();
	}
	return instance;
}

Game* Game::startGame(size_t numOfPlayers, size_t moneyAmount, size_t smallBlind, size_t bigBlind)
{
	if(instance)
	{
		delete instance;
	}
	instance = new Game(numOfPlayers, moneyAmount, smallBlind, bigBlind);
	return instance;
}

Player* Game::getPlayerById(size_t id) const
{
	for (size_t i = 0; i < players.size(); i++)
	{
		if (players[i]->getId() == id)
		{
			return players[i];
		}
	}
	return NULL;
}

bool Game::removePlayer(size_t id)
{
	for(auto it = players.begin(); it != players.end(); it++)
	{
		if ((*it)->getId() == id)
		{
			if (id == dealerId)
			{
				dealerId = getNextPlayerId(id);
			}
			players.erase(it);
			updateBlindIds();
			return true;
		}
	}
	return false;
}

vector<size_t> Game::getPlayerIds() const
{
	vector<size_t> ids;
	for (size_t i = 0; i < players.size(); i++)
	{
		ids.push_back(players[i]->getId());
	}
	return ids;
}

void Game::updateDealer()
{
	for (size_t i = 0; i < players.size(); i++)
	{
		if (players[i]->getId() == dealerId)
		{
			if (i+1 == players.size())
			{
				dealerId = players[0]->getId();
			}
			else
			{
				dealerId = players[i+1]->getId();
			}
			updateBlindIds();
			return;
		}		
	}
}

void Game::updateBlindIds()
{
	if (players.size() == 2)
	{
		smallBlindId = dealerId;
		bigBlindId = this->getNextPlayerId(dealerId);
	}
	else
	{
		smallBlindId = this->getNextPlayerId(dealerId);
		bigBlindId = this->getNextPlayerId(smallBlindId);
	}
}

size_t Game::getNextPlayerId(size_t id) const
{
	for (size_t i = 0; i < players.size(); i++)
	{
		if (players[i]->getId() == id)
		{
			if (i + 1 == players.size())
			{
				return players[0]->getId();
			}
			else
			{
				return players[i+1]->getId();
			}
		}
	}
}

size_t Game::getCallCost() const {
	if (highestRaise == 0) 
	{
		if (getPot() < bigBlind) {
			return smallBlind;
		}
		if (getPot() == (bigBlind + smallBlind)) {
			return bigBlind;
		}
		return highestRaise;
	}
	return highestRaise;
}

size_t Game::getRaiseCost() const
{
	size_t raise;

	if (highestRaise == 0) 
	{
		raise = bigBlind * 2 + getCallCost();
	}
	else 
	{
		raise = highestRaise *2;
	}

	return raise;
}

void Game::setHighestRaise(size_t raise) {
	if (raise > highestRaise)
	{
		highestRaise = raise;
	}
}

vector<Player*> Game::getActivePlayers()
{
	vector<Player*> active;

	for (size_t i = 0; i < players.size(); i++)
	{
		if (players[i]->isActive())
		{
			active.push_back(players[i]);
		}
	}
	return active;
}

