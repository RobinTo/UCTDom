

#include "BruteForce.h"


Node* BruteForce::requestNewNodePtr()
{
	if (handedOutNodes >= maxNodes)
	{
		std::cout << "No more nodes!" << std::endl;
	}
	handedOutNodes++;
	Node* toReturn = &nodeAllocationPtr[handedOutNodes - 1];
	return toReturn;

	/*if (emptyNodePtrs.size() == 0)
		std::cout << "No more nodes" << std::endl;

	Node* nodePtr = emptyNodePtrs.back();
	usedNodePtrs.push_back(nodePtr);
	emptyNodePtrs.pop_back();


	return nodePtr;*/
}


void BruteForce::initialize(int nodes)
{
	
	std::cout << "Initializing, please wait." << std::endl;
	maxNodes = nodes;
	nodeAllocationPtr = new Node[nodes];
	handedOutNodes = 0;
	for (int counter = 0; counter < nodes; counter++)
	{
		nodeAllocationPtr[counter].id = counter;
		nodeAllocationPtr[counter].bfPtr = this;
	}
	


	//emptyNodePtrs.reserve(nodes);
	//usedNodePtrs.reserve(nodes);

	////Allocate nodes
	//for (int counter = 1; counter < nodes; counter ++)
	//{
	//	emptyNodePtrs.push_back(new Node(this, nodes - counter));
	//}
	std::cout << "Done initializing" << std::endl;
}

void BruteForce::createTree(int turns)
{
	std::cout << "Creating tree." << std::endl;

	//Create root
	rootPtr = requestNewNodePtr();
	rootPtr->id = 0;
	rootPtr->bfPtr = this;
	rootPtr->isBuy = true;
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

	//usedNodePtrs.push_back(rootPtr);

	int depth = turns*2;
	for (int counter = 0; counter < depth; counter++)
	{
		std::vector<Node*> leafPtrs = findLeaves();
		
		for (std::vector<Node*>::iterator iterator = leafPtrs.begin(); iterator != leafPtrs.end(); ++iterator)
		{
			(*iterator)->doYourThing();
		}
	}

	std::cout << "Done creating tree." << std::endl;

	std::cout << "Calculating score." << std::endl;
	rootPtr->calculateScore();
	std::cout << "Done calculating score." << std::endl;
}

//void BruteForce::serializeTree()
//{
//	std::cout << "Serializing nodes." << std::endl;
//	remove("tree.txt");
//	std::ofstream file;
//	file.open("tree.txt");
//	for (std::vector<Node*>::iterator it = usedNodePtrs.begin(); it != usedNodePtrs.end(); ++it)
//	{
//		file << (*it)->serialize() << std::endl;
//	}
//	file.close();
//
//	std::cout << "Done serializing nodes." << std::endl;
//}

std::vector<Node*> BruteForce::findLeaves()
{
	std::vector<Node*> leafPtrs;
	for (int counter = 0; counter < handedOutNodes; counter++)
	{
		if (nodeAllocationPtr[counter].children.size() == 0)
			leafPtrs.push_back(&nodeAllocationPtr[counter]);
	}

	/*for (std::vector<Node*>::iterator iterator = usedNodePtrs.begin(); iterator != usedNodePtrs.end(); ++iterator)
	{
		if ((*iterator)->children.size() == 0)
			leafPtrs.push_back(*iterator);
	}*/
	return leafPtrs;
}

void BruteForce::printTree(int treeDepth)
{
	std::cout << "Printing tree." << std::endl;

	std::string fileName = std::to_string(treeDepth) + "printFile.gv";
	remove(fileName.c_str());

	std::ofstream file;
	file.open(fileName, std::ios::app);
	std::string text = "digraph bruteForce{\r\n size = \"10000000000!, 1000000000\";\r\n ratio = \"expand\";\r\n node[color = lightblue2, style = filled];";
	
	file << text << std::endl;
	/*for (std::vector<Node*>::iterator it = usedNodePtrs.begin(); it != usedNodePtrs.end(); ++it)
	{
		(*it)->printSelf(file);
	}*/

	for (int counter = 0; counter < handedOutNodes; counter++)
	{
		nodeAllocationPtr[counter].printSelf(file);
	}

	text = "\r\n }";
	file << text << std::endl;
	file.close();

	std::cout << "Done printing tree." << std::endl;
}

