
#include <list>
#include "card.h"

#ifndef __PLAYER_H__
#define __PLAYER_H__

class player{
public:
	player();
	std::list<card*> deck;
	std::list<card*> hand;
	std::list<card*> discard;
	std::list<card*> inPlay;

	void playTurn();
	void drawCard(int n);
	void endTurn();
	void shuffle();
	void receiveCard(card* c);
};

#endif