#include "HelperTools.h"

using namespace std;

string lines[76154];

void initOdds(int players, int times)
{
	// iniating the data structures
	srand(time(NULL));
	pOdds[0] = oddsP2;
	winOddsVector[0] = winOddsP2;
	pOdds[1] = oddsP3;
	winOddsVector[1] = winOddsP3;
	pOdds[2] = oddsP4;
	winOddsVector[2] = winOddsP4;
	pOdds[3] = oddsP5;
	winOddsVector[3] = winOddsP5;
	pOdds[4] = oddsP6;
	winOddsVector[4] = winOddsP6;
	
	int counter = 0;
	for (int i = 0; i < times; i++)
	{
		counter++;
		if (counter % 10000 == 0)
			std::cout << counter << std::endl;
		int winner;
		std::vector<Card*> p1Cards;
		std::vector<Card*> p2Cards;
		std::vector<Card*> p3Cards;
		std::vector<Card*> p4Cards;
		std::vector<Card*> p5Cards;
		std::vector<Card*> p6Cards;

		std::vector< std::vector<Card*> > pCards;
		pCards.push_back(p1Cards);
		pCards.push_back(p2Cards);
		pCards.push_back(p3Cards);
		pCards.push_back(p4Cards);
		pCards.push_back(p5Cards);
		pCards.push_back(p6Cards);

		int pValues[players][5];
		Deck newDeck = Deck();
		newDeck.shuffle();
		//preflop
		for (int i = 0; i < players; i++)
		{
			pCards[i].push_back(newDeck.drawCard());
			pCards[i].push_back(newDeck.drawCard());
			std::vector<int> vals = calculateHandValue(pCards[i]);
			pValues[i][0] = vals[0];
		}
		//flop
		Card* flopCard1 = newDeck.drawCard();
		Card* flopCard2 = newDeck.drawCard();
		Card* flopCard3 = newDeck.drawCard();
		for (int i = 0; i < players; i++)
		{
			pCards[i].push_back(flopCard1);
			pCards[i].push_back(flopCard2);
			pCards[i].push_back(flopCard3);
			std::vector<int> vals = calculateHandValue(pCards[i]);
			pValues[i][1] = vals[0];

		}
		//turn
		Card* turnCard = newDeck.drawCard();
		for (int i = 0; i < players; i++)
		{
			pCards[i].push_back(turnCard);
			std::vector<int> vals = calculateHandValue(pCards[i]);
			pValues[i][2] = vals[0];
		}
		//river
		Card* riverCard = newDeck.drawCard();
		int winnerEqVal;
		int winnerRow;
		for (int i = 0; i < players; i++)
		{
			pCards[i].push_back(riverCard);
			std::vector<int> vals = calculateHandValue(pCards[i]);
			pValues[i][3] = vals[0];

			int newEqVal = eqToInt(vals[1]);
			if (i == 0)
			{
				winnerEqVal = newEqVal;
				winnerRow = vals[0];
				winner = i;
			} 
			else if (newEqVal > winnerEqVal)
			{
				winnerEqVal = newEqVal;
				winnerRow = vals[0];
				winner = i;
			}
			else if (newEqVal == winnerEqVal)
			{
 				if (winnerRow < vals[0])
					winner = i;
			} 
			pValues[i][4] = newEqVal;
		}
		for (int i = 0; i < players; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				int rowVal = pValues[i][j];
				int eqVal = pValues[i][4];
				int preFlopRow = pValues[i][0] - 13;
				pOdds[players - 2][rowVal][eqVal]++;
				pOdds[players - 2][rowVal][0]++;
				//total count of hand & table cards
				winOddsVector[players - 2][rowVal][preFlopRow]++;
				
				if (i == winner)
				{
					//win count from hand & table
					winOddsVector[players - 2][rowVal][preFlopRow+91]++;
				}
			}
		}
	}
}

