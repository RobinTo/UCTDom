#include "treeNode.h"
#include "tree.h"

#ifndef __UCTMONTECARLO_H__
#define __UCTMONTECARLO_H__

class UCTMonteCarlo
{
public:
	UCTMonteCarlo();
	void rollout();
	tree t;
	void select();
	void propagate();
	treeNode selectBestLeaf(tree t);

	treeNode* bestResult();
};

#endif