#include "treeNode.h"
#include "card.h"
#include "gameState.h"

#ifndef __UCTMONTECARLO_H__
#define __UCTMONTECARLO_H__

class UCTMonteCarlo
{
public:
	treeNode* rootNodePtr;
	UCTMonteCarlo();
	void initialize(gameState* gameStatePtr);

	card* next(bool* donePtr, gameState* gameStatePtr);
private:
	card* rollout(gameState* gameStatePtr);
	double simulate(treeNode* node, gameState gameState);
	void propagate(double heuristicValue, treeNode* node);
	treeNode* selectBestLeaf(treeNode* node);
	treeNode* bestResult();
	treeNode* createTreeNode(bool state, bool isRoot, card* card, int cash, treeNode* parentPtr);
};

#endif