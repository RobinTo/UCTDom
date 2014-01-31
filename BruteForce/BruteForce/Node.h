#pragma once

#include <unordered_map>
#include <vector>

#include "SupplyPile.h"
#include "Card.h"
#include "NodePool.h"
#include "Draw.h"


class NodePool;

class Node
{
public:
	bool isRoot;
	bool isBuy;
	NodePool* nodePoolPtr;
	std::unordered_map<std::string, SupplyPile> supplyPiles; 
	std::vector<Card*> deck;
	std::vector<Card*> discard;
	std::vector<Card*> hand;
	std::vector<Node*> children;
	Card* boughtCard;

	Node();
	Node(NodePool* nodePoolPtr2);
	void doYourThing();
	void printSelf(int currentDepth);
private:
	std::vector<Card*> findAllBuys();
	std::vector<Draw> findAllDraws();
	std::vector<Card*> findUniqueCards(std::vector<Card*> pile);
	std::vector<Draw> removeDuplicates(std::vector<Draw> draws);
	Node* createBuyNode(Card* buy);
	Node* createDrawNode(Draw draw);

};


