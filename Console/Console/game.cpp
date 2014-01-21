#include "game.h"
#include <iostream>
#include "victoryCard.h"
#include "cardstack.h"
#include "treasureCard.h"
#include "player.h"
#include "tree.h"
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
	// Hand 1
	p.receiveCard(&cardstacks[0].cardType);
	p.receiveCard(&cardstacks[0].cardType);
	p.receiveCard(&cardstacks[3].cardType);
	p.receiveCard(&cardstacks[3].cardType);
	p.receiveCard(&cardstacks[3].cardType);

	// Hand 2
	p.receiveCard(&cardstacks[0].cardType);
	p.receiveCard(&cardstacks[3].cardType);
	p.receiveCard(&cardstacks[3].cardType);
	p.receiveCard(&cardstacks[3].cardType);
	p.receiveCard(&cardstacks[3].cardType);

	// Hand 3
	p.receiveCard(&cardstacks[3].cardType);
	p.receiveCard(&cardstacks[3].cardType);
	p.receiveCard(&cardstacks[3].cardType);
	p.receiveCard(&cardstacks[3].cardType);
	p.receiveCard(&cardstacks[3].cardType);

	p.endTurn(); // Draw the initial 5 cards.

	UCTMonteCarlo mc;

	createTreeNode(false, &cardstacks[0].cardType, 0, &mc.t.initial);
	createTreeNode(false, &cardstacks[1].cardType, 0, &mc.t.initial);
	createTreeNode(false, &cardstacks[2].cardType, 0, &mc.t.initial);
	createTreeNode(false, &cardstacks[3].cardType, 0, &mc.t.initial);
	createTreeNode(false, &cardstacks[4].cardType, 0, &mc.t.initial);
	createTreeNode(false, &cardstacks[5].cardType, 0, &mc.t.initial);

	treeNode bestLeaf = mc.selectBestLeaf(mc.t);
	std::cout << bestLeaf.c->name << std::endl;
	std::cout << "Starting game." << std::endl;
	int turn = 0;
	while (turn < 3)
	{
		p.playTurn();
		turn++;
	}
}

void game::createTreeNode(bool state, card* card, int cash, treeNode* parent)
{
	treeNode t;
	t.state = false;
	t.c = card;
	t.value = 1;
	t.visited = 0;
	t.cash = 0;
	t.parentNode = parent;

	parent->appendChild(t);
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
		if (cash > cardstacks[i].cardType.cost)
		{
			cards.push_back(&cardstacks[i].cardType);
		}
	}

	return cards;
}