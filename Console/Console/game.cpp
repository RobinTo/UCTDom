#include "game.h"
#include <iostream>
#include "victoryCard.h"
#include "cardstack.h"
#include "treasureCard.h"
#include "player.h"
#include "UCTMonteCarlo.h"

game::game()
{
	cardstacks[0] = cardstack(victoryCard(1), 12);
	cardstacks[1] = cardstack(victoryCard(3), 12);
	cardstacks[2] = cardstack(victoryCard(5), 12);
	cardstacks[3] = cardstack(treasureCard(1), 12);
	cardstacks[4] = cardstack(treasureCard(2), 12);
	cardstacks[5] = cardstack(treasureCard(3), 12);

	cardstacks[0].cardType.name = "Estate";
	cardstacks[1].cardType.name = "Duchy";
	cardstacks[2].cardType.name = "Province";
	cardstacks[3].cardType.name = "Copper";
	cardstacks[4].cardType.name = "Silver";
	cardstacks[5].cardType.name = "Gold";

	cardstacks[0].cardType.cost = 2;
	cardstacks[1].cardType.cost = 5;
	cardstacks[2].cardType.cost = 8;
	cardstacks[3].cardType.cost = 0;
	cardstacks[4].cardType.cost = 3;
	cardstacks[5].cardType.cost = 6;
	
	player p;
	players[0] = p;
	// Hand 1
	players[0].receiveCard(&cardstacks[0].cardType);
	players[0].receiveCard(&cardstacks[0].cardType);
	players[0].receiveCard(&cardstacks[3].cardType);
	players[0].receiveCard(&cardstacks[3].cardType);
	players[0].receiveCard(&cardstacks[3].cardType);

	// Hand 2
	players[0].receiveCard(&cardstacks[0].cardType);
	players[0].receiveCard(&cardstacks[3].cardType);
	players[0].receiveCard(&cardstacks[3].cardType);
	players[0].receiveCard(&cardstacks[3].cardType);
	players[0].receiveCard(&cardstacks[3].cardType);

	// Hand 3
	players[0].receiveCard(&cardstacks[3].cardType);
	players[0].receiveCard(&cardstacks[3].cardType);
	players[0].receiveCard(&cardstacks[3].cardType);
	players[0].receiveCard(&cardstacks[3].cardType);
	players[0].receiveCard(&cardstacks[3].cardType);

	players[0].endTurn(); // Draw the initial 5 cards.


	
	UCTMonteCarlo mc(createTreeNode(true, true, NULL, 5, NULL));

	createTreeNode(false, false, &cardstacks[0].cardType, 0, mc.rootNodePtr);
	createTreeNode(false, false, &cardstacks[1].cardType, 0, mc.rootNodePtr);
//	createTreeNode(false, &cardstacks[2].cardType, 0, &mc.t.initial);
	createTreeNode(false, false, &cardstacks[3].cardType, 0, mc.rootNodePtr);
	createTreeNode(false, false, &cardstacks[4].cardType, 0, mc.rootNodePtr);
//	createTreeNode(false, &cardstacks[5].cardType, 0, &mc.t.initial);
	
	mc.rollout(*this);
	
	std::cout << "Starting game." << std::endl;
	int turn = 0;
	while (turn < 3)
	{
		players[0].playTurn();
		turn++;
	}
}

treeNode* game::createTreeNode(bool state, bool isRoot, card* card, int cash, treeNode* parentPtr)
{
	treeNode* t = new treeNode(isRoot, parentPtr);
	t->state = state;
	t->c = card;
	t->value = 1;
	t->visited = 0;
	t->cash = cash;
	

	if (parentPtr != NULL)
		parentPtr->appendChild(t);
	return t;
}

int game::play_game()
{
	playerTurn = 0;
	return 0;
}

std::list<card*> game::getOptions(int cash)
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