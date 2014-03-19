#include "UCTMonteCarlo.h"
#include <iostream>
#include "CardManager.h"
#include <array>

#define NODESTOALLOCATE 20000

Option UCTMonteCarlo::doUCT(int maxSimulations, int UCTPlayer, GameState gameState)
{
	// Create inital root node and its children.
	Node* rootNode = requestNewNode();
	//Node* rootNode = new Node();
	rootNode->isRoot = true;
	rootNode->visited = 0;
	rootNode->currentState = gameState;
	rootNode->playerPlaying = UCTPlayer;
	createAllChildren(rootNode);

	if (rootNode->childrenPtrs.size() == 1)
		return rootNode->childrenPtrs.at(0)->opt;

	// Perform UCT
	for (int i = 0; i < maxSimulations; i++)
	{
		expand(select(rootNode), UCTPlayer);
	}

	// Return best option
	Option bestOption;
	int mostVisited = 0;
	for (int i = 0; i < rootNode->childrenPtrs.size(); i++)
	{
		std::cout << rootNode->childrenPtrs.at(i)->opt.type;
		std::cout << cardManager.cardLookup[rootNode->childrenPtrs.at(i)->opt.absoluteCardId].name;
		std::cout << " visited: " << rootNode->childrenPtrs.at(i)->visited;
		std::cout << " score: " << rootNode->childrenPtrs.at(i)->value << std::endl;

		if (rootNode->childrenPtrs.at(i)->visited > mostVisited)
		{
			bestOption = rootNode->childrenPtrs.at(i)->opt;
			mostVisited = rootNode->childrenPtrs.at(i)->visited;
		}
	}
	//printTree(gameState.turnCounter, UCTPlayer, rootNode);
	resetNodes();
	return bestOption;
}

Node* UCTMonteCarlo::select(Node* root)
{
	Node* currentNode = root;

	while (currentNode->childrenPtrs.size() > 0 && getUntriedChildren(currentNode).size() == 0)
	{
		//currentNode->visited++;
		currentNode = UCTSelectChild(currentNode);
	}
	return currentNode;
}

void UCTMonteCarlo::expand(Node* node, int UCTPlayer)
{
	if (getUntriedChildren(node).size() > 0)
	{
		Node* randomNode = getRandomNode(getUntriedChildren(node));
		//randomNode->visited++;
		rollout(randomNode, randomNode->currentState, UCTPlayer);
	}
	else
	{

		if (node->currentState.gameFinished())
		{
			rollout(node, node->currentState, UCTPlayer);
		}
		else
		{
			createAllChildren(node);
			Node* randomNode = getRandomNode(node->childrenPtrs);
			//randomNode->visited++;
			rollout(randomNode, randomNode->currentState, UCTPlayer);
		}
	}
}

// UCTPlayer is which player we are currently performing UCT for.
void UCTMonteCarlo::rollout(Node* node, GameState gameState, int UCTPlayer)
{
	int currentPlayer = node->playerPlaying;
	if (node->opt.type == END_TURN)
		currentPlayer++;
	if (currentPlayer >= gameState.playerStates.size())
	{
		gameState.turnCounter++;
		currentPlayer = 0;
	}
	while (!gameState.gameFinished())
	{
		while (gameState.playerStates[currentPlayer].buys > 0 || gameState.playerStates[currentPlayer].actions > 0)
		{
			Option cardChosen = getCardPlayoutPolicy(gameState, currentPlayer);
			if (cardChosen.type == ACTION)
			{
				playActionCard(gameState, cardChosen.absoluteCardId, currentPlayer, true);
			}
			else if (cardChosen.type == BUY)
			{
				gameState.playerStates[currentPlayer].actions = 0; // Can not perform more actions after entering buy phase.
				gameState.playerStates[currentPlayer].buyCard(cardManager, cardChosen.absoluteCardId);
				gameState.supplyPiles[cardManager.cardIndexer[cardChosen.absoluteCardId]] -= 1;
			}
			else if (cardChosen.type == END_TURN)
			{
				break;
			}
		}
		gameState.playerStates[currentPlayer].endTurn();
		currentPlayer++;
		if (currentPlayer >= gameState.playerStates.size())
			currentPlayer = 0;
		if (currentPlayer == 0)
			gameState.turnCounter++;
	}

	if (gameState.playerStates.size() == 2)
	{
		if (UCTPlayer == 0)
		{
			double score = gameState.playerStates[UCTPlayer].calculateVictoryPoints(cardManager) > gameState.playerStates[1].calculateVictoryPoints(cardManager) ? 2 : -2;
			//double score = gameState.playerStates[UCTPlayer].calculateVictoryPoints(cardManager);
			propagate(node, score);
		}
		else
		{
			double score = gameState.playerStates[UCTPlayer].calculateVictoryPoints(cardManager) < gameState.playerStates[0].calculateVictoryPoints(cardManager) ? 2 : -2;
			//double score = gameState.playerStates[UCTPlayer].calculateVictoryPoints(cardManager);
			propagate(node, score);
		}
			
	}
	else
		propagate(node, gameState.playerStates[UCTPlayer].calculateVictoryPoints(cardManager));
}

