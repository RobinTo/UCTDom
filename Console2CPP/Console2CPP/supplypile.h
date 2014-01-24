

#include "card.h"



#ifndef __SUPPLYPILE_H__
#define __SUPPLYPILE_H__

class SupplyPile
{
public:
	Card* cardPtr;
	int cardsLeft;
	int startingCards;

	SupplyPile();
	SupplyPile(Card* cardPtr2, int startingCards2);
};



#endif