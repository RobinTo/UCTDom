#include <iostream>
#include <ctime>
#include <fstream>
#include <string>

#include "BruteForce.h"


Node* BruteForce::requestNewNodePtr()
{
	if (usedNodePtrs.size() == 0)
		std::cout << "No more nodes" << std::endl;

	Node* nodePtr = emptyNodePtrs.back();
	usedNodePtrs.push_back(nodePtr);
	emptyNodePtrs.pop_back();


	return nodePtr;
}


void BruteForce::initialize(int nodes)
{
	emptyNodePtrs.reserve(nodes);
	usedNodePtrs.reserve(nodes);

	//Allocate nodes
	for (int counter = 1; counter < nodes; counter ++)
	{
		emptyNodePtrs.push_back(new Node(this, nodes - counter));
	}

	//Create root
	rootPtr = new Node(this,0);
	rootPtr->isBuy = false;
	rootPtr->isRoot = true;

	rootPtr->state.hand[0] = 4;
	rootPtr->state.hand[3] = 1;

	rootPtr->state.deck[0] = 3;
	rootPtr->state.deck[3] = 2;
	
	rootPtr->state.supplyPiles[0] = 53;
	rootPtr->state.supplyPiles[1] = 40;
	rootPtr->state.supplyPiles[2] = 30;
	rootPtr->state.supplyPiles[3] = 21;
	rootPtr->state.supplyPiles[4] = 12;
	rootPtr->state.supplyPiles[5] = 12;

	usedNodePtrs.push_back(rootPtr);
}

void BruteForce::createTree(int turns)
{
	int depth = turns*2 - 1;
	for (int counter = 0; counter < depth; counter++)
	{
		std::vector<Node*> leafPtrs = findLeaves();
		
		for (std::vector<Node*>::iterator iterator = leafPtrs.begin(); iterator != leafPtrs.end(); ++iterator)
		{
			(*iterator)->doYourThing();
		}
	}
}

std::vector<Node*> BruteForce::findLeaves()
{
	std::vector<Node*> leafPtrs;
	for (std::vector<Node*>::iterator iterator = usedNodePtrs.begin(); iterator != usedNodePtrs.end(); ++iterator)
	{
		if ((*iterator)->children.size() == 0)
			leafPtrs.push_back(*iterator);
	}
	return leafPtrs;
}

void BruteForce::printTree(int treeDepth)
{
	std::string fileName = std::to_string(treeDepth) + "printFile.gv";
	remove(fileName.c_str());

	std::ofstream file;
	file.open(std::to_string(treeDepth) + "printFile.gv", std::ios::app);
	std::string text = "digraph unix{\r\n size = \"10000000000!, 1000000000\";\r\n ratio = \"expand\";\r\n node[color = lightblue2, style = filled];";
	
	file << text << std::endl;
	file.close();
	for (std::vector<Node*>::iterator it = usedNodePtrs.begin(); it != usedNodePtrs.end(); ++it)
	{
		(*it)->printSelf(treeDepth);
	}

	file.open(std::to_string(treeDepth) + "printFile.gv", std::ios::app);
	text = "\r\n }";

	file << text << std::endl;
	file.close();
}