#include <iostream>

#include "UCT.h"

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

std::list<Option> UCT::getActionOptions(GameState* gameState, int hand[])
{
	// Return all action cards in hand
}

std::list<Option> UCT::getBuyOptions(GameState* gameState, int hand[], int supplyPile[])
{
	int currentMoney = 0;
	currentMoney += hand[gameState->cardManager.cardIndexer[COPPER]];
	currentMoney += hand[gameState->cardManager.cardIndexer[SILVER]] * 2;
	currentMoney += hand[gameState->cardManager.cardIndexer[GOLD]] * 3;
	
	std::list<Option> options;
	for(int i = 0; i < sizeof(hand)/sizeof(*hand); i++)
	{
		if (gameState->cardManager.cardLookupByIndex[i].cost <= currentMoney)
		{
			Option o;
			o.type = BUY;
			o.card = gameState->cardManager.cardLookupByIndex[i];
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
		
		rollout(currentState, stateIndex);


		simulationCounter ++;
	}
}

Node* UCT::selectBestLeaf()
{
	Node* h = new Node();
	return h;
}

void UCT::rollout(Node* startNode, GameState currentState, int stateIndex)
{
	// Select best leaf
	Node* bestLeafPtr = selectBestLeaf();


	int treeDepth = 0;	// Placeholder ints
	int turns = 0;		// TODO: Pass as parameters
	int maxTurns = 40;
	if(treeDepth < maxTurns-turns)	// If treeDepth is less than turns left.
	{
		getActionOptions(bestLeafPtr->currentState.playerStates[stateIndex].hand);
		getBuyOptions(bestLeafPtr->currentState.playerStates[stateIndex].hand);

		if(gameState.playerStates[stateIndex].actions > 0)
		{
			// Perhaps do actions?
		}
		if(gameState.playerStates[stateIndex].buys > 0)
		{
			// Perhaps buy something?
		}
	}
	
}

void UCT::propagate(Node* startNode)
{

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