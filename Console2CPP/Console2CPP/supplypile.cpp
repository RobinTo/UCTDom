#include "supplypile.h"

SupplyPile::SupplyPile(){}

SupplyPile::SupplyPile(Card* cardPtr2, int startingCards2)
{
	cardPtr = cardPtr2;
	startingCards = startingCards2;
	cardsLeft = startingCards2;
}