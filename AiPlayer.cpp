#include <stdexcept>
#include "AiPlayer.h"
#include "HelperTools.h"
#include "Game.h"
#include "GameUI.h"
#include <iostream>

using namespace std;

Command AiPlayer::playTurn() {
	if (folded) 
	{
		lastCommand = NONE;
		GameUI::getInstance()->printAction(this);
		return NONE;
	}
	Game* game = Game::getInstance();
	double callCost = game->getCallCost();
	Command move = FOLD;
	int ranValue = rand() % 100;
	Player* opponent;
	double foldCallRatio;
	HandValue hv;

	switch (hv) 
	{
		case BAD:
			if (ranValue < 90 || foldCallRatio < 0.4) 
			{
				if (callCost == 0) 
				{
					move = CALL;
				}	
				else 
				{
					move = FOLD;
				}
			}
			else 
			{
				move = RAISE; //Bluff
			}
			break;
		case AVERAGE:
			if (ranValue < 80 || foldCallRatio < 0.4) 
			{
				if (callCost == 0) 
				{
					move = CALL;
				}	
				else 
				{
					move = FOLD;
				}
			}
			if (ranValue < 87) 
			{
				move = CALL;
			}
			else 
			{
				move = RAISE;
			}
			break;
		case GOOD:
			if (ranValue < 75 || foldCallRatio < 0.4) 
			{
				move = CALL;
			}
			else 
			{
				move = RAISE;
			}
			break;
					
		case EXCELLENT:
			if (ranValue < 30) 
			{
				move = CALL;
			}
			else 
			{
				move = RAISE;
			}
			break;
		default:
			cout <<  "The AI code is broken..." << endl;
	}
	
	switch (move) {
		case RAISE:
			this->raise();
			break;
		case CALL:
			this->call();
			break;
		case FOLD:
			this->fold();
			break;
		default:
			break;
	}
	return lastCommand;
}
/*
// http://fi.pokernews.com/poker-tools/poker-odds-calculator.htm
AiPlayer::HandGoodness AiPlayer::evaluateHand() {
								
	Game* game = Game::getInstance();
	double callCost = game->getCallCost();
	double pot =  game->getPot();
	double bet = this->getBet();
	vector<int> cvv = calculateHandValue(getCards());
	vector<int> hvv = calculateHandValue(getHand());
	vector<double> oddsv = calculateOdds(cvv[0], 2, hvv[0]);
	double vp = oddsv[0]/100; // Victory probalility
	double potOdds = (callCost + bet) / (pot/2 + callCost + bet);

	
	// Call of the cost is zero so instead of Rate of return we use handstrength to make decicions
	// on how to continue.
	if (callCost == 0 || callCost == 50) {
		if (vp < 0.6)	return AVERAGE;
		if (vp < 0.85)	return GOOD;
		return EXCELLENT;	
	}
	double RoR = vp / potOdds; //Rate of Return
	//cout << "Player 1 RoR: " << RoR << " Player 1 potodds: " << potOdds<< "\n";				

	if (this->getMoney() <= 600 ) {
								
		switch (getCards().size()) {
		//These are the values that need to be iterated.
										
			case 2: // Preflop
				if (RoR < 0.85) return BAD;
				if (RoR < 1.1)	return AVERAGE;
				if (RoR < 1.4)	return GOOD;
				return EXCELLENT;
			case 5: // Flop
				if (RoR < 1.05) return BAD;
				if (RoR < 1.3)	return AVERAGE;
				if (RoR < 1.8)	return GOOD;
				return EXCELLENT;
			case 6: // Turn			
				if (RoR < 1.35) return BAD;
				if (RoR < 1.8)	return AVERAGE;
				if (RoR < 2.2)	return GOOD;
				return EXCELLENT;
			case 7: // River
				if (RoR < 1.55) return BAD;
				if (RoR < 3.15 && vp > 0.75)	return GOOD;
				if (RoR < 3.15) return AVERAGE;	
				if (vp > 0.85) return EXCELLENT;
				return GOOD;		
		}
	}
	
	if (this->getMoney() <= 1600 ) {
		switch (getCards().size()) {
		//These are the values that need to be iterated.
							
			case 2: // Preflop
				if (RoR < 0.85) return BAD;
				if (RoR < 1.1)	return AVERAGE;
				if (RoR < 1.4)	return GOOD;
				return EXCELLENT;
			case 5: // Flop
				if (RoR < 1) return BAD;
				if (RoR < 1.2)	return AVERAGE;
				if (RoR < 1.75)	return GOOD;
				return EXCELLENT;
			case 6: // Turn			
				if (RoR < 1.2) return BAD;
				if (RoR < 1.65)	return AVERAGE;
				if (RoR < 2.05)	return GOOD;
				return EXCELLENT;
			case 7: // River
				if (RoR < 1.4) return BAD;
				if (RoR < 3 && vp > 0.75)	return GOOD;
				if (RoR < 3) return AVERAGE;	
				if (vp > 0.75) return EXCELLENT;
				return GOOD;		}
								
	}
	
	else {
		switch (getCards().size()) {
		//These are the values that need to be iterated.
			case 2: // Preflop
				if (RoR < 0.8) return BAD;
				if (RoR < 1.1)	return AVERAGE;
				if (RoR < 1.35)	return GOOD;
				return EXCELLENT;
			case 5: // Flop
				if (RoR < 1.0) return BAD;
				if (RoR < 1.25)	return AVERAGE;
				if (RoR < 1.55)	return GOOD;
				return EXCELLENT;
			case 6: // Turn			
				if (RoR < 1.2) return BAD;
				if (RoR < 1.55)	return AVERAGE;
				if (RoR < 1.95)	return GOOD;
				return EXCELLENT;
			case 7: // River
				if (RoR < 1.4) return BAD;
				if (RoR < 2.85 && vp > 0.75)	return GOOD; // RoR gets easily distorted when we are at river so continue only if win percentage is preferable.
				if (RoR < 2.85) return AVERAGE;	
				if (vp > 0.75) return EXCELLENT;
				return GOOD;
		}
	}						
	throw logic_error( "AI's evaluateHand() method encountered a problem." );
}
*/								
