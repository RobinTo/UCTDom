#pragma once

#include <vector>

#include "Node.h"
#include <list>

class UCT
{
private:
	std::vector<Node*> emptyNodes;
	std::vector<Node*> usedNodes;

	Node* selectBestLeaf(Node* rootNode);
	void rollout(Node* startNode, GameState currentState, int stateIndex);
	void propagate(Node* endNode, int result);
	Node* requestNewNode();

	std::list<Option> getBuyOptions(GameState* gameState, const int (&hand)[INSUPPLY]);
	std::list<Option> getActionOptions(GameState* gameState, const int (&hand)[INSUPPLY]);

	int simulate(int playerIndex, GameState gameState);
	void buyCard(PlayerState& pState, int cardToBuy, GameState& gameState);
	int playoutPolicy(GameState& gameState, int playerIndex);
	void resetNodes();

	CardManager cardManager;
public:
	UCT();
	~UCT();
	void setCardManager(CardManager& cardManager);
	Option getNextOption(GameState currentState, int stateIndex);
};