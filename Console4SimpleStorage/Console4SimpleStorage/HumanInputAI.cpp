#include "HumanInputAI.h"

HumanInputAI::HumanInputAI()
{

}

HumanInputAI::~HumanInputAI()
{

}

Option HumanInputAI::getNextOption(GameState gameState, int playerIndex, std::vector<Move> moveHistory)
{

	std::cout << "Human players turn, player " + std::to_string(playerIndex) << std::endl;
	std::cout << "Current hand: " + gameState.playerStates[playerIndex].printPile(gameState.playerStates[playerIndex].hand) << std::endl;
	std::cout << "You have " + std::to_string(gameState.playerStates[playerIndex].actions) + " actions and " + std::to_string(gameState.playerStates[playerIndex].buys) + " buys." << std::endl;
	std::cout << "Available options are as follows: " << std::endl;

	int optionMapper = 0;
	std::map<int, Option> mappedOptions;

	if (moveHistory.size() > 2)
	{
		Move lastMove = moveHistory.back();
		int size = moveHistory.size();
		if (lastMove.player == playerIndex && lastMove.absoluteCardId == REMODEL && lastMove.type == ACTION)		// Time to trash a card from remodel.
		{
			std::cout << "You played remodel, choose a card to trash: " << std::endl;
			Option opt;
			opt.type = TRASH;

			std::vector<int> possibleCards;
			for (int i = 0; i < INSUPPLY; i++)
			{
				if (gameState.playerStates[playerIndex].hand[i] > 0)
				{
					std::cout << std::to_string(i) + ". " + CardManager::cardLookupByIndex[i].name << std::endl;
					possibleCards.push_back(i);
				}
			}
			std::cout << "Print the number of the card you wish to trash: " << std::endl;
			int trashCard = -1;
			
			while (true)
			{
				std::cin >> trashCard;

				if (std::find(possibleCards.begin(), possibleCards.end(), trashCard) != possibleCards.end()) {
					opt.absoluteCardId = CardManager::cardLookupByIndex[trashCard].id;
					return opt;
				}
			}
		}
		else if (lastMove.player == playerIndex && moveHistory[size - 2].absoluteCardId == REMODEL && lastMove.type == TRASH) // Time to gain a card from remodel.
		{
			std::cout << "You have played remodel and trashed a card, time to gain a card: " << std::endl;
			std::vector<Option> buyOptions = getBuyOptions(&gameState, CardManager::cardLookup[lastMove.absoluteCardId].cost + 2);

			for (int i = 0; i < buyOptions.size(); i++)
			{
				buyOptions[i].type = GAIN;
				mappedOptions[optionMapper] = buyOptions[i];
				std::cout << std::to_string(optionMapper) + ". " + CardManager::cardLookup[buyOptions[i].absoluteCardId].name << std::endl;
				optionMapper++;
			}
			int cardToGain = -1;

			while (mappedOptions.find(cardToGain) == mappedOptions.end())
			{
				std::cout << "Enter the number of your choice: " << std::endl;
				std::cin >> cardToGain;
			}
			return mappedOptions[cardToGain];
		}
		else if (lastMove.player == playerIndex && lastMove.absoluteCardId == THIEF && lastMove.type == ACTION)
		{
			// Why is this a special case in UCTMonteCarloCpp? Isn't this just time to flip cards and would never happen?
		}
		else if (lastMove.player != playerIndex && moveHistory[size - 2].absoluteCardId == THIEF && lastMove.type == THIEFFLIP)
		{
			std::cout << "You played a thief, you may trash or steal the following cards from the enemy player: " << std::endl;
			Option trash1Opt;
			Option trash2Opt;
			Option gain1Opt;
			Option gain2Opt;
			trash1Opt.type = THIEFTRASH;
			trash1Opt.absoluteCardId = lastMove.absoluteCardId;
			trash2Opt.type = THIEFTRASH;
			trash2Opt.absoluteCardId = lastMove.absoluteExtraCardId;
			gain1Opt.type = THIEFGAIN;
			gain1Opt.absoluteCardId = lastMove.absoluteCardId;
			gain2Opt.type = THIEFGAIN;
			gain2Opt.absoluteCardId = lastMove.absoluteExtraCardId;

			bool hasOptions = false;
			bool hasOptions2 = false;
			if (trash1Opt.absoluteCardId == COPPER || trash1Opt.absoluteCardId == SILVER || trash1Opt.absoluteCardId == GOLD)
			{
				std::cout << "1. Trash " << CardManager::cardLookup[trash1Opt.absoluteCardId].name << std::endl;
				std::cout << "2. Gain " << CardManager::cardLookup[gain1Opt.absoluteCardId].name << std::endl;
				hasOptions = true;
			}
			if (trash2Opt.absoluteCardId == COPPER || trash2Opt.absoluteCardId == SILVER || trash2Opt.absoluteCardId == GOLD)
			{
				std::cout << "3. Trash " << CardManager::cardLookup[trash2Opt.absoluteCardId].name << std::endl;
				std::cout << "4. Gain " << CardManager::cardLookup[gain2Opt.absoluteCardId].name << std::endl;
				hasOptions2 = true;
			}
			int choice = -1;

			if (hasOptions && hasOptions2)
			{
				while (choice < 1 || choice > 4)
				{
					std::cin >> choice;
				}
			}
			else if (hasOptions && !hasOptions2)
			{
				while (choice < 1 || choice > 2)
				{
					std::cin >> choice;
				}
			}
			else if (hasOptions2 && !hasOptions)
			{
				while (choice < 3 || choice > 4)
				{
					std::cin >> choice;
				}
			}

			if (choice == 1)
			{
				return trash1Opt;
			}
			else if (choice == 2)
			{
				return gain1Opt;
			}
			else if (choice == 3)
			{
				return trash2Opt;
			}
			else if (choice == 4)
			{
				return gain2Opt;
			}
			else
			{
				std::cout << "Opponent did not flip any treasure cards. The following are now your new options: " << std::endl;
			}
		}
	}



	Option endTurnOption;
	endTurnOption.type = END_TURN;

	std::vector<Option> buyOptions = getBuyOptions(&gameState, getCurrentMoney(&gameState, playerIndex));
	std::vector<Option> actionOptions = getActionOptions(&gameState, gameState.playerStates[playerIndex].hand);
	std::cout << "0. End Turn." << std::endl;
	
	mappedOptions[optionMapper] = endTurnOption;
	optionMapper++;

	if (gameState.playerStates[playerIndex].actions > 0)
	{
		for (int i = 0; i < actionOptions.size(); i++)
		{
			std::cout << std::to_string(optionMapper) + ". Play a " + CardManager::cardLookup[actionOptions[i].absoluteCardId].name << std::endl;
			mappedOptions[optionMapper] = actionOptions[i];
			optionMapper++;
		}
	}
	if (gameState.playerStates[playerIndex].buys > 0)
	{
		for (int i = 0; i < buyOptions.size(); i++)
		{
			std::cout << std::to_string(optionMapper) + ". Buy a " + CardManager::cardLookup[buyOptions[i].absoluteCardId].name << std::endl;
			mappedOptions[optionMapper] = buyOptions[i];
			optionMapper++;
		}
	}

	int choice = -1;
	while (mappedOptions.find(choice) == mappedOptions.end())
	{
		std::cout << "Enter the number of your choice: " << std::endl;
		std::cin >> choice;
	}

	return mappedOptions[choice];
}

