#include "GardensAI.h"

GardensAI::GardensAI()
{
}

GardensAI::~GardensAI()
{

}

Option GardensAI::getNextOption(GameState gameState, int playerIndex)
{
	if (gameState.playerStates[playerIndex].buys == 0)
	{
		Option o;
		o.type = END_TURN;
		o.absoluteCardId = -2;
		return o;
	}

	int money = gameState.playerStates[playerIndex].calculateCurrentMoney();
	Option o;
	o.type = BUY;
	if (gameState.turnCounter > 15)
	{
		if (money >= 8 && gameState.supplyPiles[CardManager::cardIndexer[PROVINCE]] > 0)
		{
			o.absoluteCardId = CardManager::cardLookup[PROVINCE].id;
		}
		else if (money >= 4 && gameState.supplyPiles[CardManager::cardIndexer[GARDENS]] > 0)
		{
			o.absoluteCardId = CardManager::cardLookup[GARDENS].id;
		}
		else if (money >= 5 && gameState.supplyPiles[CardManager::cardIndexer[DUCHY]] > 0)
		{
			o.absoluteCardId = CardManager::cardLookup[DUCHY].id;
		}
		else if (money >= 2 && gameState.supplyPiles[CardManager::cardIndexer[ESTATE]] > 0)
		{
			o.absoluteCardId = CardManager::cardLookup[ESTATE].id;
		}
		else
		{
			o.type = END_TURN;
			o.absoluteCardId = -1;
		}
	}
	else
	{
		if (money >= 6 && gameState.supplyPiles[CardManager::cardIndexer[GOLD]] > 0)
		{
			o.absoluteCardId = CardManager::cardLookup[GOLD].id;
		}
		else if (money >= 3 && gameState.supplyPiles[CardManager::cardIndexer[SILVER]] > 0)
		{
			o.absoluteCardId = CardManager::cardLookup[SILVER].id;
		}
		else
		{
			o.type = END_TURN;
			o.absoluteCardId = -1;
		}
	}
	return o;
}

Card GardensAI::findBestAction(GameState gameState, int playerIndex)
{
	return Card();
}

Card GardensAI::findBestBuy(GameState gameState, int playerIndex)
{
	int money = gameState.playerStates[playerIndex].calculateCurrentMoney();
	Card cardToBuy;

	if (money >= 4 && gameState.supplyPiles[CardManager::cardIndexer[GARDENS]] > 0)
	{
		cardToBuy = CardManager::cardLookup[GARDENS];
	}
	return Card();
}




/*
Will buy as follows if affordable:
- Gardens
- Festival
- WoodCutter
- Workshop
- Estate




*/