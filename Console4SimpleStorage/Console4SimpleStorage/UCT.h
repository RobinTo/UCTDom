#pragma once

#include <vector>

#include "GameState.h"
#include "Node.h"
#include "Option.h"
#include <list>
#include "CardManager.h"
#include "PlayerState.h"

class UCT
{
private:
	std::vector<Node*> emptyNodes;
	std::vector<Node*> usedNodes;

	Node* selectBestLeaf(GameState* currentState);
	void rollout(Node* startNode, GameState currentState, int stateIndex);
	void propagate(Node* endNode, int result);
	Node* requestNewNode();

	std::list<Option> getBuyOptions(GameState* gameState, int hand[]);
	std::list<Option> getActionOptions(GameState* gameState, int hand[]);

	int simulate(int playerIndex, GameState gameState, int turns, int maxTurns);
	void buyCard(PlayerState& pState, int cardToBuy);
	int playoutPolicy(GameState& gameState, int playerIndex);

	CardManager cardManager;
public:
	UCT();
	void setCardManager(CardManager& cardManager);
	Option getNextOption(GameState currentState, int stateIndex);
};