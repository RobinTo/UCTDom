
#include <string>
#include "player.h"
#include "cardstack.h"

#ifndef GAME_H
#define GAME_H
class game
{
public:
	int playerTurn;
	player players[1];
	cardstack cardstacks[6];
	int play_game();



};





#endif