void UCTMonteCarlo::propagate(Node* node, double score)
{
	node->sum += score;
	node->visited++;
	node->value = (double)node->sum / (double)node->visited;
	if (node->opt.type == DRAW)
		node->value *= node->probability;
	if (!node->isRoot)
		propagate(node->parentPtr, score);
}

// Returns best child according to UCT.
Node* UCTMonteCarlo::UCTSelectChild(Node* root)
{
	double bestValue = 0;
	Node* bestNode;
	for (int i = 0; i < root->childrenPtrs.size(); i++)
	{
		double value = (double)root->childrenPtrs.at(i)->value + 10 * sqrt(log((double)root->visited / root->childrenPtrs.at(i)->visited)); // CCCCCCCCC

		if (value >= bestValue || bestValue == 0)
		{
			bestValue = value;
			bestNode = root->childrenPtrs.at(i);
		}
	}

	return bestNode;
}

void UCTMonteCarlo::playActionCard(GameState &gameState, int absoluteCardId, int playerIndex, bool rollout)
{
	switch (absoluteCardId)
	{
	case WOODCUTTER:
		gameState.playerStates[playerIndex].playCard(cardManager, absoluteCardId);
		gameState.playerStates[playerIndex].buys += 1;
		break;
	case FESTIVAL:
		gameState.playerStates[playerIndex].playCard(cardManager, absoluteCardId);
		gameState.playerStates[playerIndex].buys += 2;
		gameState.playerStates[playerIndex].actions += 2;
		break;
	case MONEYLENDER:
		gameState.playerStates[playerIndex].playCard(cardManager, absoluteCardId);
		if (gameState.playerStates[playerIndex].hand[cardManager.cardIndexer[COPPER]] > 0)
		{
			gameState.playerStates[playerIndex].hand[cardManager.cardIndexer[COPPER]]--;
			gameState.trash[cardManager.cardIndexer[COPPER]]++;
			gameState.playerStates[playerIndex].spentMoney -= 3;	// Take away 3 spent money to give money if trashed. TODO: Use less hacky way
		}
		break;
	case SMITHY:
		gameState.playerStates[playerIndex].playCard(cardManager, absoluteCardId);
		if (rollout)
			gameState.playerStates[playerIndex].drawCards(3);
		// Do nothing if in the UCT, because draw nodes are created in tree by probability calculating function.
		break;
	case VILLAGE:
		gameState.playerStates[playerIndex].playCard(cardManager, absoluteCardId);
		gameState.playerStates[playerIndex].actions += 2;
		if (rollout)
			gameState.playerStates[playerIndex].drawCards(1);
		break;
	case MARKET:
		gameState.playerStates[playerIndex].playCard(cardManager, absoluteCardId);
		gameState.playerStates[playerIndex].buys += 1;
		gameState.playerStates[playerIndex].actions += 1;
		if (rollout)
			gameState.playerStates[playerIndex].drawCards(1);
		break;
	case LABORATORY:
		gameState.playerStates[playerIndex].playCard(cardManager, absoluteCardId);
		gameState.playerStates[playerIndex].actions += 1;
		if (rollout)
			gameState.playerStates[playerIndex].drawCards(2);
		break;
	case WITCH:
		gameState.playerStates[playerIndex].playCard(cardManager, absoluteCardId);
		if (gameState.playerStates.size() > 1)
		{
			int tempIndex = (playerIndex == gameState.playerStates.size()-1 ? 0 : playerIndex+1);
			while (tempIndex != playerIndex)
			{
				if (gameState.supplyPiles[cardManager.cardIndexer[CURSE]] > 0)
				{
					gameState.playerStates[tempIndex].discard[cardManager.cardIndexer[CURSE]]++;
					gameState.supplyPiles[cardManager.cardIndexer[CURSE]]--;
				}
				tempIndex++;
				if (tempIndex >= gameState.playerStates.size())
					tempIndex = 0;
			}
		}
		if (rollout)
			gameState.playerStates[playerIndex].drawCards(2);
		break;
	case BUREAUCRAT:
		gameState.playerStates[playerIndex].playCard(cardManager, absoluteCardId);
		if (gameState.supplyPiles[cardManager.cardIndexer[SILVER]] > 0)
		{
			gameState.playerStates[playerIndex].addToTopOfDeck(cardManager.cardIndexer[SILVER]);
			//gameState.playerStates[playerIndex].topOfDeckAsIndex.push(cardManager.cardIndexer(SILVER));
			gameState.supplyPiles[cardManager.cardIndexer[SILVER]]--;
		}
		for (int i = 0; i < gameState.playerStates.size(); i++)
		{
			if (i != playerIndex)
			{
				if (gameState.playerStates[i].hand[cardManager.cardIndexer[ESTATE]] > 0)
				{
					gameState.playerStates[i].addToTopOfDeck(cardManager.cardIndexer[ESTATE]);
					gameState.playerStates[i].hand[cardManager.cardIndexer[ESTATE]]--;
				}
				else if (gameState.playerStates[i].hand[cardManager.cardIndexer[DUCHY]] > 0)
				{
					gameState.playerStates[i].addToTopOfDeck(cardManager.cardIndexer[DUCHY]);
					gameState.playerStates[i].hand[cardManager.cardIndexer[DUCHY]]--;
				}
				else if (gameState.playerStates[i].hand[cardManager.cardIndexer[PROVINCE]] > 0)
				{
					gameState.playerStates[i].addToTopOfDeck(cardManager.cardIndexer[PROVINCE]);
					gameState.playerStates[i].hand[cardManager.cardIndexer[PROVINCE]]--;
				}
				else if (gameState.playerStates[i].hand[cardManager.cardIndexer[GARDENS]] > 0)
				{
					gameState.playerStates[i].addToTopOfDeck(cardManager.cardIndexer[GARDENS]);
					gameState.playerStates[i].hand[cardManager.cardIndexer[GARDENS]]--;
				}
			}
		}
		break;
	default:
		break;
	}
}

