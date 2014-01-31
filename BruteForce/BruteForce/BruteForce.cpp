#include <unordered_map>
#include <iostream>

#include "BruteForce.h"
#include "SupplyPile.h"


void BruteForce::initialize()
{
	// Node pool
	nodePoolPtr = new NodePool(500000);
	std::cout << "Done allocating nodes" <<std::endl;

	// Root node
	rootPtr = nodePoolPtr->requestNewNodePtr();
	rootPtr->isRoot = true;
	rootPtr->isBuy = false;
	rootPtr->nodePoolPtr = nodePoolPtr;

	// Cards
	Card* cardEstatePtr = new Card("Estate", 2, 1, 0);
	Card* cardDuchyPtr = new Card("Duchy", 5, 3, 0);
	Card* cardProvincePtr = new Card("Province", 8, 6, 0);
	Card* cardCopperPtr = new Card("Copper", 0, 0, 1);
	Card* cardSilverPtr = new Card("Silver", 3, 0, 2);
	Card* cardGoldPtr = new Card("Gold", 6, 0, 3);

	// Initial supply piles
	rootPtr->supplyPiles["Estate"] = (SupplyPile(cardEstatePtr, 24));		//Normally 12 (8 if 2 players), excluding those in players hands.
	rootPtr->supplyPiles["Duchy"] = (SupplyPile(cardDuchyPtr, 12));			//Normally 12 (8 if 2 players), excluding those in players hands.
	rootPtr->supplyPiles["Province"] = (SupplyPile(cardProvincePtr, 12));	//Normally 12 (8 if 2 players), excluding those in players hands.
	rootPtr->supplyPiles["Copper"] = (SupplyPile(cardCopperPtr, 60));		//Normally 60 - those in player's hands.
	rootPtr->supplyPiles["Silver"] = (SupplyPile(cardSilverPtr, 40));		//Normally 40
	rootPtr->supplyPiles["Gold"] = (SupplyPile(cardGoldPtr, 30));			//Normally 30

	// Starting hand
	rootPtr->hand.push_back(rootPtr->supplyPiles["Estate"].cardPtr);
	rootPtr->hand.push_back(rootPtr->supplyPiles["Estate"].cardPtr);
	rootPtr->hand.push_back(rootPtr->supplyPiles["Copper"].cardPtr);
	rootPtr->hand.push_back(rootPtr->supplyPiles["Copper"].cardPtr);
	rootPtr->hand.push_back(rootPtr->supplyPiles["Copper"].cardPtr);

	// Starting deck
	rootPtr->deck.push_back(rootPtr->supplyPiles["Estate"].cardPtr);
	rootPtr->deck.push_back(rootPtr->supplyPiles["Copper"].cardPtr);
	rootPtr->deck.push_back(rootPtr->supplyPiles["Copper"].cardPtr);
	rootPtr->deck.push_back(rootPtr->supplyPiles["Copper"].cardPtr);
	rootPtr->deck.push_back(rootPtr->supplyPiles["Copper"].cardPtr);

	rootPtr->supplyPiles["Estate"].cardsLeft -= 3;
	rootPtr->supplyPiles["Copper"].cardsLeft -= 7;
}

void BruteForce::createTree(int turns)
{
	int depth = turns*2 - 1;
	for (int counter = 0; counter < depth; counter++)
	{
		std::vector<Node*> leafPtrs = findLeaves();
		
		for (std::vector<Node*>::iterator iterator = leafPtrs.begin(); iterator != leafPtrs.end(); ++iterator)
		{
			(*iterator)->doYourThing();
		}
	}
}

std::vector<Node*> BruteForce::findLeaves()
{
	std::vector<Node*> leafPtrs;
	for (std::vector<Node*>::iterator iterator = nodePoolPtr->usedNodePtrs.begin(); iterator != nodePoolPtr->usedNodePtrs.end(); ++iterator)
	{
		if ((*iterator)->children.size() <= 0)
			leafPtrs.push_back(*iterator);
	}

	return leafPtrs;
}

void BruteForce::printTree()
{
	rootPtr->printSelf(1);
}