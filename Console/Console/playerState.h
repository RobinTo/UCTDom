#include <list>
#include "card.h"

#ifndef __PLAYERSTATE_H__
#define __PLAYERSTATE_H__

class playerState{
public:
	std::list<card*> deck;
	std::list<card*> hand;
	std::list<card*> discard;
	std::list<card*> inPlay;

	void drawCard(int n);
	void endTurn();
	void shuffle();
	void receiveCard(card* cardPtr);
	void playCard(card* cardPtr);
};

#endif
	