#include <vector>
#include <string>

#include "playerstate.h"
#include "gamestate.h"
#include "uct_ai.h"

#ifndef __PLAYER_H__
#define __PLAYER_H__

class Player
{
private:
	GameState* gameStatePtr;
	std::vector<PlayerState*> otherStatePtrs;
	PlayerState* selfStatePtr;
	Uct_Ai uctai;

	void draw(int numberOfCards);
	void gain(Card* cardPtr);
	void shuffle();
public:
	std::string name;
	int id;

	Player(GameState* gameStatePtr2, std::vector<PlayerState*> otherStatePtrs2, PlayerState* selfStatePtr2);

	void startUp(std::vector<Card*> startDeck);
	void playTurn();
	void endTurn();
};


#endif