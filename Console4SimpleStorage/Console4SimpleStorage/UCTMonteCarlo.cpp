#include "UCTMonteCarlo.h"
#include <iostream>
#include "CardManager.h"
Option UCTMonteCarlo::doUCT(int maxSimulations, int UCTPlayer, GameState gameState)
{
	// Create inital root node and its children.
	Node* rootNode = new Node();
	rootNode->isRoot = true;
	rootNode->visited = 1;
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
		std::cout << cardManager.cardLookupByIndex[rootNode->childrenPtrs.at(i)->opt.card].name;
		std::cout << " visited: " << rootNode->childrenPtrs.at(i)->visited;
		std::cout << " score: " << rootNode->childrenPtrs.at(i)->value << std::endl;
		if (rootNode->childrenPtrs.at(i)->visited > mostVisited)
		{
			bestOption = rootNode->childrenPtrs.at(i)->opt;
			mostVisited = rootNode->childrenPtrs.at(i)->visited;
		}
	}
	return bestOption;
}

Node* UCTMonteCarlo::select(Node* root)
{
	Node* currentNode = root;
	while (currentNode->childrenPtrs.size() > 0 && getUntriedChilds(currentNode).size() == 0)
	{
		currentNode->visited++;
		currentNode = UCTSelectChild(currentNode);
	}
	return currentNode;
}

