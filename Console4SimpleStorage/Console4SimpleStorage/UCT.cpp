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

std::list<Option> UCT::getPossibleOptions()
{
	if (GameState.CardManager.cardLookup[id].cost <= currentMoney)
	{

	}
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
		// Select best leaf
		Node* bestLeafPtr = selectBestLeaf();
		
		// Find options
		bestLeafPtr->currentState.playerStates[stateIndex].hand;

		// Create children for each option

		// For each child
			// Add to tree
			// Rollout
			// Propagate
		/*for (each child)
		{
			rollout();
			propagate();
		}*/
		
		
		
		


		simulationCounter ++;
	}
}

Node* UCT::selectBestLeaf()
{
	Node* h = new Node();
	return h;
}

void UCT::rollout(Node* startNode)
{

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