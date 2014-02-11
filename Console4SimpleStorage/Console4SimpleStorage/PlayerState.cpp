#include <random>

#include "PlayerState.h"

PlayerState::PlayerState()
{
	for (int index = 0; index < INSUPPLY; index++)
	{
		deck[index] = 0;
		discard[index] = 0;
		hand[index] = 0;
		inPlay[index] = 0;
	}
}

int PlayerState::calculateVictoryPoints(CardManager& cardManager)
{
	int victoryPoints = 0;
	victoryPoints += 1 * deck[cardManager.cardIndexer[ESTATE]];
	victoryPoints += 3 * deck[cardManager.cardIndexer[DUCHY]];
	victoryPoints += 6 * deck[cardManager.cardIndexer[PROVINCE]];
	victoryPoints += 1 * hand[cardManager.cardIndexer[ESTATE]];
	victoryPoints += 3 * hand[cardManager.cardIndexer[DUCHY]];
	victoryPoints += 6 * hand[cardManager.cardIndexer[PROVINCE]];
	return victoryPoints;
}

int PlayerState::countCards(const int (&cardPile)[INSUPPLY])
{
	int cards = 0;
	for (int index = 0; index < INSUPPLY; index ++)
	{
		cards += cardPile[index];
	}
	return cards;
}

void PlayerState::drawCards(int cards)
{
	// For each card to be drawn
	for (int handCounter = 0; handCounter < cards; handCounter ++)
	{
		// If deck is empty, shuffle
		if (countCards(deck) == 0)
			shuffle();

		// Pick a random card
		int cardIndex = pickRandom(deck);
		hand[cardIndex] += 1;
		deck[cardIndex] -= 1;
	}
}

void PlayerState::endTurn()
{
	for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
	{
		discard[cardIndex] += (inPlay[cardIndex] + hand[cardIndex]);
		inPlay[cardIndex] = 0;
		hand[cardIndex] = 0;
	}
	drawCards(5);
}

int PlayerState::pickRandom(const int (&cardPile)[INSUPPLY])
{
	int randomCounter = rand() % countCards(deck) + 1;
	for (int index = 0; index < INSUPPLY; index++)
	{
		randomCounter -= cardPile[index];
		if (randomCounter <= 0)
			return index;
	}
	return -1; //Something went wrong
}

void PlayerState::shuffle()
{
	for (int index = 0; index < INSUPPLY; index++)
	{
		deck[index] += discard[index];
		discard[index] = 0;
	}
}