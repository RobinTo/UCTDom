#pragma once

#include "CardManager.h"


class PlayerState
{
public:
	int deck[INSUPPLY];
	int discard[INSUPPLY];
	int hand[INSUPPLY];
	int inPlay[INSUPPLY];
	int buys;
	int actions;

	void drawCards(int cards);
	void endTurn();
	int calculateVictoryPoints(CardManager& cardManager);
	int calculateCurrentMoney(CardManager& cardManager);

	PlayerState();

private:
	int countCards(const int (&cardPile)[INSUPPLY]);
	int pickRandom(const int (&cardPile)[INSUPPLY]);
	void shuffle();
};