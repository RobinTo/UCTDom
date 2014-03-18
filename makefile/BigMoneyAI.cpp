#include "BigMoneyAI.h"

BigMoneyAI::BigMoneyAI()
{
}

BigMoneyAI::~BigMoneyAI()
{

}

Option BigMoneyAI::getNextOption(GameState gameState, int playerIndex)
{
	if (gameState.playerStates[playerIndex].buys == 0)
	{
		Option o;
		o.type = END_TURN;
		o.absoluteCardId = -2;
		return o;
	}

	int money = gameState.playerStates[playerIndex].calculateCurrentMoney(cardManager);
	Option o;
	o.type = BUY;
	if (gameState.turnCounter > 15)
	{
		if (money >= 8 && gameState.supplyPiles[cardManager.cardIndexer[PROVINCE]] > 0)
		{
			o.absoluteCardId = cardManager.cardLookup[PROVINCE].id;
		}
		else if (money >= 4 && gameState.supplyPiles[cardManager.cardIndexer[GARDENS]] > 0)
		{
			o.absoluteCardId = cardManager.cardLookup[GARDENS].id;
		}
		else if (money >= 5 && gameState.supplyPiles[cardManager.cardIndexer[DUCHY]] > 0)
		{
			o.absoluteCardId = cardManager.cardLookup[DUCHY].id;
		}
		else if (money >= 2 && gameState.supplyPiles[cardManager.cardIndexer[ESTATE]] > 0)
		{
			o.absoluteCardId = cardManager.cardLookup[ESTATE].id;
		}
		else
		{
			o.type = END_TURN;
			o.absoluteCardId = -1;
		}
	}
	else
	{
		if (money >= 6 && gameState.supplyPiles[cardManager.cardIndexer[GOLD]] > 0)
		{
			o.absoluteCardId = cardManager.cardLookup[GOLD].id;
		}
		else if (money >= 3 && gameState.supplyPiles[cardManager.cardIndexer[SILVER]] > 0)
		{
			o.absoluteCardId = cardManager.cardLookup[SILVER].id;
		}
		else
		{
			o.type = END_TURN;
			o.absoluteCardId = -1;
		}
	}
	return o;
}