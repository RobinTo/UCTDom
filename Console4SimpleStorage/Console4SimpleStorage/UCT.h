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
	void nextPlayerTurn(int& playerIndex, GameState& gameState);

	int rollout(int playerIndex, GameState gameState);
	void buyCard(PlayerState& pState, int cardToBuy, GameState& gameState);
	int playoutPolicy(GameState& gameState, int playerIndex);
	void resetNodes();
	void printNode(Node* nodePtr, std::ofstream& file); //Recursive print function

	CardManager cardManager;

	template <typename Iterator>
	inline bool next_combination(const Iterator first, Iterator k, const Iterator last)
	{
		/* Credits: Thomas Draper */
		if ((first == last) || (first == k) || (last == k))
			return false;
		Iterator itr1 = first;
		Iterator itr2 = last;
		++itr1;
		if (last == itr1)
			return false;
		itr1 = last;
		--itr1;
		itr1 = k;
		--itr2;
		while (first != itr1)
		{
			if (*--itr1 < *itr2)
			{
				Iterator j = k;
				while (!(*itr1 < *j)) ++j;
				std::iter_swap(itr1, j);
				++itr1;
				++j;
				itr2 = k;
				std::rotate(itr1, j, last);
				while (last != j)
				{
					++j;
					++itr2;
				}
				std::rotate(k, itr2, last);
				return true;
			}
		}
		std::rotate(first, k, last);
		return false;
	}

public:
	UCT();
	~UCT();
	void initialize(CardManager& cardManager, int simulations2);
	Option getNextOption(GameState currentState, int stateIndex);
	void setNodes(std::vector<Node*>& emptyNodePtrs);
	void printTree(int turnCounter, Node* rootNodePtr);
};