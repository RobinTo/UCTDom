#include "treeNode.h"
#include "tree.h"
#include "game.h"

#ifndef __UCTMONTECARLO_H__
#define __UCTMONTECARLO_H__

class UCTMonteCarlo
{
public:
	UCTMonteCarlo(game g);
	void rollout();
	tree t;
	game g;
	void select();
	void propagate();
	treeNode selectBestLeaf(tree t);

	treeNode* bestResult();
};

#endif