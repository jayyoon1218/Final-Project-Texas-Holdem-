#ifndef HOLDEM_CARD
#define HOLDEM_CARD

#include <string>

using namespace std;

class Card
{
public:
	Card(size_t const color, size_t const value) : color(color), value(value) {}

	string toString() const;

	size_t const color;
	size_t const value;
};
#endif
