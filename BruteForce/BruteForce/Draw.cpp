#include "Draw.h"

Draw::Draw()
{

}

Draw::Draw(std::vector<Card*> deck)
{
	currentDeck = deck;
}

Draw::Draw(std::vector<Card*> deck, Card* removeCard)
{
	for (std::vector<Card*>::iterator iterator = deck.begin(); iterator != deck.end(); ++iterator)
	{
		if ((*iterator)->name == removeCard->name)
		{
			deck.erase(iterator);
			break;
		}
	}
	
	currentDeck = deck;
	
}

Draw::Draw(std::vector<Card*> deck, std::vector<Card*> draw)
{
	currentDeck = deck;
	currentDraw = draw;
}

Draw::Draw(std::vector<Card*> deck, std::vector<Card*> draw, Card* removeCard)
{
	for (std::vector<Card*>::iterator iterator = deck.begin(); iterator != deck.end(); ++iterator)
	{
		if ((*iterator)->name == removeCard->name)
		{
			deck.erase(iterator);
			break;
		}
	}

	currentDeck = deck;
	currentDraw = draw;
}