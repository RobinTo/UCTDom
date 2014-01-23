#include "gameState.h"


std::list<card*> gameState::getOptions(int cash)
{
	std::list<card*> cards;
	for(int i = 0; i < 6; i++)
	{
		if (cash >= cardstacks[i].cardType.cost)
		{
			cards.push_back(&cardstacks[i].cardType);
		}
	}

	return cards;
}