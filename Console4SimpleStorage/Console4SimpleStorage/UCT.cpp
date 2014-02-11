#include <iostream>

#include "UCT.h"
#include <math.h>

UCT::UCT()
{
	int allocatedNodes = 50000;
	emptyNodes.reserve(allocatedNodes);
	usedNodes.reserve(allocatedNodes);
	for (int counter = 0; counter < allocatedNodes; counter++)
	{
		Node* nodePtr = new Node();
		emptyNodes.push_back(nodePtr);
	}
}

void UCT::setCardManager(CardManager& cm)
{
	cardManager = cm;
}

std::list<Option> UCT::getActionOptions(GameState* gameState, int hand[])
{
	// Return all action cards in hand
	std::list<Option> options;
	return options;
}

std::list<Option> UCT::getBuyOptions(GameState* gameState, int hand[])
{
	int currentMoney = 0;
	currentMoney += hand[cardManager.cardIndexer[COPPER]];
	currentMoney += hand[cardManager.cardIndexer[SILVER]] * 2;
	currentMoney += hand[cardManager.cardIndexer[GOLD]] * 3;
	
	std::list<Option> options;
	for(int i = 0; i < sizeof(hand)/sizeof(*hand); i++)
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
	//gameState->cardManager.cardIndexer[COPPER]; returns int index in hand etc of COPPPER
	//gameState->cardManager.cardLookup[COPPER]; returns card struct
	//gameState->cardManager.cardLookupByIndex[1];	returns card struct

	// Return all cards in stockpile with cost <= currentMoney
}

Option UCT::getNextOption(GameState currentState, int stateIndex)
{
	//Create rootNode
	Node* rootNode = requestNewNode();
	rootNode->isRoot = true;
	rootNode->currentState = currentState;

	int simulations = 50;

	// While more simulations
	int simulationCounter = 0;
	while(simulationCounter < simulations)
	{
		
		rollout(rootNode, currentState, stateIndex);

		simulationCounter ++;
	}

	Option o;
	double highestScore = 0;

	for (int i = 0; i<rootNode->childrenPtrs.size(); i++)
	{
		if (rootNode->childrenPtrs.at(i)->value > highestScore || highestScore == 0)
		{
			o = rootNode->childrenPtrs.at(i)->opt;
			highestScore = rootNode->childrenPtrs.at(i)->value;
		}
	}
	
	return o;
}

Node* UCT::selectBestLeaf(Node* rootNode)
{
	double bestValue = 0;
	Node* bestNode = rootNode;

	for (int i = 0; i < rootNode->childrenPtrs.size(); i++)
	{
		if (!rootNode->childrenPtrs.at(i)->isRoot)
		{
			double thisValue = rootNode->childrenPtrs.at(i)->value * sqrt(log((double)rootNode->propagateCounter/rootNode->childrenPtrs.at(i)->propagateCounter));
			if(thisValue > bestValue || bestValue == 0)
			{
				bestValue = thisValue;
				bestNode = rootNode->childrenPtrs.at(i);
			}
		}
	}

	if (bestNode->childrenPtrs.size() <= 0)
		return bestNode;
	else
		return selectBestLeaf(bestNode);
}

void UCT::rollout(Node* startNode, GameState currentState, int stateIndex)
{
	// Select best leaf
	Node* bestLeafPtr = selectBestLeaf(startNode);


	int treeDepth = 0;	// Placeholder ints
	int turns = 0;		// TODO: Pass as parameters
	int maxTurns = 40;
	if(treeDepth < maxTurns-turns)	// If treeDepth is less than turns left.
	{
		std::list<Option> actionOptions = getActionOptions(&currentState, bestLeafPtr->currentState.playerStates[stateIndex].hand);
		std::list<Option> buyOptions = getBuyOptions(&currentState, bestLeafPtr->currentState.playerStates[stateIndex].hand);

		// TODO: for each action

		std::list<Option>::iterator iter;

		for(iter=actionOptions.begin(); iter != actionOptions.end(); iter++)
		{
			if(currentState.playerStates[stateIndex].actions > 0)
			{
				Node* newNode = requestNewNode();
				newNode->parentPtr = bestLeafPtr;
				newNode->setState(currentState);	// Copy, not reference.
			
				// playAction(&newNode->currentState, actionIterator);
				// simulate(newNode->currentState, stateIndex, newNode, turns, maxTurns);
			}
		}
		
		for(iter=buyOptions.begin(); iter != buyOptions.end(); iter++)
		{
			if(currentState.playerStates[stateIndex].buys > 0)
			{
				Node* newNode = requestNewNode();
				newNode->parentPtr = bestLeafPtr;
				newNode->setState(currentState);
				newNode->setOption(*iter);

				buyCard(newNode->currentState.playerStates[stateIndex], (*iter).card);
				int result = simulate(stateIndex, newNode->currentState, turns, maxTurns);
				propagate(newNode, result);
			}
		}
	}
}

void buyCard(PlayerState& pState, int cardToBuy)
{
	pState.hand[cardToBuy] += 1;
}

int UCT::simulate(int playerIndex, GameState gameState, int turn, int maxTurns)
{
	gameState.playerStates[playerIndex].endTurn();
	
	for (int i = 0; i<maxTurns-turn; i++)
	{
		int cardChosen = playoutPolicy(gameState, playerIndex);
		buyCard(gameState.playerStates[playerIndex], cardChosen);
		gameState.playerStates[playerIndex].endTurn();
	}

	return gameState.playerStates[playerIndex].calculateVictoryPoints();
}

// Currently greedy
int UCT::playoutPolicy(GameState& gameState, int playerIndex)
{
	std::list<Option> buyOptions = getBuyOptions(&gameState, gameState.playerStates[playerIndex].hand);
	std::list<Option>::iterator iter;

	int highestCost = 0;
	int cardToReturn = 0;

	for(iter=buyOptions.begin(); iter != buyOptions.end(); iter++)
	{
		if (cardManager.cardLookupByIndex[(*iter).card].cost > highestCost || highestCost = 0)
		{
			highestCost = cardManager.cardLookupByIndex[(*iter)].card.cost;
			cardToReturn = (*iter).card;
		}
	}

	return cardToReturn;
}

void UCT::propagate(Node* endNode, int result)
{
	endNode->sum += result;
	endNode->propagateCounter += 1;
	endNode->value = (endNode->sum/endNode->propagateCounter);
	if(!endNode->isRoot)
	{
		propagate(endNode->parentPtr, result);
	}
}

Node* UCT::requestNewNode()
{
	if (usedNodes.size() <= 0)
		std::cout << "No more nodes!" << std::endl;
	
	Node* returnNode = emptyNodes.back();
	usedNodes.push_back(returnNode);
	emptyNodes.pop_back();
	return returnNode;
}