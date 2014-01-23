#include <list>
#include "cardstack.h"
#include "card.h"
#include "playerState.h"


#ifndef GAMESTATE_H
#define GAMESTATE_H
class gameState
{
public:
	gameState();
	gameState(const gameState& original);

	cardstack cardstacks[6];
	std::list<playerState*> playerStatePtrs;

	std::list<card*> getOptions(int cash);

};





#endif

	