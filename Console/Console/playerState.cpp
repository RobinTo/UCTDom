#include "playerState.h"
#include <iostream>

void playerState::drawCard(int n)
{
	for (int i = 0; i<n; i++)
	{
		if(deck.empty())
			shuffle();
		if(!deck.empty())
		{
			hand.push_back(deck.front());
			deck.pop_front();
		}
	}
}

void playerState::endTurn()
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

void playerState::receiveCard(card* cardPtr)
{
	deck.push_back(cardPtr);
}

void playerState::shuffle()
{
	// For now just push all from discard into deck
	while(!discard.empty())
	{
		deck.push_back(discard.front());
		discard.pop_front();
	}
}

void playerState::playCard(card* cardPtr)
{
	std::cout << cardPtr->name << std::endl;
	inPlay.push_back(cardPtr);
}