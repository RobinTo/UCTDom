#include "player.h"
#include "card.h"
#include <iostream>

player::player()
{

}

void player::playTurn()
{
	while(!hand.empty())
	{
		playCard(hand.front());
		hand.pop_front();
	}
	endTurn();

	std::cout << "Ended turn." << std::endl;
}

void player::drawCard(int n)
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
	deck.push_back(c);
}

void player::shuffle()
{
	// For now just push all from discard into deck
	while(!discard.empty())
	{
		deck.push_back(discard.front());
		discard.pop_front();
	}
}

void player::playCard(card* c)
{
	std::cout << c->name << std::endl;
	inPlay.push_back(c);
}