#include <vector>

#include "card.h"



#ifndef __PLAYERSTATE_H__
#define __PLAYERSTATE_H__

class PlayerState
{
public:
	std::vector<Card*> deck;
	std::vector<Card*> discard;
	std::vector<Card*> hand;
	std::vector<Card*> inPlay;
	int actionsLeft;
	int moneyLeft;
	int buysLeft;

	PlayerState();
};


#endif