void writeOddsToFile(int playerNum)
{
	std::stringstream oddsFileName;
	oddsFileName << "odds" << playerNum << ".thd";
	std::string normOdds;
	oddsFileName >> normOdds;
	std::stringstream winFileName;
	winFileName << "winOdds" << playerNum << ".thd";
	std::string winOdds;
	winFileName >> winOdds;

	std::ofstream oddsFile;
	std::ofstream winOddsFile;
	oddsFile.open(normOdds);
	winOddsFile.open(winOdds);

	if (oddsFile.is_open() && winOddsFile.is_open())
	{
		for (int i = 0; i < 76154; i++)
		{
			std::stringstream ss;
			std::stringstream ss2;
			ss << i << "\t";
			ss2 << i << "\t";
			// writes the total hits of odds (eq-classes) 
			for (int j = 0; j < 10; j++)
			{
					ss << pOdds[playerNum-2][i][j];
					if (j < 9)
						ss << "\t";
					else
						ss << "\n";
			}
			// writes the hits of win-odds
			for (int k = 0; k < 182; k++)
			{
				ss2 << winOddsVector[playerNum-2][i][k];
				if (k < 181)
					ss2 << "\t";
				else
					ss2 << "\n";
			}
			oddsFile << ss.str();
			winOddsFile << ss2.str();
		}
		oddsFile.close();
		winOddsFile.close();
	}
	else
	{
		std::cout << "Could not open/create the necessary files, terminating program" << endl;
		oddsFile.close();
		winOddsFile.close();
		exit(1);
	}
}


odds readOddsFromFile(int rowVal, size_t playerNum)
{
	vector<int> currentOdds(10);
	vector<int> currentWinOdds(182);

	ifstream oddsFile;
	stringstream oddsName;
	oddsName << "odds" << playerNum << ".thd";
	oddsFile.open(oddsName.str());
	ifstream winOddsFile;
	stringstream winOddsName;
	winOddsName << "winOdds" << playerNum << ".thd";
	winOddsFile.open(winOddsName.str());
	if (oddsFile.is_open() && winOddsFile.is_open())
	{
		string rowLine;
		string rowLine2;
		for (int i = 0; i < rowVal+1; i++)
		{
			getline(oddsFile, rowLine);
			getline(winOddsFile, rowLine2);

			if (i == rowVal)
			{
				//reading and saving normal odds from file
				vector<string> rowValues;
				stringstream data(rowLine);
				string line;
				getline(data,line,'\t');
				while (getline(data,line,'\t'))
						rowValues.push_back(line);
				for (int j = 0; j < 10 ; j++)
				{
					stringstream temp;
					temp << rowValues[j];
					int tempInt;
					temp >> tempInt;
					currentOdds[j] = tempInt;

 				}
				//reading and saving win odds from file
				vector<string> rowValues2;
				stringstream data2(rowLine2);
				string line2;
				getline(data2,line2,'\t');
				while (getline(data2,line2,'\t'))
					rowValues2.push_back(line2);
				for (int k = 0; k < 182 ; k++)
				{
					stringstream temp;
					temp << rowValues2[k];
					int tempInt;
					temp >> tempInt;
					currentWinOdds[k] = tempInt;
				}

			}

		}
		oddsFile.close();
		winOddsFile.close();

		odds returnOdds;
		returnOdds.push_back(currentOdds);
		returnOdds.push_back(currentWinOdds);

		return returnOdds;
	}
	else
	{
		cout << "Could not load the odds-files, terminating system" << endl;
		oddsFile.close();
		winOddsFile.close();
		exit(1);
	}
}

vector<int> calculateHandValue(vector<Card*> cards)
{
	//sort the cards for the DAG
	sort(cards.begin(), cards.end(), compareCards);

	vector<size_t> cardValues(cards.size());

	vector<Card*> spad;
	vector<Card*> heart;
	vector<Card*> diam;
	vector<Card*> club;
	int spads = 0;
	int hearts = 0;
	int diams = 0;
	int clubs = 0;

	vector<Card*> flushCards = spad;
	//check for flushes
	for (size_t i = 0; i < cards.size(); i++)
	{
		cardValues[i] = cards[i]->value-2;
		if (cards[i]->color == SPADES)
		{
			spads++;
			spad.push_back(cards[i]);
			if (spads > 4)
				flushCards = spad;
		}
		if (cards[i]->color == HEARTS)
		{
			hearts++;
			heart.push_back(cards[i]);
			if (hearts > 4)
				flushCards = heart;
		}
		if (cards[i]->color == DIAMONDS)
		{
			diams++;
			diam.push_back(cards[i]);
			if (diams > 4)
				flushCards = diam;

		}
		if (cards[i]->color == CLUBS)
		{
			clubs++;
			club.push_back(cards[i]);
			if (clubs > 4)
				flushCards = club;
		}

	}
	int rowValue = 0;
	int currentBestValue = 0;
	int nextRow = cardValues[0];

	for (size_t i = 0; i < cards.size(); i++)
	{
		vector<string> rowValues;
		string rowLine = lines[nextRow];
		stringstream data(rowLine);
		string line;
		while (getline(data,line,'\t'))
			rowValues.push_back(line);
		//get next row if needed
		if (i < cards.size()-1)
		{
			istringstream converter(rowValues[cardValues[i+1]+1]);
			converter >> nextRow;
		}
		else
		{
			//valueting 7-cards
			if (cards.size() == 7)
			{
				int tempVal;
				istringstream converter(rowValues[15]);
				converter >> tempVal;
				currentBestValue = tempVal;
			}
			//valueting less than 7-cards
			else
			{
				int stepper = 17;
				istringstream converter(rowValues[stepper]);
				converter >> currentBestValue;
			}
		}
	rowValue = nextRow;
	}
	// the same algorithm for getting correct value for flush-cases
	if (spads >= 5 || hearts >= 5 || diams >= 5 || clubs >= 5)
	{
		sort(flushCards.begin(), flushCards.end(), compareCards);
		vector<size_t> flushCardValues(flushCards.size());

		for (size_t i = 0; i < flushCards.size(); i++)
			flushCardValues[i] = flushCards[i]->value-2;

		int nextRow = flushCardValues[0];

		for (size_t i = 0; i < flushCards.size(); i++)
		{
			vector<string> rowValues;
			string rowLine = lines[nextRow];
			stringstream data(rowLine);
			string line;
			while (getline(data,line,'\t'))
				rowValues.push_back(line);
			if (i < flushCards.size()-1)
			{
				istringstream converter(rowValues[flushCardValues[i+1]+1]);
				converter >> nextRow;
			}
			else
			{
				int tempVal;
				istringstream converter(rowValues[14]);
				converter >> tempVal;
				currentBestValue = tempVal;
			}
		}
	}

	vector<int> returnTable(2);
	returnTable[0] = rowValue;
	returnTable[1] = currentBestValue;
	return returnTable;

}

