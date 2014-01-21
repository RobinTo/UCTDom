#include "UCTMonteCarlo.h"
#include <list>
#include <math.h>
#include <stdlib.h>

UCTMonteCarlo::UCTMonteCarlo()
{
	treeNode newNode;
	tree t(newNode);
}

void UCTMonteCarlo::rollout()
{
	for (int i = 0; i < 500; i++)
	{
		treeNode n = selectBestLeaf(t);

		//Random playthrough

		propagate();
	}
}

treeNode UCTMonteCarlo::selectBestLeaf(tree t)
{
	treeNode initialNode = t.initial;
	std::list<treeNode> childs = initialNode.childNodes;
	
	std::list<treeNode>::const_iterator iterator;
	double bestValue = 0.0;
	double thisValue = 0.0;
	treeNode bestNode;

	for (iterator = childs.begin(); iterator != childs.end(); ++iterator) {
		thisValue = (*iterator).value + 1 * sqrt(log( (double)((*iterator).parentNode)->visited) / (double)((*iterator).visited));
		if (thisValue > bestValue || bestValue == 0.0)
		{
			bestValue = thisValue;
			bestNode = (*iterator);
		}
	}
	return bestNode;

}

void UCTMonteCarlo::propagate()
{

}