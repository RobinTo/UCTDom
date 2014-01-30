#pragma once


#include "Card.h"







class SupplyPile
{
public:
	Card* cardPtr;
	int cardsLeft;
	int startingCards;

	SupplyPile();
	SupplyPile(Card* cardPtr2, int startingCards2);
};
