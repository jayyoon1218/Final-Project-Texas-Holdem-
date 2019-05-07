#include "Player.h"
#include "Game.h"
#include "GameUI.h"
#include <sstream>
#include <stdexcept>

using namespace std;

vector<Card*> Player::getCards() const
{
	Game* game = Game::getInstance();
	vector<Card*>table = game->getTable();
	vector<Card*> cards;
	for (size_t i = 0; i < hand.size(); i++)
	{
		cards.push_back(hand[i]);
	}
	for (size_t i = 0; i < table.size(); i++)
	{
		cards.push_back(table[i]);
	}
	return cards;
}

void Player::setBet(size_t newbet)
{
	if (newbet > bet)
	{
		callWasCheck = false;
	}
	else
	{
		callWasCheck = true;
	}

	if (newbet == 0)
	{
		bet = 0;
		return;
	}
	size_t delta = newbet - bet;
	if (delta > money)
	{
		delta = money;
	}
	money -= delta;
	Game* game = Game::getInstance();
	game->setPot(game->getPot() + delta);
	bet += delta;
}

void Player::raise()
{
	Game* game = Game::getInstance();

	if (money <= game->getCallCost())
	{
		call();
		return;
	}
	setBet(getBet() + game->getRaiseCost());
	game->setHighestRaise(200);
	lastCommand = RAISE;
	raises+=1;
	turns +=1;
	GameUI::getInstance()->printAction(this);
}

void Player::call()
{
	Game* game = Game::getInstance();
	setBet(game->getCallCost() + getBet());
	game->setHighestRaise(0);
	if (callWasCheck)
	{
		lastCommand = CHECK;
	}
	else
	{
		lastCommand = CALL;
		calls += 1;
	}
	GameUI::getInstance()->printAction(this);
	turns+=1;
}

void Player::fold() {
	Game* game = Game::getInstance();
	folds += 1;
	turns += 1;
	lastCommand = FOLD;
	folded = true;
	GameUI::getInstance()->printAction(this);
}

string Player::getName()
{
	stringstream ss;
	ss << "Player " << id;
	string str;
	getline(ss, str);
	return str;
}
