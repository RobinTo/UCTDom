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

	void buyCard(int absoluteCardId);
	void playCard(int absoluteCardId);
	void drawCards(int cards);
	void endTurn();
	void addToTopOfDeck(int cardIndex);
	int calculateVictoryPoints();
	int calculateCurrentMoney();
	int flipThiefCards(int& absoluteCardId, int& extraCardId);
	std::string printPile(const int(&cardPile)[INSUPPLY]);
	PlayerState();
	~PlayerState();
	void shuffle();
	int countCards();
	int countCards(const int(&cardPile)[INSUPPLY]);
private:
	int pickRandom(const int (&cardPile)[INSUPPLY]);
};