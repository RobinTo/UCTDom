#include "UCTMonteCarlo.h"
#include <list>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "treasureCard.h"

UCTMonteCarlo::UCTMonteCarlo()
{
	rootNodePtr = createTreeNode(true, true, NULL, 5, NULL);
}

void UCTMonteCarlo::initialize(gameState* gameStatePtr)
{
	createTreeNode(false, false, &gameStatePtr->cardstacks[0].cardType, 0, rootNodePtr);
	createTreeNode(false, false, &gameStatePtr->cardstacks[1].cardType, 0, rootNodePtr);
	createTreeNode(false, false, &gameStatePtr->cardstacks[3].cardType, 0, rootNodePtr);
	createTreeNode(false, false, &gameStatePtr->cardstacks[4].cardType, 0, rootNodePtr);
}

card* UCTMonteCarlo::next(bool* donePtr, gameState* gameStatePtr)
{
	card* bestCardPtr = rollout(gameStatePtr);
	*donePtr = true;

	return bestCardPtr;
}

card* UCTMonteCarlo::rollout(gameState* gameStatePtr)
{
	for (int i = 0; i < 20; i++)
	{
		treeNode* bestLeafPtr = selectBestLeaf(rootNodePtr);

		// Dynamically find how much money the player has.
		int currentMoney = 5;
		
		std::list<card*> options = gameStatePtr->getOptions(currentMoney);
		std::list<card*>::const_iterator iterator;
		for (iterator = options.begin(); iterator != options.end(); ++iterator)
		{
			//Add child
			treeNode* newNode = new treeNode();
			newNode->state = false;
			newNode->isRoot = false;
			newNode->c = (*iterator);
			newNode->value = 1;
			newNode->visited = 0;
			newNode->parentNodePtr = bestLeafPtr;
			bestLeafPtr->appendChild(newNode);
			
			//Simulate and Propagate
			double heuristicValue = simulate(newNode, *gameStatePtr);
			propagate(heuristicValue, newNode);
		}
	}
	
	// Find best card to buy
	treeNode* bestNodePtr = rootNodePtr->childNodePtrs.front();
	std::list<treeNode*>::const_iterator iterator;
	for (iterator = rootNodePtr->childNodePtrs.begin(); iterator != rootNodePtr->childNodePtrs.end(); ++iterator)
	{
		if ((*iterator)->value > bestNodePtr->value)
			bestNodePtr = *iterator;
	}
	return bestNodePtr->c;
}

double UCTMonteCarlo::simulate(treeNode* node, gameState gameState2)
{
	//gameState gameStateCopy;
	//gameStateCopy.cardstacks[0] = gameState2.cardstacks[0];
	//gameStateCopy.cardstacks[1] = gameState2.cardstacks[1];
	//gameStateCopy.cardstacks[2] = gameState2.cardstacks[2];
	//gameStateCopy.cardstacks[3] = gameState2.cardstacks[3];
	//gameStateCopy.cardstacks[4] = gameState2.cardstacks[4];
	//gameStateCopy.cardstacks[5] = gameState2.cardstacks[5];
	//playerState playerStateCopy;
	//playerStateCopy.deck = gameState2.playerStatePtrs.front()->deck;
	//playerStateCopy.discard = gameState2.playerStatePtrs.front()->discard;
	//gameStateCopy.playerStatePtrs.push_back(&playerStateCopy);

	gameState gameStateCopy = gameState(gameState2);

	double heuristicScore = 0;
	gameStateCopy.playerStatePtrs.front()->receiveCard(node->c);
	heuristicScore += node->c->cost;
	gameStateCopy.playerStatePtrs.front()->endTurn();
	
	for (int counter = 0; counter < 2; counter++)
	{
		//Dynamically find current money available
		int nextMoney = 5;
		std::list<card*> options = gameStateCopy.getOptions(nextMoney);
		
		int randomValue = rand() % options.size();
		for (int index = 0; index < randomValue; index++)
		{
			options.pop_front();
		}
		gameStateCopy.playerStatePtrs.front()->receiveCard(options.front());
		heuristicScore += node->c->cost;
		gameStateCopy.playerStatePtrs.front()->endTurn();
	}
	std::cout << "Done Simulate" << std::endl;

	return heuristicScore;
}

void UCTMonteCarlo::propagate(double heuristicValue, treeNode* nodePtr)
{
	nodePtr->value = heuristicValue;
	treeNode* tempNodePtr = nodePtr->parentNodePtr;
	while (!tempNodePtr->isRoot)
	{
		std::cout << "Starting propagate" << std::endl;
		double childrenScore = 0.0;
		std::list<treeNode*>::const_iterator iterator;
		for (iterator = tempNodePtr->childNodePtrs.begin(); iterator != tempNodePtr->childNodePtrs.end(); ++iterator)
		{
			childrenScore += (*iterator)->value;
			std::cout << "Child score: " << (*iterator)->value << std::endl;
		}
		
		tempNodePtr->value = childrenScore/tempNodePtr->childNodePtrs.size();
		std::cout << "Parent score: " << tempNodePtr->value << std::endl;
		tempNodePtr = tempNodePtr->parentNodePtr;
	}
}

treeNode* UCTMonteCarlo::selectBestLeaf(treeNode* startNodePtr)
{
	std::list<treeNode*>::const_iterator iterator;
	double bestValue = 0.0;
	double thisValue = 0.0;
	treeNode* bestNodePtr;

	for (iterator = startNodePtr->childNodePtrs.begin(); iterator != startNodePtr->childNodePtrs.end(); ++iterator) {
		thisValue = (*iterator)->value + 1 * sqrt(log( (double)((*iterator)->parentNodePtr)->visited) / (double)((*iterator)->visited));
		if (thisValue > bestValue || bestValue == 0.0)
		{
			bestValue = thisValue;
			bestNodePtr = (*iterator);
		}
	}

	if(bestNodePtr->childNodePtrs.size() > 0)
	{
		return selectBestLeaf(bestNodePtr);
	}
	else
	{
		return bestNodePtr;
	}
}

treeNode* UCTMonteCarlo::createTreeNode(bool state, bool isRoot, card* card, int cash, treeNode* parentPtr)
{
	treeNode* t = new treeNode(isRoot, parentPtr);
	t->state = state;
	t->c = card;
	t->value = 1;
	t->visited = 0;
	t->cash = cash;
	

	if (parentPtr != NULL)
		parentPtr->appendChild(t);
	return t;
}

