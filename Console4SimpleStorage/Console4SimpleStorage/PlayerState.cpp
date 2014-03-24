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

	std::stack<int>().swap(topOfDeckAsIndex);
}

PlayerState::~PlayerState()
{
}

void PlayerState::buyCard(int absoluteCardId)
{
	buys--;											
	discard[CardManager::cardIndexer[absoluteCardId]] += 1;
	spentMoney += CardManager::cardLookup[absoluteCardId].cost;
	spentMoney = spentMoney;

}

void PlayerState::playCard(int absoluteCardId)
{
	actions--;
	inPlay[CardManager::cardIndexer[absoluteCardId]]++;
	hand[CardManager::cardIndexer[absoluteCardId]]--;
}

std::string PlayerState::printPile(const int(&cardPile)[INSUPPLY])
{
	std::string textStringToReturn = "";
	for (int index = 0; index < INSUPPLY; index++)
	{
		if (cardPile[index] > 0)
			textStringToReturn += CardManager::cardLookupByIndex[index].name + ":" + std::to_string(cardPile[index]) + " ";
	}
	
	return textStringToReturn + "\r\n";

}

int PlayerState::calculateVictoryPoints()
{
	int victoryPoints = 0;
	victoryPoints += 1 * deck[CardManager::cardIndexer[ESTATE]];
	victoryPoints += 3 * deck[CardManager::cardIndexer[DUCHY]];
	victoryPoints += 6 * deck[CardManager::cardIndexer[PROVINCE]];
	victoryPoints -= 1 * deck[CardManager::cardIndexer[CURSE]];
	victoryPoints += 1 * deck[CardManager::cardIndexer[GARDENS]] * (countCards() / 10);

	victoryPoints += 1 * hand[CardManager::cardIndexer[ESTATE]];
	victoryPoints += 3 * hand[CardManager::cardIndexer[DUCHY]];
	victoryPoints += 6 * hand[CardManager::cardIndexer[PROVINCE]];
	victoryPoints -= 1 * hand[CardManager::cardIndexer[CURSE]];
	victoryPoints += 1 * hand[CardManager::cardIndexer[GARDENS]] * (countCards()/10);

	victoryPoints += 1 * discard[CardManager::cardIndexer[ESTATE]];
	victoryPoints += 3 * discard[CardManager::cardIndexer[DUCHY]];
	victoryPoints += 6 * discard[CardManager::cardIndexer[PROVINCE]];
	victoryPoints -= 1 * discard[CardManager::cardIndexer[CURSE]];
	victoryPoints += 1 * discard[CardManager::cardIndexer[GARDENS]] * (countCards() / 10);

	victoryPoints += 1 * inPlay[CardManager::cardIndexer[ESTATE]];
	victoryPoints += 3 * inPlay[CardManager::cardIndexer[DUCHY]];
	victoryPoints += 6 * inPlay[CardManager::cardIndexer[PROVINCE]];
	victoryPoints -= 1 * inPlay[CardManager::cardIndexer[CURSE]];
	victoryPoints += 1 * inPlay[CardManager::cardIndexer[GARDENS]] * (countCards() / 10);
	return victoryPoints;
}

int PlayerState::calculateCurrentMoney()
{
	int currentMoney = 0;
	currentMoney += hand[CardManager::cardIndexer[COPPER]];
	currentMoney += hand[CardManager::cardIndexer[SILVER]] * 2;
	currentMoney += hand[CardManager::cardIndexer[GOLD]] * 3;
	currentMoney += inPlay[CardManager::cardIndexer[WOODCUTTER]] * 2;
	currentMoney += inPlay[CardManager::cardIndexer[FESTIVAL]] * 2;
	currentMoney += inPlay[CardManager::cardIndexer[MARKET]] * 1;

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

int PlayerState::flipThiefCards(int& absoluteCardId, int& extraCardId)
{
	// For each card to be flipped
	for (int index = 0; index < 2; index++)
	{
		// If there are any top cards, flip them first
		if (topOfDeckAsIndex.size() > 0)
		{
			deck[topOfDeckAsIndex.top()]--;
			discard[topOfDeckAsIndex.top()]++;
			if (index == 0)
				absoluteCardId = CardManager::cardLookupByIndex[topOfDeckAsIndex.top()].id;
			else
				extraCardId = CardManager::cardLookupByIndex[topOfDeckAsIndex.top()].id;
			topOfDeckAsIndex.pop();
		}
		else
		{
			// If deck is empty, shuffle
			if (countCards(deck) == 0)
			{
				// If discard too is empty, then don't draw more cards.
				if (countCards(discard) == 0)
					return index;
				else
					shuffle();
			}

			int cardIndex = pickRandom(deck);
			if (index == 0)
				absoluteCardId = CardManager::cardLookupByIndex[cardIndex].id;
			else
				extraCardId = CardManager::cardLookupByIndex[cardIndex].id;
			deck[cardIndex]--;
			discard[cardIndex]++;
		}
	}
	return 2;
}