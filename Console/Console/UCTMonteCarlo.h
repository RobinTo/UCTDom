#include "treeNode.h"
#include "game.h"

#ifndef __UCTMONTECARLO_H__
#define __UCTMONTECARLO_H__

class UCTMonteCarlo
{
public:
	treeNode* rootNodePtr;
	UCTMonteCarlo(treeNode* rootNodePtr);
	void rollout(game g);
	void select();
	double simulate(treeNode* node, game g);
	void propagate(double heuristicValue, treeNode* node);
	treeNode* selectBestLeaf(treeNode* node);
	treeNode* bestResult();
};

#endif