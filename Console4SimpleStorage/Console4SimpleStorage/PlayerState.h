#pragma once

#include <string>
#include <stack>

#include "CardManager.h"



class PlayerState
{
public:
	int deck[INSUPPLY];
	int discard[INSUPPLY];
	int hand[INSUPPLY];
	int inPlay[INSUPPLY];
	std::stack<int> topOfDeckAsIndex;
	int buys;
	int actions;
	int spentMoney;

	void buyCard(CardManager& cardManager, int absoluteCardId);
	void playCard(CardManager& cardManager, int absoluteCardId);
	void drawCards(int cards);
	void endTurn();
	void addToTopOfDeck(int absoluteID);
	int calculateVictoryPoints(CardManager& cardManager);
	int calculateCurrentMoney(CardManager& cardManager);
	std::string printPile(CardManager& cardManager, const int(&cardPile)[INSUPPLY]);
	PlayerState();

private:
	int countCards();
	int countCards(const int (&cardPile)[INSUPPLY]);
	int pickRandom(const int (&cardPile)[INSUPPLY]);
	void shuffle();
};