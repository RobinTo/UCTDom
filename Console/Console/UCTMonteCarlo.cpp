#include "UCTMonteCarlo.h"
#include <list>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "treasureCard.h"

UCTMonteCarlo::UCTMonteCarlo(treeNode* rootNodePtr2)
{
	rootNodePtr = rootNodePtr2;
}

void UCTMonteCarlo::rollout(game g)
{
	game originalGame = g;
	for (int i = 0; i < 500; i++)
	{
		treeNode* bestLeafPtr = selectBestLeaf(rootNodePtr);

		int currentMoney = 5;
		std::list<card*> hand = g.players[0].hand;
		//std::list<card*>::const_iterator iterator;
		//for (iterator = hand.begin(); iterator != hand.end(); ++iterator)
		//{
		//	if (((*iterator))->name == "Copper" || ((*iterator))->name == "Silver" || ((*iterator))->name == "Gold")
		//	{
				//treasureCard tc = *iterator;

				//card* c_ptr = *iterator;
				//card c = *c_ptr;
				//treasureCard tc = dynamic_cast<treasureCard>(c);



				//card* c_ptr = &cat;
				//treasureCard* t_ptr = dynamic_cast<treasureCard*>(c_ptr); 
		//	}
			
		//}
		std::list<card*> options = g.getOptions(currentMoney);
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
			
			//Simulate
			double heuristicValue = simulate(newNode, g);
			propagate(heuristicValue, newNode);
			g = originalGame;
		}
	}
}

double UCTMonteCarlo::simulate(treeNode* node, game g)
{
	double heuristicScore = 0;
	g.players[0].receiveCard(node->c);
	heuristicScore += node->c->cost;
	g.players[0].endTurn();

	int counter = 0;
	while (counter < 2)
	{
		int nextMoney = 5;
		std::list<card*> options = g.getOptions(nextMoney);
		int randomValue = rand() % options.size();
		for (int index = 0; index < randomValue; index++)
		{
			options.pop_front();
		}
		g.players[0].receiveCard(options.front());
		heuristicScore += node->c->cost;
		g.players[0].endTurn();

		counter++;
	}
	std::cout << "Done Simulate" << std::endl;
	return heuristicScore;
}

void UCTMonteCarlo::propagate(double heuristicValue, treeNode* nodePtr)
{
	treeNode* tempNodePtr = nodePtr;
	while (!tempNodePtr->isRoot)
	{
		if (tempNodePtr->value < heuristicValue)
			tempNodePtr->value = heuristicValue;
		std::cout << tempNodePtr->value << std::endl;
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
