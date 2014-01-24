#include <vector>
#include <string>

#include "playerstate.h"
#include "gamestate.h"

#ifndef __PLAYER_H__
#define __PLAYER_H__

class Player
{
private:
	GameState* gameStatePtr;
	std::vector<PlayerState*> otherStatePtrs;
	PlayerState* selfStatePtr;

	void draw(int numberOfCards);
	void discard(Card* cardPtr);
	void play(Card* cardPtr);
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