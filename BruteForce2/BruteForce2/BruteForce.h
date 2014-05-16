#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <ctime>


#include "Node.h"

class Node;

class BruteForce
{
public:
	Node* rootPtr;
	/*std::vector<Node*> emptyNodePtrs;
	std::vector<Node*> usedNodePtrs;*/
	Node* nodeAllocationPtr;
	int handedOutNodes;
	int maxNodes;
	
	Node* requestNewNodePtr();
	void initialize(int nodes);
	void createTree(int treeDepth);
	//void serializeTree();
	void printTree(int treeDepth);
	void printSmallTreeAccordingToInput();
	//void loadTree(std::string fileName);
	//Node* getNodeByID(int id);

private:
	std::vector<Node*> findLeaves();
	void printNode(Node* nodePtr, std::ofstream& file);
};