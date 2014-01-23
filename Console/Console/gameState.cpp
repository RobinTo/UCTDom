#include "gameState.h"

gameState::gameState(){}

gameState::gameState(const gameState& original)
{
	// Deep copies game and playerstate. Only for the first playerstate! Need more work if more than one player, but should be easy.
	gameState.cardstacks[0] = original.cardstacks[0];
	gameState.cardstacks[1] = original.cardstacks[1];
	gameState.cardstacks[2] = original.cardstacks[2];
	gameState.cardstacks[3] = original.cardstacks[3];
	gameState.cardstacks[4] = original.cardstacks[4];
	gameState.cardstacks[5] = original.cardstacks[5];
	playerState playerStateCopy;
	playerStateCopy.deck = original.playerStatePtrs.front()->deck;
	playerStateCopy.discard = original.playerStatePtrs.front()->discard;
	playerStateCopy.inPlay = original.playerStatePtrs.front()->inPlay;
	playerStateCopy.hand = original.playerStatePtrs.front()->hand;
	gameState.playerStatePtrs.push_back(&playerStateCopy);
}

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