void UCTMonteCarlo::createAllChildren(Node* node)
{
	// If parent is end turn, end old players turn and increment turn counter.

	int currentlyPlaying = node->playerPlaying;
	GameState currentState = node->currentState;

	if (node->opt.type == DRAW && node->parentPtr->opt.type == END_TURN)
	{
		currentlyPlaying++;
		if (currentlyPlaying >= node->currentState.playerStates.size())
		{
			currentlyPlaying = 0;
			currentState.turnCounter++;
		}
	}

	if (node->opt.type == END_TURN)
	{
		// Clean-up
		for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
		{
			currentState.playerStates[currentlyPlaying].discard[cardIndex] += (currentState.playerStates[currentlyPlaying].hand[cardIndex]);
			currentState.playerStates[currentlyPlaying].hand[cardIndex] = 0;
			currentState.playerStates[currentlyPlaying].discard[cardIndex] += currentState.playerStates[currentlyPlaying].inPlay[cardIndex];
			currentState.playerStates[currentlyPlaying].inPlay[cardIndex] = 0;
		}
		currentState.playerStates[currentlyPlaying].buys = 1;
		currentState.playerStates[currentlyPlaying].actions = 1;
		currentState.playerStates[currentlyPlaying].spentMoney = 0;

		createDrawNodes(node, currentState, currentlyPlaying, 5);

	}	// If action and a card which contains +cards.
	else if (node->opt.type == ACTION && (node->opt.absoluteCardId == SMITHY || node->opt.absoluteCardId == VILLAGE || node->opt.absoluteCardId == MARKET || node->opt.absoluteCardId == LABORATORY || node->opt.absoluteCardId == WITCH))
	{
		switch (node->opt.absoluteCardId)
		{
		case SMITHY:
			createDrawNodes(node, currentState, currentlyPlaying, 3);
			break;
		case VILLAGE:
			createDrawNodes(node, currentState, currentlyPlaying, 1);
			break;
		case MARKET:
			createDrawNodes(node, currentState, currentlyPlaying, 1);
			break;
		case LABORATORY:
			createDrawNodes(node, currentState, currentlyPlaying, 2);
			break;
		case WITCH:
			createDrawNodes(node, currentState, currentlyPlaying, 2);
			break;
		default:
			break;
		}
	}
	else
	{
		// Create end turn node to enable doing nothing.
		Node* endTurnChild = requestNewNode();
		//Node* endTurnChild = new Node();
		Option o;
		o.type = END_TURN;
		o.absoluteCardId = -1;
		endTurnChild->opt = o;
		endTurnChild->parentPtr = node;
		endTurnChild->currentState = currentState;
		endTurnChild->playerPlaying = currentlyPlaying;
		node->childrenPtrs.push_back(endTurnChild);

		// Add all possible actions.
		if (node->currentState.playerStates[currentlyPlaying].actions > 0)
		{
			std::vector<Option> possibleActions = getActionOptions(&node->currentState, node->currentState.playerStates[currentlyPlaying].hand);
			for (int i = 0; i < possibleActions.size(); i++)
			{
				Node* newActionNode = requestNewNode();
				//Node* newBuyNode = new Node();
				newActionNode->opt = possibleActions.at(i);
				newActionNode->parentPtr = node;
				newActionNode->currentState = currentState;
				newActionNode->playerPlaying = currentlyPlaying;
				playActionCard(newActionNode->currentState, newActionNode->opt.absoluteCardId, newActionNode->playerPlaying, false);
				node->childrenPtrs.push_back(newActionNode);
			}
		}

		// Add all possible buy nodes.
		if (node->currentState.playerStates[currentlyPlaying].buys > 0)
		{
			std::vector<Option> possibleBuys = getBuyOptions(&node->currentState, currentlyPlaying);
			for (int i = 0; i < possibleBuys.size(); i++)
			{
				Node* newBuyNode = requestNewNode();
				//Node* newBuyNode = new Node();
				newBuyNode->opt = possibleBuys.at(i);
				newBuyNode->parentPtr = node;
				newBuyNode->currentState = currentState;
				newBuyNode->playerPlaying = currentlyPlaying;
				newBuyNode->currentState.playerStates[currentlyPlaying].actions = 0;
				// Perform the buy action of this node.
				newBuyNode->currentState.playerStates[currentlyPlaying].buyCard(cardManager, newBuyNode->opt.absoluteCardId);
				newBuyNode->currentState.supplyPiles[cardManager.cardIndexer[newBuyNode->opt.absoluteCardId]] -= 1;
				node->childrenPtrs.push_back(newBuyNode);
			}
		}
	
	}
}

