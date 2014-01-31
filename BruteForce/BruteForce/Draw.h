#pragma once

#include <vector>

#include "Card.h"

class Draw
{
public:
	std::vector<Card*> currentDeck;
	std::vector<Card*> currentDraw;

	int copper, silver, gold, estate, duchy, province;

	Draw();
	Draw(std::vector<Card*> deck);
	Draw(std::vector<Card*> deck, Card* removeCard);
	Draw(std::vector<Card*> deck, std::vector<Card*> draw);
	Draw(std::vector<Card*> deck, std::vector<Card*> draw, Card* removeCard);

};