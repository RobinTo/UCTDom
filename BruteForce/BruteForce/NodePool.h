#pragma once

#include <vector>
#include "Node.h"

class Node;

class NodePool
{
public:
	std::vector<Node*> emptyNodePtrs;
	std::vector<Node*> usedNodePtrs;
	
	Node* requestNewNodePtr();
	NodePool(int nodes);
};


