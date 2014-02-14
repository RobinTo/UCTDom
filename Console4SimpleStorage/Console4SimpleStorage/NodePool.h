#pragma once

#include <vector>

#include "Node.h"
#include <list>

class NodePool
{
public:
	std::vector<Node*> nodePtrs;

	void allocateNodes(int allocatedNodes);
	void resetNodes();
	std::vector<Node*> getRange(int first, int last);
};
