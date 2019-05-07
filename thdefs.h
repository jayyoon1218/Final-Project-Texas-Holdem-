#ifndef THDEFS_H
#define THDEFS_H
#include <vector>

using namespace std;

enum Command { RAISE, CALL, CHECK, FOLD, NONE, PLAY, QUIT, HELP, ODDS, NUMS, EMPTY, BOTS, NOBOTS};
enum GameState { PREFLOP, FLOP, TURN, RIVER, WIN, END };
enum Suit { SPADES = 0, HEARTS = 1, DIAMONDS = 2, CLUBS = 3};

typedef vector<vector<int>> odds;

#endif