void BruteForce::printSmallTreeAccordingToInput()
{
	std::cout << "Cutting and printing tree." << std::endl;

	Node* currentNode = rootPtr;

	while (currentNode->children.size() > 0)
	{
		// Receive draw input and convert to stringstream
		std::cout << "Please enter your draw according to the following format:" << std::endl;
		std::cout << "Copper,silver,gold,estate,duchy,province. For instance: '2,2,0,1,0,0', without the quotes." << std::endl;
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
		int matchHand[6];
		matchHand[0] = std::atoi(seglist[0].c_str());
		matchHand[1] = std::atoi(seglist[1].c_str());
		matchHand[2] = std::atoi(seglist[2].c_str());
		matchHand[3] = std::atoi(seglist[3].c_str());
		matchHand[4] = std::atoi(seglist[4].c_str());
		matchHand[5] = std::atoi(seglist[5].c_str());



		Node* nextNode = nullptr;
		// For each child in currentroot,
		for (std::vector<Node*>::iterator it = currentNode->children.begin(); it != currentNode->children.end(); ++it)
		{
			// except the one with a matching hand
			if ((*it)->state.hand[0] == matchHand[0] && (*it)->state.hand[1] == matchHand[1] && (*it)->state.hand[2] == matchHand[2] && (*it)->state.hand[3] == matchHand[3] && (*it)->state.hand[4] == matchHand[4] && (*it)->state.hand[5] == matchHand[5])
			{
				// Save next node
				nextNode = (*it);
				continue;
			}
			// remove their children
			else
			{
				(*it)->children.clear();
			}
		}

		// Move currentRoot downwards
		currentNode = nextNode;

		if (currentNode->children.size() == 0)
			break;

		// Receive buy input
		std::cout << "Please enter your buy according to the following format:" << std::endl;
		std::cout << "Copper = 0, silver = 1, gold = 2, estate = 3, duchy = 4, province = 5. For instance: '2', without the quotes." << std::endl;
		int buyInput = -1;
		std::cin >> buyInput;

		nextNode = nullptr;
		// For each child in currentroot,
		for (std::vector<Node*>::iterator it = currentNode->children.begin(); it != currentNode->children.end(); ++it)
		{
			// except the one with a matching hand
			if ((*it)->boughtCard == buyInput)
			{
				// Save next node
				nextNode = (*it);
				continue;
			}
			// remove their children
			else
			{
				(*it)->children.clear();
			}
		}
		currentNode = nextNode;

	}

	// Then print tree
	std::string fileName = "SmallTree.gv";
	remove(fileName.c_str());
	std::ofstream file;
	file.open(fileName, std::ios::app);
	std::string text = "digraph unix{\r\n size = \"10000000000!, 1000000000\";\r\n ratio = \"expand\";\r\n node[color = lightblue2, style = filled];";
	file << text << std::endl;
	printNode(rootPtr, file);
	text = "\r\n }";
	file << text << std::endl;
	file.close();

	std::cout << "Done cutting and printing tree." << std::endl;
}


void BruteForce::printNode(Node* nodePtr, std::ofstream& file)
{
	nodePtr->printSelf(file);
	for (std::vector<Node*>::iterator it = nodePtr->children.begin(); it != nodePtr->children.end(); ++it)
		printNode(*it, file);
}

//void BruteForce::loadTree(std::string fileName)
//{
//	std::cout << "Loading tree." << std::endl;
//	std::ifstream file;
//	std::string temp = "";
//
//	file.open(fileName);
//
//	while (!file.eof())
//	{
//		temp = "";
//		file >> temp;
//		if (temp == "")
//			break;
//		Node* nodePtr = requestNewNodePtr();
//		nodePtr->deserialize(temp);
//	}
//	file.close();
//
//	for (int i = 0; i < usedNodePtrs.size(); i++)
//	{
//		if (usedNodePtrs.at(i)->isRoot)
//		{
//			rootPtr = usedNodePtrs.at(i);
//		}
//		for (int c = 0; c < usedNodePtrs.at(i)->tempIDvector.size(); c++)
//		{
//			usedNodePtrs.at(i)->children.push_back(getNodeByID(usedNodePtrs.at(i)->tempIDvector.at(c)));
//		}
//	}
//	std::cout << "Done loading tree." << std::endl;
//}

//Node* BruteForce::getNodeByID(int id)
//{
//	for (int i = 0; i < usedNodePtrs.size(); i++)
//	{
//		if (usedNodePtrs.at(i)->id == id)
//			return usedNodePtrs.at(i);
//	}
//	return NULL;
//}