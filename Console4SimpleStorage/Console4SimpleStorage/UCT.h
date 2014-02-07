#pragma once

#include <vector>

#include "GameState.h"
#include "Node.h"

class UCT
{
private:
	std::vector<Node*> emptyNodes;
	std::vector<Node*> usedNodes;

	Node* selectBestLeaf();
	void rollout(Node* startNode);
	void propagate(Node* startNode);
	Node* requestNewNode();

public:
	UCT();
	void getNextOption(GameState currentState, int stateIndex);
};