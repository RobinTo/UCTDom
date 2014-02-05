#pragma once

#include <vector>

#include "GameState.h"

class Node
{
public:
	GameState currentState;
	int visited;
	bool isRoot;
	double value;
	std::vector<Node*> childrenPtrs;
	Node* parentPtr;

	Node();
};