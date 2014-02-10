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
	void rollout(Node* startNode, GameState currentState, int stateIndex);
	void propagate(Node* endNode);
	Node* requestNewNode();

	std::list<Option> getBuyOptions(GameState* gameState, int hand[]);
	std::list<Option> getActionOptions(GameState* gameState, int hand[]);

	int UCT::Simulate(GameState gameState, int playerIndex, Node node, int turns, int maxTurns);

public:
	UCT();
	Option getNextOption(GameState currentState, int stateIndex);
};