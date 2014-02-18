#include <iostream>

#include "UCT2.h"

void UCT2::initialize(CardManager& cm, int simulations2, std::vector<Node*>& emptyNodePtrs)
{
	simulations = simulations2;
	cardManager = cm;
	emptyNodes = emptyNodePtrs;
	usedNodes.reserve(emptyNodePtrs.size());
}

void UCT2::resetNodes()
{
	for (std::vector<Node*>::iterator iterator = usedNodes.begin(); iterator != usedNodes.end(); ++iterator)
	{
		(*iterator)->reset();
		emptyNodes.push_back(*iterator);
	}
	usedNodes.clear();
}

Node* UCT2::requestNewNode()
{
	if (emptyNodes.size() <= 0)
		std::cout << "No more nodes!" << std::endl;

	Node* returnNode = emptyNodes.back();
	usedNodes.push_back(returnNode);
	emptyNodes.pop_back();
	return returnNode;
}

Option UCT2::getNextOption(GameState rootState, int maximumIterations)
{
	// Check for end turn
	if (rootState.playerStates[0].buys == 0)
	{
		Option option;
		option.type = END_TURN;
		return option;
	}

	Node* rootNodePtr = requestNewNode();
	rootNodePtr->currentState = rootState;
	rootNodePtr->findMoves(cardManager);

	for (int i = 0; i < maximumIterations; i++)
	{
		Node* nodePtr = requestNewNode();
		nodePtr = rootNodePtr;
		GameState state = rootState;

		// Select
		while (nodePtr->untriedMoves.empty() && !nodePtr->childrenPtrs.empty()) // Node is fully expanded and non-terminal
		{
			nodePtr = UCTSelectChild(nodePtr);
			doMove(state, nodePtr->opt);
		}

		// Expand
		if (!nodePtr->untriedMoves.empty())										// If we can expand, (i.e state/node is non-terminal).
		{
			int randomIndex = rand() % nodePtr->untriedMoves.size();
			Option option = nodePtr->untriedMoves[randomIndex];
			doMove(state, option);
			Node* newNodePtr = requestNewNode();								// node.addchild and descend down the tree
			newNodePtr->currentState = state;
			newNodePtr->findMoves(cardManager);
			newNodePtr->opt = option;
			newNodePtr->parentPtr = nodePtr;
			nodePtr->childrenPtrs.push_back(newNodePtr);
			nodePtr->untriedMoves.erase(nodePtr->untriedMoves.begin() + randomIndex);
			nodePtr = newNodePtr;
		}

		// Rollout
		while (!getBuyOptions(state).empty() && state.turnCounter <= 40) //While state is non-terminal
		{
			std::vector<Option> options = getBuyOptions(state);
			doMove(state, options[rand() % options.size()]);
		}
		
		// Backpropagate
		while (nodePtr)
		{
			nodePtr->sum += state.playerStates[0].calculateVictoryPoints(cardManager);
			nodePtr->propagateCounter++;
			nodePtr = nodePtr->parentPtr;
		}
	}

	// Return node with the highest propagateCounter
	Option o;
	double highestCount = 0;

	for (int i = 0; i<rootNodePtr->childrenPtrs.size(); i++)
	{
		std::cout << "Score for " << cardManager.cardLookupByIndex[rootNodePtr->childrenPtrs.at(i)->opt.card].name << " was: " << rootNodePtr->childrenPtrs.at(i)->sum / rootNodePtr->childrenPtrs.at(i)->propagateCounter << std::endl;
		std::cout << "Propagated : " << rootNodePtr->childrenPtrs.at(i)->propagateCounter << std::endl;
		if (rootNodePtr->childrenPtrs[i]->propagateCounter > highestCount || highestCount == 0)
		{
			o = rootNodePtr->childrenPtrs[i]->opt;
			highestCount = rootNodePtr->childrenPtrs[i]->propagateCounter;
		}
	}
	resetNodes();
	return o;
}


Node* UCT2::UCTSelectChild(Node* rootNode)
{
	double bestValue = 0;
	Node* bestNode = rootNode;

	for (int i = 0; i < rootNode->childrenPtrs.size(); i++)
	{
		double thisValue = 
			(double)rootNode->childrenPtrs[i]->sum / (double)rootNode->childrenPtrs[i]->propagateCounter +
			sqrt(2 * log((double)rootNode->propagateCounter) / rootNode->childrenPtrs[i]->propagateCounter);

		if (thisValue > bestValue || bestValue == 0)
		{
			bestValue = thisValue;
			bestNode = rootNode->childrenPtrs[i];
		}		
	}
		
	return bestNode;
}

std::vector<Option> UCT2::getBuyOptions(const GameState& gameState)
{
	int currentMoney = 0;
	currentMoney += gameState.playerStates[0].hand[cardManager.cardIndexer[COPPER]];
	currentMoney += gameState.playerStates[0].hand[cardManager.cardIndexer[SILVER]] * 2;
	currentMoney += gameState.playerStates[0].hand[cardManager.cardIndexer[GOLD]] * 3;

	std::vector<Option> options;
	for (int i = 0; i < sizeof(gameState.playerStates[0].hand) / sizeof(*gameState.playerStates[0].hand); i++)
	{
		if (cardManager.cardLookupByIndex[i].cost <= currentMoney && gameState.supplyPiles[i] > 0)
		{
			Option o;
			o.type = BUY;
			o.card = i;
			options.push_back(o);
		}
	}

	return options;
}

void UCT2::doMove(GameState& gameState, Option option)
{
	gameState.playerStates[0].buyCard(cardManager, option.card);
	gameState.supplyPiles[cardManager.cardIndexer[option.card]] -= 1;
	gameState.turnCounter++;
}