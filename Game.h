#ifndef HOLDEM_GAME
#define HOLDEM_GAME

#include "Deck.h"
#include "Player.h"
#include <iostream>

using namespace std;

class Game
{
public:
	static Game* getInstance();
	
	~Game() {
		delete deck;
		for (size_t i=0; i < players.size(); i++)
		{
			delete players[i];
		}
	}

	static Game* startGame(size_t numOfPlayers, size_t moneyAmount, size_t smallBlind, size_t bigBlind);

	Player* getPlayerById(size_t id) const;

	vector<size_t> getPlayerIds() const;

	size_t getNextPlayerId(size_t id) const;

	vector<Player*> getPlayers() const
	{
		return players;
	}

	vector<Player*> getActivePlayers();

	bool removePlayer(size_t id);

	size_t getDealerId() const
	{
		return dealerId;
	}

	void updateDealer();

	size_t getCallCost() const;

	size_t getRaiseCost() const;

	void setHighestRaise(size_t raise);

	size_t getSmallBlindId() const
	{
		return smallBlindId;
	}

	size_t getBigBlindId() const
	{	
		return bigBlindId;
	}

	size_t getSmallBlind() const
	{
		cout << "Small: " << smallBlind << endl;
		return smallBlind;
	}

	size_t getBigBlind() const
	{
		cout << "Big: " << bigBlind << endl;
		return bigBlind;
	}

	void raiseBlinds()
	{
		smallBlind *= 2;
		bigBlind *= 2;
	}

	size_t getNumOfPlayers() const
	{
		return players.size();
	}

	Deck* getDeck() const
	{
		return deck;
	}

	vector<Card*> getTable() const
	{
		return table;
	}

	void addToTable(Card* card)
	{
		table.push_back(card);
	}

	void clearTable()
	{
		table.clear();
	}

	void setPot(size_t pot)
	{
		Game::pot = pot;
		highestRaise = 0;
	}

	size_t getPot() const
	{
		return pot;
	}

	void resetDeck() 
	{
		delete deck;
		deck = new Deck();
		deck->shuffle();
	}

private:
	Game(size_t numOfPlayers =2, size_t moneyAmount =5000, size_t smallBlind =50, size_t bigBlind =100);

	static Game* instance;

	void updateBlindIds();

	Deck* deck;
	vector<Player*> players;
	vector<Card*> table;

	size_t smallBlind;
	size_t bigBlind;
	size_t highestRaise;

	size_t dealerId;
	size_t smallBlindId;
	size_t starterId;
	size_t bigBlindId;

	size_t pot;

	bool botsOnly;
};

#endif
