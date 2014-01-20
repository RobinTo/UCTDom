#include "card.h"

#ifndef CARDSTACK_H
#define CARDSTACK_H


#include "card.h"

class cardstack
{
public:
	cardstack::cardstack(card cardType);
	cardstack::~cardstack();
	card cardType;
	int cardsLeft;
	int maximumCards;
};





#endif