#include <sstream>
#include "Card.h"
#include "thdefs.h"

using namespace std;

string Card::toString() const
{
	stringstream ss (stringstream::in | stringstream::out);


	switch (color)
	{
	case SPADES:
		ss << "\033[0;34m";
		ss << "♠";
		break;
	case HEARTS:
		ss << "\033[0;31m";
		ss << "♥";
		break;
	case DIAMONDS:
		ss << "\033[0;31m";
		ss << "♦";
		break;
	case CLUBS:
		ss << "\033[0;34m";
		ss << "♣";
		break;
	}
	ss << " ";
	switch (value)
	{
	case 10:
		ss << "T";
		break;
	case 11:
		ss << "J";
		break;
	case 12:
		ss << "Q";
		break;
	case 13:
		ss << "K";
		break;
	case 14:
		ss << "A";
		break;
	default:
		ss << value;
		break;
	}

	ss << "\033[0m";

	string temp;
	temp = ss.str();
	return temp;
}

