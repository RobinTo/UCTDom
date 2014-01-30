#pragma once

#include <unordered_map>
#include <vector>

#include "SupplyPile.h"
#include "Card.h"
#include "NodePool.h"



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
private:
	std::vector<Card*> findAllBuys();
	std::vector<std::vector<Card*>> findAllDraws();
	void createBuyNode(Card* buy);
	void createDrawNode(std::vector<Card*> draw);
};


