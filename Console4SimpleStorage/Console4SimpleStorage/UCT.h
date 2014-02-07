#pragma once

#include <vector>

#include "GameState.h"
#include "Node.h"
#include "Option.h"
#include <list>

class UCT
{
private:
	std::vector<Node*> emptyNodes;
	std::vector<Node*> usedNodes;

	Node* selectBestLeaf();
	void rollout(Node* startNode);
	void propagate(Node* startNode);
	Node* requestNewNode();

	std::list<Option> getPossibleOptions();

public:
	UCT();
	Option getNextOption(GameState currentState, int stateIndex);
};