void UCTMonteCarlo::expand(Node* node, int UCTPlayer)
{
	if (getUntriedChilds(node).size() > 0)
	{
		Node* randomNode = getRandomNode(getUntriedChilds(node));
		randomNode->visited++;
		rollout(randomNode, randomNode->currentState, UCTPlayer);
	}
	else
	{
		createAllChildren(node);
		if (node->childrenPtrs.size() > 0)
		{
			Node* randomNode = getRandomNode(node->childrenPtrs);
			randomNode->visited++;
			rollout(randomNode, randomNode->currentState, UCTPlayer);
		}
		else
		{
			rollout(node, node->currentState, UCTPlayer);
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
		currentPlayer = 0;

	while (!gameState.gameFinished())
	{
		while (gameState.playerStates[currentPlayer].buys > 0)
		{
			int cardChosen = getCardPlayoutPolicy(gameState, currentPlayer);
			gameState.playerStates[currentPlayer].buyCard(cardManager, cardChosen);
		}
		gameState.playerStates[currentPlayer].endTurn();
		currentPlayer++;
		if (currentPlayer >= gameState.playerStates.size())
			currentPlayer = 0;
		if (currentPlayer == 0)
			gameState.turnCounter++;
	}

	propagate(node, gameState.playerStates[UCTPlayer].calculateVictoryPoints(cardManager));
}

void UCTMonteCarlo::propagate(Node* node, double score)
{
	node->sum += score;
	node->value = node->sum / node->visited;
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
		double value = (double)root->childrenPtrs.at(i)->value + 10 * sqrt(log((double)root->visited / root->childrenPtrs.at(i)->visited));

		if (value >= bestValue)
		{
			bestValue = value;
			bestNode = root->childrenPtrs.at(i);
		}
	}

	return bestNode;
}

void UCTMonteCarlo::createAllChildren(Node* node)
{
	// If parent is end turn, end old players turn and increment turn counter.

	int currentlyPlaying = node->playerPlaying;
	GameState currentState = node->currentState;
	if (node->opt.type == END_TURN)
	{
		currentState.playerStates[currentlyPlaying].endTurn();
		currentlyPlaying++;
		if (currentlyPlaying >= node->currentState.playerStates.size())
			currentlyPlaying = 0;
	}

	if (node->currentState.gameFinished())
	{
		Node* endTurnChild = new Node();
		Option o;
		o.type = END_TURN;
		endTurnChild->opt = o;
		endTurnChild->parentPtr = node;
		endTurnChild->currentState = currentState;
		endTurnChild->playerPlaying = currentlyPlaying;
		endTurnChild->currentState.playerStates[currentlyPlaying].endTurn();
		node->childrenPtrs.push_back(endTurnChild);
		return;
	}

	// Create end turn node to enable doing nothing.
	Node* endTurnChild = new Node();
	Option o;
	o.type = END_TURN;
	endTurnChild->opt = o;
	endTurnChild->parentPtr = node;
	endTurnChild->currentState = currentState;
	endTurnChild->playerPlaying = currentlyPlaying;
	endTurnChild->currentState.playerStates[currentlyPlaying].endTurn();
	node->childrenPtrs.push_back(endTurnChild);

	// Add all possible actions.
	/*
	std::vector<Option> possibleActions = getActionOptions();
	for (int i = 0; i < possibleActions.size(); i++)
	{
		Node* newActionNode;
		newActionNode->opt = possibleActions.at(i);
		newActionNode->parentPtr = node;
		newActionNode->currentState = currentState;
		newActionNode->playerPlaying = currentlyPlaying;
		node->childrenPtrs.push_back(newActionNode);
	}
	*/

	// Add all possible buy nodes.
	if (node->currentState.playerStates[node->playerPlaying].buys > 0)
	{
		std::vector<Option> possibleBuys = getBuyOptions(&node->currentState, node->currentState.playerStates[node->playerPlaying].hand);
		for (int i = 0; i < possibleBuys.size(); i++)
		{
			Node* newBuyNode = new Node();
			newBuyNode->opt = possibleBuys.at(i);
			newBuyNode->parentPtr = node;
			newBuyNode->currentState = currentState;
			newBuyNode->playerPlaying = currentlyPlaying;
			// Perform the buy action of this node.
			newBuyNode->currentState.playerStates[currentlyPlaying].buyCard(cardManager, newBuyNode->opt.card);
			node->childrenPtrs.push_back(newBuyNode);
		}
	}
}

Node* UCTMonteCarlo::getRandomNode(std::vector<Node*> nodes)
{
	int randomNode = rand() % nodes.size();

	return nodes.at(randomNode);
}

std::vector<Node*> UCTMonteCarlo::getUntriedChilds(Node* parent)
{
	std::vector<Node*> untriedNodes;

	for (int i = 0; i < parent->childrenPtrs.size(); i++)
	{
		if (parent->childrenPtrs.at(i)->visited <= 0)
			untriedNodes.push_back(parent->childrenPtrs.at(i));
	}

	return untriedNodes;
}

std::vector<Option> UCTMonteCarlo::getBuyOptions(GameState* gameState, const int(&hand)[INSUPPLY])
{
	int currentMoney = 0;
	currentMoney += hand[cardManager.cardIndexer[COPPER]];
	currentMoney += hand[cardManager.cardIndexer[SILVER]] * 2;
	currentMoney += hand[cardManager.cardIndexer[GOLD]] * 3;

	std::vector<Option> options;
	for (int i = 0; i < sizeof(hand) / sizeof(*hand); i++)
	{
		if (cardManager.cardLookupByIndex[i].cost <= currentMoney && gameState->supplyPiles[i] > 0)
		{
			Option o;
			o.type = BUY;
			o.card = i;
			options.push_back(o);
		}
	}

	return options;
}

std::vector<Option> UCTMonteCarlo::getActionOptions()
{
	std::vector<Option> actionOptions;
	return actionOptions;
}

int UCTMonteCarlo::getCardPlayoutPolicy(GameState& gameState, int playerIndex)
{
	std::vector<Option> buyOptions = getBuyOptions(&gameState, gameState.playerStates[playerIndex].hand);

	int cardToReturn = 0;

	std::vector<Option>::iterator iter;

	int highestCost = 0;

	for (iter = buyOptions.begin(); iter != buyOptions.end(); iter++)
	{
		if (cardManager.cardLookupByIndex[(*iter).card].cost > highestCost || highestCost == 0 && cardManager.cardLookupByIndex[(*iter).card].name != "Curse")
		{
			highestCost = cardManager.cardLookupByIndex[(*iter).card].cost;
			cardToReturn = (*iter).card;
		}
	}

	return cardToReturn;
}