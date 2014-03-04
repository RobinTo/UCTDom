#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include "Option.h"
#include "Node.h"

class UCTMonteCarlo
{
public:
	UCTMonteCarlo();
	~UCTMonteCarlo();
	CardManager cardManager;
	Option doUCT(int maxSimulations, int playerTurn, GameState gameState);
private:
	// MC UCT
	Node* select(Node* root);
	void expand(Node* node, int UCTPlayer);
	void rollout(Node* node, GameState gameState, int UCTPlayer);
	void propagate(Node* node, double score);
	Node* UCTSelectChild(Node* root);
	void createAllChildren(Node* node);
	Node* getRandomNode(std::vector<Node*> nodes);
	std::vector<Node*> getUntriedChildren(Node* parent);

	// Dominion-specific
	std::vector<Option> getBuyOptions(GameState* gameState, const int(&hand)[INSUPPLY]);
	std::vector<Option> getActionOptions();
	int getCardPlayoutPolicy(GameState& gameState, int playerIndex);
	
	// Tree printing
	void printNode(Node* nodePtr, std::ofstream& file);
	void printTree(int turnCounter, Node* rootNodePtr);

	// Node allocation
	std::vector<Node*> emptyNodes;
	std::vector<Node*> usedNodes;
	void resetNodes();
	Node* requestNewNode();
};