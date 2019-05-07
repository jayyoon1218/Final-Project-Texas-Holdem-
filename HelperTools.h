#ifndef HOLDEM_HELPERTOOLS
#define HOLDEM_HELPERTOOLS

#include <utility>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Card.h"
#include "Deck.h"
#include "thdefs.h"

void initOdds(int players, int times);

void writeOddsToFile(int playerNum);

odds readOddsFromFile(int rowVal, size_t playerNum);

vector<int> calculateHandValue(vector<Card*>);

vector<double> calculateOdds(int rowVal, size_t players, int handRowVal);

int eqToInt(int eq);

bool compareCards(Card* Card1, Card* Card2);

string cardsToString(vector<Card*> cards);

#endif
