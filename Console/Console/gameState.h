#include <list>
#include "cardstack.h"
#include "card.h"
#include "playerState.h"


#ifndef GAMESTATE_H
#define GAMESTATE_H
class gameState
{
public:

	cardstack cardstacks[6];
	std::list<playerState*> playerStatePtrs;

	std::list<card*> getOptions(int cash);

};





#endif

	