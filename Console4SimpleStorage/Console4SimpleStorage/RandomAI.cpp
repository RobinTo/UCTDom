#include "RandomAI.h"

RandomAI::RandomAI()
{
}

RandomAI::~RandomAI()
{

}

Option RandomAI::getNextOption(GameState gameState, int playerIndex)
{
	int money = getCurrentMoney(&gameState, playerIndex);

	std::vector<Option> allOptions = getOptions(&gameState, money, playerIndex);

	int r = rand() % allOptions.size();

	return allOptions.at(r);
}

int RandomAI::getCurrentMoney(GameState* gameState, int playerIndex)
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

std::vector<Option> RandomAI::getOptions(GameState* gameState, int currentMoney, int playerIndex)
{
	std::vector<Option> options;

	Option endTurn;
	endTurn.type = END_TURN;
	options.push_back(endTurn);

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