Node* UCTMonteCarlo::getRandomNode(std::vector<Node*> nodes)
{
	int randomNode = rand() % nodes.size();

	return nodes.at(randomNode);
}

std::vector<Node*> UCTMonteCarlo::getUntriedChildren(Node* parent)
{
	std::vector<Node*> untriedNodes;

	for (int i = 0; i < parent->childrenPtrs.size(); i++)
	{
		if (parent->childrenPtrs.at(i)->visited <= 0)
			untriedNodes.push_back(parent->childrenPtrs.at(i));
	}

	return untriedNodes;
}

std::vector<Option> UCTMonteCarlo::getBuyOptions(GameState* gameState, int playerIndex)
{
	int currentMoney = 0;
	currentMoney += gameState->playerStates[playerIndex].hand[cardManager.cardIndexer[COPPER]];
	currentMoney += gameState->playerStates[playerIndex].hand[cardManager.cardIndexer[SILVER]] * 2;
	currentMoney += gameState->playerStates[playerIndex].hand[cardManager.cardIndexer[GOLD]] * 3;
	currentMoney += gameState->playerStates[playerIndex].inPlay[cardManager.cardIndexer[WOODCUTTER]] * 2;
	currentMoney += gameState->playerStates[playerIndex].inPlay[cardManager.cardIndexer[FESTIVAL]] * 2;
	currentMoney += gameState->playerStates[playerIndex].inPlay[cardManager.cardIndexer[MARKET]] * 1;
	currentMoney -= gameState->playerStates[playerIndex].spentMoney;

	std::vector<Option> options;
	for (int i = 0; i < INSUPPLY; i++)
	{
		if (cardManager.cardLookupByIndex[i].cost <= currentMoney && gameState->supplyPiles[i] > 0)
		{
			Option o;
			o.type = BUY;
			o.absoluteCardId = cardManager.cardLookupByIndex[i].id;
			options.push_back(o);
		}
	}

	return options;
}

