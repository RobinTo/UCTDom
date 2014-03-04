#pragma once

#include <vector>
#include <random>
#include <math.h>
#include <iostream>
#include <fstream>
#include <list>
#include <string>

#include "Node.h"

class UCT
{
private:
	std::vector<Node*> emptyNodes;
	std::vector<Node*> usedNodes;
	int simulations;

	Node* selectBestLeaf(Node* rootNode);
	void selectAndExpand(Node* startNode, GameState currentState, int stateIndex);
	void propagate(Node* endNode, int result);
	Node* requestNewNode();

	std::vector<Option> getBuyOptions(GameState* gameState, const int (&hand)[INSUPPLY]);
	std::vector<Option> getActionOptions(GameState* gameState, const int (&hand)[INSUPPLY]);

	int rollout(int playerIndex, GameState gameState);
	void buyCard(PlayerState& pState, int cardToBuy, GameState& gameState);
	int playoutPolicy(GameState& gameState, int playerIndex);
	void resetNodes();
	void printNode(Node* nodePtr, std::ofstream& file); //Recursive print function

	CardManager cardManager;
public:
	UCT();
	~UCT();
	void initialize(CardManager& cardManager, int simulations2);
	Option getNextOption(GameState currentState, int stateIndex);
	void setNodes(std::vector<Node*>& emptyNodePtrs);
	void printTree(int turnCounter, Node* rootNodePtr);
};