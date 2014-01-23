
#include <string>
#include "player.h"
#include "cardstack.h"
#include "card.h"
#include "treeNode.h"
#include "gameState.h"

#ifndef GAME_H
#define GAME_H
class game
{
public:
	
	gameState gameState;

	game();
	int playerTurn;
	player players[1];
	int play_game();

};





#endif