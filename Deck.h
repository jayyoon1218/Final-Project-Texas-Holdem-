#ifndef HOLDEM_DECK
#define HOLDEM_DECK

#include <stdlib.h>
#include <time.h>
#include <vector>
#include "Card.h"

using namespace std;

class Deck {
public:
	Deck() 
	{
		for (size_t color = 0; color < 4; color++)
		{
			for (size_t value = 2; value < 15; value++)
			{
				Card* card = new Card(color, value);
				deck.push_back(card);
				cards.push_back(card);
			}
		}
	}

	~Deck()
	{
		for (size_t i = 0; i < cards.size(); i++)
		{
			delete cards[i];
		}
	}

	Card* drawCard()
	{
		Card* returnCard = deck[deck.size()-1];
		deck.pop_back();
		return returnCard;
	}

	void shuffle()
	{
		for (size_t i = 0; i < deck.size();  i++)
		{
			size_t j = rand() % deck.size();
			swap(i, j);
		}
	}

private:
	void swap(size_t i, size_t j)
	{
		Card* tmp = deck[i];
		deck[i] = deck[j];
		deck[j] = tmp;
	}

	vector<Card*> deck;
 
	vector<Card*> cards;
};

#endif
