#pragma once

#include <vector>

#include "GameState.h"
#include "Option.h"

class Node
{
public:
	GameState currentState;
	int visited;
	bool isRoot;
	double value;
	int sum;
	Option opt;
	int propagateCounter;
	std::vector<Node*> childrenPtrs;
	std::vector<Option> untriedMoves;
	Node* parentPtr;
	void setOption(Option o);
	void setState(GameState gameState);
	Node();
	~Node();
	void reset();

	void findMoves(CardManager cardManager);
};