#include "card.h"

#ifndef CARDSTACK_H
#define CARDSTACK_H


#include "card.h"

class cardstack
{
public:
	cardstack(card cardType, int numberOfCards);
	~cardstack();
	card cardType;
	int cardsLeft;
	int maximumCards;
};





#endif