vector<double> calculateOdds(int rowVal, size_t players, int handRowVal){
	vector<double> returnDoubles;
	odds oddsFromFile = readOddsFromFile(rowVal, players);

	double win = (double(oddsFromFile[1][(handRowVal-13)+91]) / double(oddsFromFile[1][handRowVal-13]))*100;
	double sf = (double(oddsFromFile[0][9]) / double(oddsFromFile[0][0]))*100;
	double four = (double(oddsFromFile[0][8]) / double(oddsFromFile[0][0]))*100;
	double fh = (double(oddsFromFile[0][7]) / double(oddsFromFile[0][0]))*100;
	double f = (double(oddsFromFile[0][6]) / double(oddsFromFile[0][0]))*100;
	double s = (double(oddsFromFile[0][5]) / double(oddsFromFile[0][0]))*100;
	double three = (double(oddsFromFile[0][4]) / double(oddsFromFile[0][0]))*100;
	double twop = (double(oddsFromFile[0][3]) / double(oddsFromFile[0][0]))*100;
	double onep = (double(oddsFromFile[0][2]) / double(oddsFromFile[0][0]))*100;
	double hc = (double(oddsFromFile[0][1]) / double(oddsFromFile[0][0]))*100;

	returnDoubles.push_back(win);
	returnDoubles.push_back(sf);
	returnDoubles.push_back(four);
	returnDoubles.push_back(fh);
	returnDoubles.push_back(f);
	returnDoubles.push_back(s);
	returnDoubles.push_back(three);
	returnDoubles.push_back(twop);
	returnDoubles.push_back(onep);
	returnDoubles.push_back(hc);

	return returnDoubles;
}

int eqToInt(int eq)
{
	int retEq;
	//straight-flush
	if (eq >= 1 && eq <= 10)
		retEq = 9;
	//four of a kind
	if (eq >= 11 && eq <= 166)
		retEq = 8;
	//full house
	if (eq >= 167 && eq <= 322)
		retEq = 7;
	//flush
	if (eq >= 323 && eq <= 1599)
		retEq = 6;
	//straight
	if (eq >= 1600 && eq <= 1609)
		retEq = 5;
	//three of a kind
	if (eq >= 1610 && eq <= 2467)
		retEq = 4;
	//two pairs
	if (eq >= 2468 && eq <= 3325)
		retEq = 3;
	//one pair
	if (eq >= 3326 && eq <= 6185)
		retEq = 2;
	//high card
	if (eq >= 6186 && eq <= 7462)
		retEq = 1;

	return retEq;
}

bool compareCards(Card* Card1, Card* Card2)
{
	return Card2->value < Card1->value;
}

string cardsToString(vector<Card*> cards)
{
	stringstream ss;

	for (size_t i = 0; i < cards.size(); i++)
	{
		ss << cards[i]->toString();

		if (i+1 != cards.size())
			ss << " ";
	}

	string temp;

	getline(ss, temp);

	return temp;
}
