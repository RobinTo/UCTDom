#include <vector>
#include "card.h"
#include <stdlib.h>
#include <time.h>

#ifndef __PLAYER_H__
#define __PLAYER_H__

class player
{
public:
	player();



	std::vector<card*> hand;
	std::vector<card*> discard;
	std::vector<card*> deck;
	std::vector<card*> inPlay;

	void shuffle();
	void endTurn();
	void drawCard();
	void buyCard(card* c);
	int moneyInHand();
	int victoryPoints();
};


#endif