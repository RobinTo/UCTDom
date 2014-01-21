#include "player.h"
#include "card.h"


void player::playTurn()
{

}

void player::drawCard(int n)
{
	for (int i = 0; i<n; i++)
	{
		if(deck.empty())
			shuffle();
		hand.push_back(deck.front());
		deck.pop_front();
	}
}

void player::endTurn()
{
	while(!inPlay.empty())
	{
		discard.push_back(inPlay.front());
		inPlay.pop_front();
	}
	while(!hand.empty())
	{
		discard.push_back(hand.front());
		hand.pop_front();
	}
	drawCard(5);
}

void player::receiveCard(card* c)
{
	discard.push_back(c);
}

void player::shuffle()
{
	// Shuffle!
}