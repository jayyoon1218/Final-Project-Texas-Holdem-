#ifndef HOLDEM_AIPLAYER
#define HOLDEM_AIPLAYER

#include "Player.h"
using namespace std;

class AiPlayer : public Player {
public:

	AiPlayer(size_t id, size_t money) : Player(id, money) {}

	Command playTurn();

private:
	enum HandValue {BAD, AVERAGE, GOOD, EXCELLENT};

	HandValue evaluateHand();
};
#endif
