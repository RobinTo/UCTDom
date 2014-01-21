#include "cardstack.h"
#include "card.h"

cardstack::cardstack(card cardType, int numberOfCards)
{
	cardType = cardType;
	cardsLeft = numberOfCards;
	maximumCards = numberOfCards;
}

cardstack::~cardstack()
{

}
