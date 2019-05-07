#ifndef HOLDEM_HPLAYER
#define HOLDEM_HPLAYER

#include "Player.h"

using namespace std;

class HumanPlayer : public Player
{
public:
	HumanPlayer(size_t id, size_t money) : Player(id, money) {}

	Command playTurn();

	string getName()
	{
		return "Human Player";
	}
};

#endif
