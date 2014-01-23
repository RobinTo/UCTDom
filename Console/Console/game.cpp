#include "game.h"
#include <iostream>
#include "victoryCard.h"
#include "cardstack.h"
#include "treasureCard.h"
#include "player.h"
#include <ctime>

game::game()
{
	srand(time(0));
	gameState.cardstacks[0] = cardstack(victoryCard(1), 12);
	gameState.cardstacks[1] = cardstack(victoryCard(3), 12);
	gameState.cardstacks[2] = cardstack(victoryCard(5), 12);
	gameState.cardstacks[3] = cardstack(treasureCard(1), 12);
	gameState.cardstacks[4] = cardstack(treasureCard(2), 12);
	gameState.cardstacks[5] = cardstack(treasureCard(3), 12);

	gameState.cardstacks[0].cardType.name = "Estate";
	gameState.cardstacks[1].cardType.name = "Duchy";
	gameState.cardstacks[2].cardType.name = "Province";
	gameState.cardstacks[3].cardType.name = "Copper";
	gameState.cardstacks[4].cardType.name = "Silver";
	gameState.cardstacks[5].cardType.name = "Gold";

	gameState.cardstacks[0].cardType.cost = 2;
	gameState.cardstacks[1].cardType.cost = 5;
	gameState.cardstacks[2].cardType.cost = 8;
	gameState.cardstacks[3].cardType.cost = 0;
	gameState.cardstacks[4].cardType.cost = 3;
	gameState.cardstacks[5].cardType.cost = 6;
	
	player p;
	p.initialize(&gameState);
	players[0] = p;
	// Hand 1
	players[0].playerStatePtr->receiveCard(&gameState.cardstacks[0].cardType);
	players[0].playerStatePtr->receiveCard(&gameState.cardstacks[0].cardType);
	players[0].playerStatePtr->receiveCard(&gameState.cardstacks[3].cardType);
	players[0].playerStatePtr->receiveCard(&gameState.cardstacks[3].cardType);
	players[0].playerStatePtr->receiveCard(&gameState.cardstacks[3].cardType);

	// Hand 2
	players[0].playerStatePtr->receiveCard(&gameState.cardstacks[0].cardType);
	players[0].playerStatePtr->receiveCard(&gameState.cardstacks[3].cardType);
	players[0].playerStatePtr->receiveCard(&gameState.cardstacks[3].cardType);
	players[0].playerStatePtr->receiveCard(&gameState.cardstacks[3].cardType);
	players[0].playerStatePtr->receiveCard(&gameState.cardstacks[3].cardType);

	// Hand 3
	players[0].playerStatePtr->receiveCard(&gameState.cardstacks[3].cardType);
	players[0].playerStatePtr->receiveCard(&gameState.cardstacks[3].cardType);
	players[0].playerStatePtr->receiveCard(&gameState.cardstacks[3].cardType);
	players[0].playerStatePtr->receiveCard(&gameState.cardstacks[3].cardType);
	players[0].playerStatePtr->receiveCard(&gameState.cardstacks[3].cardType);

	players[0].playerStatePtr->endTurn(); // Draw the initial 5 cards.
	
	std::cout << "Starting game." << std::endl;
	int turn = 0;
	while (turn < 3)
	{
		players[0].playTurn(&gameState);
		turn++;
	}
}


int game::play_game()
{
	playerTurn = 0;
	return 0;
}

