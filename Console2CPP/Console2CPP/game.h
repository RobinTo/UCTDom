#include <vector>

#include "player.h"
#include "gamestate.h"


#ifndef __GAME_H__
#define __GAME_H__

class Game
{
private:
	GameState* gameStatePtr;
	std::vector<Player> players;
	
public:
	void initialize();
	void prepareCards();
	void loopGame();
};


#endif