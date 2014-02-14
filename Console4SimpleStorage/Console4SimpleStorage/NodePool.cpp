#include "NodePool.h"

void NodePool::allocateNodes(int allocatedNodes)
{
	nodePtrs.reserve(allocatedNodes);
	for (int counter = 0; counter < allocatedNodes; counter++)
	{
		Node* nodePtr = new Node();
		nodePtrs.push_back(nodePtr);
	}
}

void NodePool::resetNodes()
{
	for (std::vector<Node*>::iterator iterator = nodePtrs.begin(); iterator != nodePtrs.end(); ++iterator)
	{
		(*iterator)->reset();
	}
}

std::vector<Node*> NodePool::getRange(int first, int last)
{
	std::vector<Node*>::const_iterator firstIterator = nodePtrs.begin() + first;
	std::vector<Node*>::const_iterator lastIterator = nodePtrs.begin() + last;

	return std::vector<Node*>(firstIterator, lastIterator);
}