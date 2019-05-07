#ifndef HOLDEM_PLAYER
#define HOLDEM_PLAYER

#include <vector>
#include "Card.h"
#include "thdefs.h"
#include <iostream>

using namespace std;

class Player
{
public:

	Player(size_t id, size_t money) : id(id), money(money), bet(0), folded(false), lastCommand(NONE), raises(0), turns(0), calls(0), folds(0) {}

	virtual Command playTurn() = 0;

	void setHand(Card* card1, Card* card2)
	{
		hand.clear();
		hand.push_back(card1);
		hand.push_back(card2);
	}

	size_t getId() const
	{
		return id;
	}
	
	bool isActive() const
	{
		return !folded;
	}

	size_t getBet() const
	{
		return bet;
	}
	
	void setBet(size_t bet);

	size_t getMoney()
	{
		return money;
	}

	double getRaises() {
		return raises;
	}
	double getFolds() {
		return folds;
	}

	vector<Card*> getHand() const
	{
		return hand;
	}

	vector<Card*> getCards() const;

	Command getLastCommand() const
	{
		return lastCommand;
	}
	
	void resetLastCommand()
	{
		lastCommand = NONE;
	}

	void resetFold()
	{
		folded = false;
	}

	void giveMoney(size_t amount)
	{
		money += amount;
	}

	bool isAllIn()
	{
		return money == 0;
	}

	virtual string getName();

	void raise();

	void call();

	void fold();

	const size_t id;
	vector<Card*> hand;
	size_t money;
	size_t bet;
	bool folded;
	bool callWasCheck;
	Command lastCommand;
	double raises;
	double turns;
	double calls;
	double folds;
};

#endif
