#ifndef GAME_H
#define GAME_H
#include <string>

class game
{
public:
	int playerTurn;
	player players[1];
	cardstack cardstacks[6];
	int play_game();



}





#endif