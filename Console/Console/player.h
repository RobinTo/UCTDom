#include <list>
#include "card.h"
#include "gameState.h"
#include "UCTMonteCarlo.h"
#include "playerState.h"

#ifndef __PLAYER_H__
#define __PLAYER_H__

class player{
public:
	player();
	UCTMonteCarlo uctmc;
	playerState* playerStatePtr;

	void initialize(gameState* gameStatePtr);
	void playTurn(gameState* gameStatePtr);
};

#endif