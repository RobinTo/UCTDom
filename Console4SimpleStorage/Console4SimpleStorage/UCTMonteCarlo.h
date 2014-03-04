#pragma once

#include <vector>
#include "Option.h"
#include "Node.h"

class UCTMonteCarlo
{
public:
	CardManager cardManager;
	Option doUCT(int maxSimulations, int playerTurn, GameState gameState);
private:
	Node* select(Node* root);
	void expand(Node* node, int UCTPlayer);
	void rollout(Node* node, GameState gameState, int UCTPlayer);
	void propagate(Node* node, double score);
	Node* UCTSelectChild(Node* root);
	void createAllChildren(Node* node);
	Node* getRandomNode(std::vector<Node*> nodes);
	std::vector<Node*> getUntriedChilds(Node* parent);
	std::vector<Option> getBuyOptions(GameState* gameState, const int(&hand)[INSUPPLY]);
	std::vector<Option> getActionOptions();
	int getCardPlayoutPolicy(GameState& gameState, int playerIndex);

};