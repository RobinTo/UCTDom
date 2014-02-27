#pragma once

#include <vector>
#include <string>

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
	void createTree(int treeDepth);
	void printTree(int treeDepth);
	void printSmallTreeAccordingToInput();
	void loadTree(std::string fileName);
	Node* getNodeByID(int id);

private:
	std::vector<Node*> findLeaves();
};