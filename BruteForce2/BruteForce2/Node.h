#pragma once

#include "BruteForce.h"
#include "State.h"

class BruteForce;

class Node
{
public:
	bool isRoot;
	bool isBuy;
	int boughtCard;
	State state;
	std::vector<Node*> children;

	Node(BruteForce* bfPtr2);
	void doYourThing();
	void printSelf(int currentDepth);

private:
	BruteForce* bfPtr;
};