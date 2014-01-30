#pragma once


#include <vector>

#include "NodePool.h"
#include "Node.h"






class BruteForce
{
public:
	Node* rootPtr;
	NodePool* nodePoolPtr;

	void initialize();
	void createTree(int turns);

private:
	std::vector<Node*> findLeaves();
};
