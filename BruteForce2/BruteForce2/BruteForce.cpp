#include <iostream>
#include <sstream>
#include <ctime>
#include <fstream>

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
}

void BruteForce::createTree(int turns)
{
	//Create root
	rootPtr = new Node(this, 0);
	rootPtr->isBuy = false;
	rootPtr->isRoot = true;

	//rootPtr->state.hand[0] = 4;
	//rootPtr->state.hand[3] = 1;

	rootPtr->state.deck[0] = 7;
	rootPtr->state.deck[3] = 3;

	rootPtr->state.supplyPiles[0] = 53;
	rootPtr->state.supplyPiles[1] = 40;
	rootPtr->state.supplyPiles[2] = 30;
	rootPtr->state.supplyPiles[3] = 21;
	rootPtr->state.supplyPiles[4] = 12;
	rootPtr->state.supplyPiles[5] = 12;

	usedNodePtrs.push_back(rootPtr);

	int depth = turns*2 - 1;
	for (int counter = 0; counter < depth; counter++)
	{
		std::vector<Node*> leafPtrs = findLeaves();
		
		for (std::vector<Node*>::iterator iterator = leafPtrs.begin(); iterator != leafPtrs.end(); ++iterator)
		{
			(*iterator)->doYourThing();
		}
	}
	std::cout << "Calculating score." << std::endl;
	rootPtr->calculateScore();
	std::cout << "Done calculating score." << std::endl;

	remove("tree.txt");
	std::ofstream file;
	file.open("tree.txt");
	for (std::vector<Node*>::iterator it = usedNodePtrs.begin(); it != usedNodePtrs.end(); ++it)
	{
		file << (*it)->serialize() << std::endl;
	}
	file.close();
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

void BruteForce::printSmallTreeAccordingToInput()
{
	std::cout << "Please enter your draw according to the following format:" << std::endl;
	std::cout << "Copper,silver,gold,estate,duchy,province. For instance: '2,2,0,1,0,0', without the quotes." << std::endl;

	// Receive draw input and convert to stringstream
	std::string cardInput = "";
	std::cin >> cardInput;
	std::stringstream cardInputStream(cardInput);
	
	// Split the stringstream on char ','
	std::string segment;
	std::vector<std::string> seglist;
	while (std::getline(cardInputStream, segment, ','))
	{
		seglist.push_back(segment);
	}

	// Remove all children from currentroot, except the one with the draw matched in vector

	// Receive buy input

	// Remove children from currentroot, except the one matched from buy input

	// Repeat until currentroot has no children



}


void BruteForce::loadTree(std::string fileName)
{
	std::ifstream file;
	std::string temp = "";

	file.open(fileName);

	while (!file.eof())
	{
		temp = "";
		file >> temp;
		if (temp == "")
			break;
		Node* nodePtr = requestNewNodePtr();
		nodePtr->deserialize(temp);
	}
	file.close();

	for (int i = 0; i < usedNodePtrs.size(); i++)
	{
		for (int c = 0; c < usedNodePtrs.at(i)->tempIDvector.size(); c++)
		{
			usedNodePtrs.at(i)->children.push_back(getNodeByID(usedNodePtrs.at(i)->tempIDvector.at(c)));
		}
	}
}

Node* BruteForce::getNodeByID(int id)
{
	for (int i = 0; i < usedNodePtrs.size(); i++)
	{
		if (usedNodePtrs.at(i)->id == id)
			return usedNodePtrs.at(i);
	}
	return NULL;
}