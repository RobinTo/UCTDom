#include "game.h"
#include <iostream>
#include "victoryCard.h"
#include "cardstack.h"
#include "treasureCard.h"
#include "player.h"

game::game()
{
	cardstacks[0] = cardstack(victoryCard(1), 12);
	cardstacks[1] = cardstack(victoryCard(3), 12);
	cardstacks[2] = cardstack(victoryCard(5), 12);
	cardstacks[3] = cardstack(treasureCard(1), 12);
	cardstacks[4] = cardstack(treasureCard(2), 12);
	cardstacks[5] = cardstack(treasureCard(3), 12);

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
}

int game::play_game()
{
	playerTurn = 0;
	return 0;
}