std::vector<Option> HumanInputAI::getBuyOptions(GameState* gameState, int currentMoney)
{
	std::vector<Option> options;
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

	return options;
}

std::vector<Option> HumanInputAI::getActionOptions(GameState* gameState, const int(&hand)[INSUPPLY])
{
	std::vector<Option> actionOptions;
	if (hand[CardManager::cardIndexer[WOODCUTTER]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = WOODCUTTER;
		actionOptions.push_back(o);
	}
	if (hand[CardManager::cardIndexer[FESTIVAL]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = FESTIVAL;
		actionOptions.push_back(o);
	}
	if (hand[CardManager::cardIndexer[MONEYLENDER]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = MONEYLENDER;
		actionOptions.push_back(o);
	}
	if (hand[CardManager::cardIndexer[SMITHY]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = SMITHY;
		actionOptions.push_back(o);
	}

	if (hand[CardManager::cardIndexer[VILLAGE]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = VILLAGE;
		actionOptions.push_back(o);
	}
	if (hand[CardManager::cardIndexer[MARKET]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = MARKET;
		actionOptions.push_back(o);
	}
	if (hand[CardManager::cardIndexer[LABORATORY]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = LABORATORY;
		actionOptions.push_back(o);
	}
	if (hand[CardManager::cardIndexer[WITCH]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = WITCH;
		actionOptions.push_back(o);
	}
	if (hand[CardManager::cardIndexer[BUREAUCRAT]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = BUREAUCRAT;
		actionOptions.push_back(o);
	}
	if (hand[CardManager::cardIndexer[REMODEL]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = REMODEL;
		actionOptions.push_back(o);
	}
	if (hand[CardManager::cardIndexer[THIEF]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = THIEF;
		actionOptions.push_back(o);
	}
	return actionOptions;
}

int HumanInputAI::getCurrentMoney(GameState* gameState, int playerIndex)
{
	int currentMoney = 0;
	currentMoney += gameState->playerStates[playerIndex].hand[CardManager::cardIndexer[COPPER]];
	currentMoney += gameState->playerStates[playerIndex].hand[CardManager::cardIndexer[SILVER]] * 2;
	currentMoney += gameState->playerStates[playerIndex].hand[CardManager::cardIndexer[GOLD]] * 3;
	currentMoney += gameState->playerStates[playerIndex].inPlay[CardManager::cardIndexer[WOODCUTTER]] * 2;
	currentMoney += gameState->playerStates[playerIndex].inPlay[CardManager::cardIndexer[FESTIVAL]] * 2;
	currentMoney += gameState->playerStates[playerIndex].inPlay[CardManager::cardIndexer[MARKET]] * 1;
	currentMoney -= gameState->playerStates[playerIndex].spentMoney;
	return currentMoney;
}