void UCTMonteCarlo::createDrawNodes(Node* parentNode, GameState& currentState, int currentlyPlaying, int numberOfCards)
{
	GameState copyState = currentState;

	// Check for shuffle
	int cardCounter = 0;
	for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
	{
		cardCounter += copyState.playerStates[currentlyPlaying].deck[cardIndex];
	}
	std::array<int, INSUPPLY> guaranteedCards;// = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // TODO: Not dynamic, must be manually set if INSUPPLY changes!
	for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)  // Dynamic initialization
		guaranteedCards[cardIndex] = 0;

	if (copyState.playerStates[currentlyPlaying].topOfDeckAsIndex.size() > 0)
	{
		while (numberOfCards > 0 && copyState.playerStates[currentlyPlaying].topOfDeckAsIndex.size() > 0)
		{
			numberOfCards--;
			guaranteedCards[copyState.playerStates[currentlyPlaying].topOfDeckAsIndex.top()]++;
			copyState.playerStates[currentlyPlaying].topOfDeckAsIndex.pop();
		}
	}

	// "Shuffle" after making current cards guaranteedcards.
	if (cardCounter < numberOfCards)
	{
		for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
		{
			guaranteedCards[cardIndex] = copyState.playerStates[currentlyPlaying].deck[cardIndex];
			copyState.playerStates[currentlyPlaying].deck[cardIndex] = copyState.playerStates[currentlyPlaying].discard[cardIndex];
			copyState.playerStates[currentlyPlaying].discard[cardIndex] = 0;
		}
	}

	// Finding n and k for draw probability
	int n = 0; // Total cards - those in guaranteedCards and discard. Possible cards to draw.
	std::size_t k = numberOfCards;
	// If less than numberOfCards in cardCounter, then draw fewer cards.
	if (cardCounter < numberOfCards)
		k = numberOfCards - cardCounter;

	// Append each card in deck to string
	std::string s = "";
	for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
	{
		for (int j = 0; j < copyState.playerStates[currentlyPlaying].deck[cardIndex]; j++)
		{
			s.append(cardManager.cardLookupByIndex[cardIndex].charId);
			n++;
		}
	}


	// While there are still new combinations of chars in string, create a new draw.
	std::vector<std::array<int, INSUPPLY>> draws;
	std::vector<double> probabilities;

	// Create one draw if there are enough known cards to draw.
	if (numberOfCards == 0)
	{
		std::array<int, INSUPPLY> draw; // = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };// TODO: sNot dynamic, must be manually set if INSUPPLY changes!
		for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++) // Dynamic initialization
			draw[cardIndex] = guaranteedCards[cardIndex];
		draws.push_back(draw);
		probabilities.push_back(1.0);
	}

	do
	{
		double probability = 0, nkInCardComboPossibilities = 1, nkPossibilities = 1;

		std::array<int, INSUPPLY> draw; // = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };// TODO: sNot dynamic, must be manually set if INSUPPLY changes!
		for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++) // Dynamic initialization
			draw[cardIndex] = 0;

		// This is the combinationString, containing a combination of cards.
		std::string combinationString = std::string(s.begin(), s.begin() + k);

		// For each letter in the combinationString
		for (int stringIndex = 0; stringIndex < k; stringIndex++)
		{
			// Convert letter to int, and add to draw
			std::string cardCharId = std::string(combinationString.begin() + stringIndex, combinationString.begin() + stringIndex + 1);
			draw[cardManager.cardLookupCharToIndex[cardCharId]]++;
		}

		// Find possible ways to draw the draw
		for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
		{
			int nInCardCombo = copyState.playerStates[currentlyPlaying].deck[cardIndex];
			int kInCardCombo = draw[cardIndex];
			nkInCardComboPossibilities *= choose(nInCardCombo, kInCardCombo);
		}

		// Add the guaranteedCards
		for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
		{
			draw[cardIndex] += guaranteedCards[cardIndex];
		}

		// Find the total possible outcomes
		nkPossibilities = choose(n, k);

		// Calculate probability
		probability = nkInCardComboPossibilities / nkPossibilities;

		// Finally, add draw to collection of draws, and probability to collection of probabilities (associate a probability with each draw)
		draws.push_back(draw);
		probabilities.push_back(probability);
	} while (next_combination(s.begin(), s.begin() + k, s.end()));

	// For each draw, create child node
	for (int drawCounter = 0; drawCounter < draws.size(); drawCounter++)
	{
		Node* newNodePtr = requestNewNode();
		newNodePtr->currentState = currentState;

		for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
		{
			newNodePtr->currentState.playerStates[currentlyPlaying].hand[cardIndex] += draws[drawCounter][cardIndex];
			newNodePtr->currentState.playerStates[currentlyPlaying].deck[cardIndex] = copyState.playerStates[currentlyPlaying].deck[cardIndex] - draws[drawCounter][cardIndex] + guaranteedCards[cardIndex];
			newNodePtr->currentState.playerStates[currentlyPlaying].discard[cardIndex] = copyState.playerStates[currentlyPlaying].discard[cardIndex];
			newNodePtr->currentState.supplyPiles[cardIndex] = copyState.supplyPiles[cardIndex];
		}
		parentNode->childrenPtrs.push_back(newNodePtr);
		newNodePtr->parentPtr = parentNode;
		newNodePtr->playerPlaying = currentlyPlaying;
		newNodePtr->opt.absoluteCardId = -2;
		newNodePtr->opt.type = DRAW;
		newNodePtr->probability = probabilities[drawCounter];
	}
}

