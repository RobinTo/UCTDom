#pragma once

#include <algorithm>
#include <vector>
#include <fstream>
#include <iostream>
#include "Node.h"
#include "Move.h"
#include "CardManager.h"
//#include "Option.h"


class UCTMonteCarlo
{
public:
	UCTMonteCarlo();
	~UCTMonteCarlo();
	Option doUCT(int maxSimulations, int UCTPlayer, GameState gameState, std::vector<Move> moveHistory);
private:
	Node* nodeAllocationPtr;
	int handedOutNodes;
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
	std::vector<Option> getBuyOptions(GameState* gameState, int money);
	std::vector<Option> getActionOptions(GameState* gameState, const int(&hand)[INSUPPLY]);
	Option getCardPlayoutPolicy(GameState& gameState, int playerIndex);
	void playActionCard(GameState &gameState, int card, int playerIndex, bool rollout);
	void createDrawNodes(Node* parentNode, GameState& currentState, int currentlyPlaying, int numberOfCards, bool createThiefDraws);
	void createTrashNodes(Node* parentNode, GameState& currentState, int currentlyPlaying);
	int getCurrentMoney(GameState* gameState, int playerIndex);

	// Tree printing
	void printNode(Node* nodePtr, std::ofstream& file);
	void printTree(int turnCounter, int player, Node* rootNodePtr);

	// Node allocation
	std::vector<Node*> emptyNodes;
	std::vector<Node*> usedNodes;
	void resetNodes();
	Node* requestNewNode();
	unsigned long long choose(unsigned long long n, unsigned long long k);

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
};