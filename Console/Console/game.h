
#include <string>
#include "player.h"
#include "cardstack.h"
#include "card.h"
#include "treeNode.h"


#ifndef GAME_H
#define GAME_H
class game
{
public:
	game();
	int playerTurn;
	player players[1];
	cardstack cardstacks[6];
	int play_game();

	std::list<card*> getOptions(int cash);
	void createTreeNode(bool state, card* card, int cash, treeNode* p);

};





#endif