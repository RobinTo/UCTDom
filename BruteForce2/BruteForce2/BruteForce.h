#pragma once

#include <vector>

#include "Node.h"

class Node;

class BruteForce
{
public:
	Node* rootPtr;
	std::vector<Node*> emptyNodePtrs;
	std::vector<Node*> usedNodePtrs;
	
	Node* requestNewNodePtr();
	void initialize(int nodes);
	void createTree(int turns);
	void printTree();

private:
	std::vector<Node*> findLeaves();
};