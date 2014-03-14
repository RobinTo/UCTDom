#include <random>

#include "PlayerState.h"

PlayerState::PlayerState()
{
	buys = 1;
	actions = 1;
	spentMoney = 0;

	for (int index = 0; index < INSUPPLY; index++)
	{
		deck[index] = 0;
		discard[index] = 0;
		hand[index] = 0;
		inPlay[index] = 0;
	}
}

void PlayerState::buyCard(CardManager& cardManager, int absoluteCardId)
{
	buys--;											
	discard[cardManager.cardIndexer[absoluteCardId]] += 1;
	spentMoney += cardManager.cardLookup[absoluteCardId].cost;
	spentMoney = spentMoney;

}

void PlayerState::playCard(CardManager& cardManager, int absoluteCardId)
{
	actions--;
	inPlay[cardManager.cardIndexer[absoluteCardId]]++;
	hand[cardManager.cardIndexer[absoluteCardId]]--;
}

std::string PlayerState::printPile(CardManager& cardManager, const int(&cardPile)[INSUPPLY])
{
	std::string textStringToReturn = "";
	for (int index = 0; index < INSUPPLY; index++)
	{
		if (cardPile[index] > 0)
			textStringToReturn += cardManager.cardLookupByIndex[index].name + ":" + std::to_string(cardPile[index]) + " ";
	}
	
	return textStringToReturn + "\r\n";

}

int PlayerState::calculateVictoryPoints(CardManager& cardManager)
{
	int victoryPoints = 0;
	victoryPoints += 1 * deck[cardManager.cardIndexer[ESTATE]];
	victoryPoints += 3 * deck[cardManager.cardIndexer[DUCHY]];
	victoryPoints += 6 * deck[cardManager.cardIndexer[PROVINCE]];
	victoryPoints -= 1 * deck[cardManager.cardIndexer[CURSE]];
	victoryPoints += 1 * deck[cardManager.cardIndexer[GARDENS]] * (countCards() / 10);

	victoryPoints += 1 * hand[cardManager.cardIndexer[ESTATE]];
	victoryPoints += 3 * hand[cardManager.cardIndexer[DUCHY]];
	victoryPoints += 6 * hand[cardManager.cardIndexer[PROVINCE]];
	victoryPoints -= 1 * hand[cardManager.cardIndexer[CURSE]];
	victoryPoints += 1 * hand[cardManager.cardIndexer[GARDENS]] * (countCards()/10);

	victoryPoints += 1 * discard[cardManager.cardIndexer[ESTATE]];
	victoryPoints += 3 * discard[cardManager.cardIndexer[DUCHY]];
	victoryPoints += 6 * discard[cardManager.cardIndexer[PROVINCE]];
	victoryPoints -= 1 * discard[cardManager.cardIndexer[CURSE]];
	victoryPoints += 1 * discard[cardManager.cardIndexer[GARDENS]] * (countCards() / 10);

	victoryPoints += 1 * inPlay[cardManager.cardIndexer[ESTATE]];
	victoryPoints += 3 * inPlay[cardManager.cardIndexer[DUCHY]];
	victoryPoints += 6 * inPlay[cardManager.cardIndexer[PROVINCE]];
	victoryPoints -= 1 * inPlay[cardManager.cardIndexer[CURSE]];
	victoryPoints += 1 * inPlay[cardManager.cardIndexer[GARDENS]] * (countCards() / 10);
	return victoryPoints;
}

int PlayerState::calculateCurrentMoney(CardManager& cardManager)
{
	int currentMoney = 0;
	currentMoney += hand[cardManager.cardIndexer[COPPER]];
	currentMoney += hand[cardManager.cardIndexer[SILVER]] * 2;
	currentMoney += hand[cardManager.cardIndexer[GOLD]] * 3;
	currentMoney += inPlay[cardManager.cardIndexer[WOODCUTTER]] * 2;
	currentMoney += inPlay[cardManager.cardIndexer[FESTIVAL]] * 2;
	currentMoney += inPlay[cardManager.cardIndexer[MARKET]] * 1;

	currentMoney -= spentMoney;
	return currentMoney;
}

int PlayerState::countCards(const int (&cardPile)[INSUPPLY]) // counts the cards in a specific pile (discard, hand, inPlay or deck)
{
	int cards = 0;
	for (int index = 0; index < INSUPPLY; index ++)
	{
		cards += cardPile[index];
	}
	return cards;
}

int PlayerState::countCards() // Counts all cards belonging to the player
{
	int cards = 0;
	for (int index = 0; index < INSUPPLY; index++)
	{
		cards += deck[index];
		cards += hand[index];
		cards += discard[index];
		cards += inPlay[index];
	}
	return cards;
}

void PlayerState::drawCards(int cards)
{
	// For each card to be drawn
	for (int handCounter = 0; handCounter < cards; handCounter ++)
	{
		// If there are any top cards, draw them first
		if (topOfDeckAsIndex.size() > 0)
		{
			deck[topOfDeckAsIndex.top()]--;
			hand[topOfDeckAsIndex.top()]++;
			topOfDeckAsIndex.pop();
		}
		else
		{
			// If deck is empty, shuffle
			if (countCards(deck) == 0)
			{
				// If discard too is empty, then don't draw more cards.
				if (countCards(discard) == 0)
					return;
				else
					shuffle();
			}

			// Pick a random card
			int cardIndex = pickRandom(deck);
			hand[cardIndex] += 1;
			deck[cardIndex] -= 1;
		}
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
	buys = 1;
	actions = 1;
	spentMoney = 0;
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


void PlayerState::addToTopOfDeck(int cardIndex)
{
	topOfDeckAsIndex.push(cardIndex);
	deck[cardIndex]++;
}