#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <list>
#include "card.h"

class player{
public:
	std::list<card*> deck;
	std::list<card*> hand;
	std::list<card*> discard;
	std::list<card*> inPlay;

	void playTurn();

};

#endif