std::vector<Option> UCTMonteCarlo::getActionOptions(GameState* gameState, const int(&hand)[INSUPPLY])
{
	std::vector<Option> actionOptions;
	if (hand[cardManager.cardIndexer[WOODCUTTER]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = WOODCUTTER;
		actionOptions.push_back(o);
	}
	if (hand[cardManager.cardIndexer[FESTIVAL]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = FESTIVAL;
		actionOptions.push_back(o);
	}
	if (hand[cardManager.cardIndexer[MONEYLENDER]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = MONEYLENDER;
		actionOptions.push_back(o);
	}
	if (hand[cardManager.cardIndexer[SMITHY]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = SMITHY;
		actionOptions.push_back(o);
	}

	if (hand[cardManager.cardIndexer[VILLAGE]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = VILLAGE;
		actionOptions.push_back(o);
	}
	if (hand[cardManager.cardIndexer[MARKET]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = MARKET;
		actionOptions.push_back(o);
	}
	if (hand[cardManager.cardIndexer[LABORATORY]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = LABORATORY;
		actionOptions.push_back(o);
	}
	if (hand[cardManager.cardIndexer[WITCH]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = WITCH;
		actionOptions.push_back(o);
	}
	if (hand[cardManager.cardIndexer[BUREAUCRAT]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = BUREAUCRAT;
		actionOptions.push_back(o);
	}
	return actionOptions;
}

Option UCTMonteCarlo::getCardPlayoutPolicy(GameState& gameState, int playerIndex)
{
	std::vector<Option>::iterator iter;

	Option cardToReturn;
	cardToReturn.type = END_TURN;
	cardToReturn.absoluteCardId = -1;
	int highestCost = 0;

	if (gameState.playerStates[playerIndex].actions > 0)
	{
		std::vector<Option> actionOptions = getActionOptions(&gameState, gameState.playerStates[playerIndex].hand);
		if (actionOptions.size() > 0)
		{
			for (iter = actionOptions.begin(); iter != actionOptions.end(); iter++)
			{
				if (cardManager.cardLookup[(*iter).absoluteCardId].cost > highestCost || highestCost == 0)
				{
					highestCost = cardManager.cardLookup[(*iter).absoluteCardId].cost;
					cardToReturn.type = ACTION;
					cardToReturn.absoluteCardId = (*iter).absoluteCardId;
				}
			}

			return cardToReturn;	// Return early if there was an action card.
		}
	}
	if (gameState.playerStates[playerIndex].buys > 0)
	{
		std::vector<Option> buyOptions = getBuyOptions(&gameState, playerIndex);
		cardToReturn.absoluteCardId = -1;
		highestCost = 0;

		for (iter = buyOptions.begin(); iter != buyOptions.end(); iter++)
		{
			if (cardManager.cardLookup[(*iter).absoluteCardId].cost > highestCost || highestCost == 0 && cardManager.cardLookup[(*iter).absoluteCardId].name != "Curse")
			{
				highestCost = cardManager.cardLookup[(*iter).absoluteCardId].cost;
				cardToReturn.absoluteCardId = (*iter).absoluteCardId;
				cardToReturn.type = BUY;
			}
		}
	}

	return cardToReturn;	// -1 if there were no buys or actions available.
}

// Tree printing
void UCTMonteCarlo::printTree(int turnCounter, int player, Node* rootNodePtr)
{
	//std::cout << "Printing tree" << std::endl;
	std::string fileName = std::to_string(turnCounter) + "_" + std::to_string(player) + "uctTree.gv";
	remove(fileName.c_str());
	std::ofstream file;
	file.open(fileName, std::ios::app);
	std::string text = "digraph ucttree{\r\n size = \"10000000000!, 1000000000\";\r\n ratio = \"expand\";\r\n node[color = lightblue2, style = filled];";
	file << text << std::endl;
	printNode(rootNodePtr, file);
	text = "\r\n }";
	file << text << std::endl;
	file.close();
	//std::cout << "Done printing tree" << std::endl;
}
void UCTMonteCarlo::printNode(Node* nodePtr, std::ofstream& file)
{
	nodePtr->printSelf(file);
	for (std::vector<Node*>::iterator it = nodePtr->childrenPtrs.begin(); it != nodePtr->childrenPtrs.end(); ++it)
		printNode(*it, file);
}


// Node allocation
UCTMonteCarlo::UCTMonteCarlo()
{
	int allocatedNodes = NODESTOALLOCATE;
	emptyNodes.reserve(allocatedNodes);
	usedNodes.reserve(allocatedNodes);
	for (int counter = 0; counter < allocatedNodes; counter++)
	{
		Node* nodePtr = new Node();
		nodePtr->id = allocatedNodes - counter;
		emptyNodes.push_back(nodePtr);
	}
}
UCTMonteCarlo::~UCTMonteCarlo()
{
	for (int counter = 0; counter < emptyNodes.size(); counter++)
	{
		delete emptyNodes[counter];
	}
	emptyNodes.clear();
	//std::vector<Node*>().swap(emptyNodes);
	for (int counter = 0; counter < usedNodes.size(); counter++)
	{
		delete usedNodes[counter];
	}
	usedNodes.clear();
	//std::vector<Node*>().swap(usedNodes);
}
Node* UCTMonteCarlo::requestNewNode()
{
	//std::cout << "Requesting node" << std::endl;
	if (emptyNodes.size() <= 0)
		std::cout << "No more nodes!" << std::endl;

	Node* returnNode = emptyNodes.back();
	usedNodes.push_back(returnNode);
	emptyNodes.pop_back();
	//std::cout << "Done requesting node" << std::endl;
	return returnNode;
}
void UCTMonteCarlo::resetNodes()
{
	//std::cout << "Resetting nodes" << std::endl;
	for (std::vector<Node*>::iterator iterator = usedNodes.begin(); iterator != usedNodes.end(); ++iterator)
	{
		(*iterator)->reset();
		emptyNodes.push_back(*iterator);
	}
	usedNodes.clear();
	//std::cout << "Done resetting nodes" << std::endl;
}

unsigned long long UCTMonteCarlo::choose(unsigned long long n, unsigned long long k)
{
	if (k > n) {
		return 0;
	}
	unsigned long long r = 1;
	for (unsigned long long d = 1; d <= k; ++d) {
		r *= n--;
		r /= d;
	}
	return r;
}


// TODO: Score on the last turn is wrong, because the UCT simulates on too many turns, after moving of turncounter incrementation.
// TODO: Bureaucrat cheats!
// TODO: Visited:1 Cases are weird