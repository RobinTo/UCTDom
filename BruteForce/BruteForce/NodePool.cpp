

#include "NodePool.h"


NodePool::NodePool(int nodes)
{
	for (int counter = 0; counter < nodes; counter ++)
	{
		emptyNodePtrs.push_back(new Node(this));
	}
}

Node* NodePool::requestNewNodePtr()
{
	Node* toReturn = emptyNodePtrs.back();
	usedNodePtrs.push_back(toReturn);
	emptyNodePtrs.pop_back();
	return toReturn;
}