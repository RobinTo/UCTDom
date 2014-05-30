#include "DoubleWitchAI.h"

DoubleWitchAI::DoubleWitchAI()
{
}

DoubleWitchAI::~DoubleWitchAI()
{

}

Option DoubleWitchAI::getNextOption(GameState gameState, int playerIndex)
{
	Option endTurn;
	endTurn.type = END_TURN;

	int money = getCurrentMoney(&gameState, playerIndex);

	// std::vector<Option> buyOptions = getBuyOptions(&gameState, money, playerIndex);
	std::vector<Option> actionOptions = getActionOptions(&gameState, playerIndex);

	if (actionOptions.size() > 0)
		return actionOptions.at(0);
	else if (gameState.playerStates[playerIndex].buys > 0)
	{
		Option buyOption;
		buyOption.type = BUY;
		if (gameState.supplyPiles[CardManager::cardIndexer[PROVINCE]] > 0 && money >= CardManager::cardLookup[PROVINCE].cost && gameState.playerStates[playerIndex].countCardInDeck(CardManager::cardIndexer[GOLD]) > 0)
			buyOption.absoluteCardId = CardManager::cardLookup[PROVINCE].id;
		else if (gameState.supplyPiles[CardManager::cardIndexer[WITCH]] > 0 && money >= CardManager::cardLookup[WITCH].cost && gameState.playerStates[playerIndex].countCardInDeck(CardManager::cardIndexer[WITCH]) < 2)
			buyOption.absoluteCardId = CardManager::cardLookup[WITCH].id;
		else if (gameState.supplyPiles[CardManager::cardIndexer[DUCHY]] > 0 && money >= CardManager::cardLookup[DUCHY].cost && gameState.supplyPiles[CardManager::cardIndexer[PROVINCE]] < 4)
			buyOption.absoluteCardId = CardManager::cardLookup[DUCHY].id;
		else if (gameState.supplyPiles[CardManager::cardIndexer[ESTATE]] > 0 && money >= CardManager::cardLookup[ESTATE].cost && gameState.supplyPiles[CardManager::cardIndexer[PROVINCE]] < 2)
			buyOption.absoluteCardId = CardManager::cardLookup[ESTATE].id;
		else if (gameState.supplyPiles[CardManager::cardIndexer[GOLD]] > 0 && money >= CardManager::cardLookup[GOLD].cost)
			buyOption.absoluteCardId = CardManager::cardLookup[GOLD].id;
		else if (gameState.supplyPiles[CardManager::cardIndexer[SILVER]] > 0 && money >= CardManager::cardLookup[SILVER].cost)
			buyOption.absoluteCardId = CardManager::cardLookup[SILVER].id;
		else
			return endTurn;

		return buyOption;
	}
	return endTurn;
}

int DoubleWitchAI::getCurrentMoney(GameState* gameState, int playerIndex)
{
	int currentMoney = 0;
	currentMoney += gameState->playerStates[playerIndex].hand[CardManager::cardIndexer[COPPER]];
	currentMoney += gameState->playerStates[playerIndex].hand[CardManager::cardIndexer[SILVER]] * 2;
	currentMoney += gameState->playerStates[playerIndex].hand[CardManager::cardIndexer[GOLD]] * 3;
	currentMoney += gameState->playerStates[playerIndex].inPlay[CardManager::cardIndexer[WOODCUTTER]] * WOODCUTTERMONEY;
	currentMoney += gameState->playerStates[playerIndex].inPlay[CardManager::cardIndexer[FESTIVAL]] * 2;
	currentMoney += gameState->playerStates[playerIndex].inPlay[CardManager::cardIndexer[MARKET]] * 1;
	currentMoney -= gameState->playerStates[playerIndex].spentMoney;
	return currentMoney;
}

std::vector<Option> DoubleWitchAI::getBuyOptions(GameState* gameState, int currentMoney, int playerIndex)
{
	std::vector<Option> options;

	if (gameState->playerStates[playerIndex].buys > 0)
	{
		for (int i = 0; i < INSUPPLY; i++)
		{
			if (CardManager::cardLookupByIndex[i].cost <= currentMoney && gameState->supplyPiles[i] > 0)
			{
				Option o;
				o.type = BUY;
				o.absoluteCardId = CardManager::cardLookupByIndex[i].id;
				options.push_back(o);
			}
		}
	}

	return options;
}

std::vector<Option> DoubleWitchAI::getActionOptions(GameState* gameState, int playerIndex)
{
	std::vector<Option> options;

	if (gameState->playerStates[playerIndex].actions > 0)
	{
		if (gameState->playerStates[playerIndex].hand[CardManager::cardIndexer[WOODCUTTER]] > 0)
		{
			Option o;
			o.type = ACTION;
			o.absoluteCardId = WOODCUTTER;
			options.push_back(o);
		}
		if (gameState->playerStates[playerIndex].hand[CardManager::cardIndexer[FESTIVAL]] > 0)
		{
			Option o;
			o.type = ACTION;
			o.absoluteCardId = FESTIVAL;
			options.push_back(o);
		}
		if (gameState->playerStates[playerIndex].hand[CardManager::cardIndexer[MONEYLENDER]] > 0)
		{
			Option o;
			o.type = ACTION;
			o.absoluteCardId = MONEYLENDER;
			options.push_back(o);
		}
		if (gameState->playerStates[playerIndex].hand[CardManager::cardIndexer[SMITHY]] > 0)
		{
			Option o;
			o.type = ACTION;
			o.absoluteCardId = SMITHY;
			options.push_back(o);
		}

		if (gameState->playerStates[playerIndex].hand[CardManager::cardIndexer[VILLAGE]] > 0)
		{
			Option o;
			o.type = ACTION;
			o.absoluteCardId = VILLAGE;
			options.push_back(o);
		}
		if (gameState->playerStates[playerIndex].hand[CardManager::cardIndexer[MARKET]] > 0)
		{
			Option o;
			o.type = ACTION;
			o.absoluteCardId = MARKET;
			options.push_back(o);
		}
		if (gameState->playerStates[playerIndex].hand[CardManager::cardIndexer[LABORATORY]] > 0)
		{
			Option o;
			o.type = ACTION;
			o.absoluteCardId = LABORATORY;
			options.push_back(o);
		}
		if (gameState->playerStates[playerIndex].hand[CardManager::cardIndexer[WITCH]] > 0)
		{
			Option o;
			o.type = ACTION;
			o.absoluteCardId = WITCH;
			options.push_back(o);
		}
		if (gameState->playerStates[playerIndex].hand[CardManager::cardIndexer[BUREAUCRAT]] > 0)
		{
			Option o;
			o.type = ACTION;
			o.absoluteCardId = BUREAUCRAT;
			options.push_back(o);
		}
		if (gameState->playerStates[playerIndex].hand[CardManager::cardIndexer[REMODEL]] > 0)
		{
			Option o;
			o.type = ACTION;
			o.absoluteCardId = REMODEL;
			options.push_back(o);
		}
		if (gameState->playerStates[playerIndex].hand[CardManager::cardIndexer[THIEF]] > 0)
		{
			Option o;
			o.type = ACTION;
			o.absoluteCardId = THIEF;
			options.push_back(o);
		}
	}

	return options;

}