#include <iostream>
#include "player.h"
#include "card.h"
#include "treeNode.h"
#include <vector>
#include "card.h"
#include <string>
#include <stdlib.h>
#include <time.h>

player p;
	
std::vector<card> cards;
std::vector<int> cardCounters;

int turns = 0;

void addCard(std::string name, int value, int cost, int amount)
{
	card c;

	c.name = name;
	c.value = value;
	c.cost = cost;

	cards.push_back(c);
	cardCounters.push_back(amount);
}

card* getCardByName(std::string name)
{
	for (int i = 0; i < cards.size(); i++)
	{
		if (name == cards.at(i).name)
			return &cards.at(i);
	}
}

card* getNextOption(int c, int turns)
{
	if (c >= 8)
		return getCardByName("Province");
	else if (c >= 6 && turns < 30)
		return getCardByName("Gold");
	else if (c >= 5)
		return getCardByName("Duchy");
	else if (c>=3 && turns < 30)
		return getCardByName("Silver");
	else if (c>=2)
		return getCardByName("Estate");
	else
		return getCardByName("Copper");
}


std::vector<card*> getOptions(int c)
{
	std::vector<card*> returnCards;

	for (int i = 0; i < cards.size(); i++)
	{
		if(c >= cards.at(i).cost)
			returnCards.push_back(&cards.at(i));
	}

	return returnCards;
}

treeNode* bestChildNode(std::vector<treeNode*> childs)
{
	int bestValue = 0;
	treeNode* bestNode;
	bool notSet = true;

	for (int i = 0; i < childs.size(); i++)
	{
		int nodeValue = -childs.at(i)->timesChosen;
		if(nodeValue > bestValue || notSet)
		{
			bestValue = nodeValue;
			bestNode = childs.at(i);
			notSet = false;
		}
	}

	return bestNode;
}

treeNode* getBestLeaf(treeNode* node, player* pCopy)
{
	node->timesChosen++;
	if (!node->isTop)
	{
		pCopy->buyCard(node->cardOption);
		pCopy->endTurn();
	}
	if (node->childNodes.size() <= 0)
		return node;
	else
		return getBestLeaf(bestChildNode(node->childNodes), pCopy);
}

void propagate(int value, treeNode* propagateNode)
{
	propagateNode->totalValue += value;
	propagateNode->timesPropagated += 1.0;
	propagateNode->value = propagateNode->totalValue/propagateNode->timesPropagated;
	if (!propagateNode->isTop)
		propagate(value, propagateNode->parent);
}


int simulate(treeNode* newNode, player newPCopy)
{
	newPCopy.buyCard(newNode->cardOption);
	newPCopy.endTurn();
	for(int i = turns; i < 40-newNode->treeDepth; i++)
	{
		std::vector<card*> newOptions = getOptions(newPCopy.moneyInHand());
		int randCard = rand() % newOptions.size();
		newPCopy.buyCard(newOptions.at(randCard));
		newPCopy.endTurn();
	}

	int endSum = 0;

	for (int i = 0; i < newPCopy.hand.size(); i++)
		endSum += newPCopy.hand.at(i)->value;
	for (int i = 0; i < newPCopy.deck.size(); i++)
		endSum += newPCopy.deck.at(i)->value;
	for (int i = 0; i < newPCopy.discard.size(); i++)
		endSum += newPCopy.discard.at(i)->value;
	for (int i = 0; i < newPCopy.inPlay.size(); i++)
		endSum += newPCopy.inPlay.at(i)->value;

	return endSum;
}

void rollout(treeNode* node, player pCopy)
{
	treeNode* nodeToExpand = getBestLeaf(node, &pCopy);
	
	if(nodeToExpand->treeDepth < 40-turns)
	{
		std::vector<card*> possibilities = getOptions(pCopy.moneyInHand());
		for (int i = 0; i < possibilities.size(); i++)
		{
			treeNode* newNode = nodeToExpand->appendChild(possibilities.at(i), nodeToExpand);
			int value = simulate(newNode, pCopy);			// New copy of player, don't want to include simulation buys in next possibility simulation.
			propagate(value, nodeToExpand);
		}
	}
	else
	{
		int value = simulate(nodeToExpand, pCopy);
		propagate(value, nodeToExpand);
	}
}


card* getNextOptionUCT(player pCopy)
{
	std::vector<card*> options = getOptions(pCopy.moneyInHand());
	treeNode topNode;
	topNode.isTop = true;
		
	for (int i = 0; i < options.size()*1000; i++)
	{
		rollout(&topNode, pCopy);
	}

	double bestValue = 0.0;
	treeNode* bestNode;
	bool isSet = false;
	for (int i = 0; i < topNode.childNodes.size(); i++)
	{
		std::cout << topNode.childNodes.at(i)->timesChosen << std::endl;
		if (topNode.childNodes.at(i)->value > bestValue || !isSet)
		{
			bestValue = topNode.childNodes.at(i)->value;
			bestNode = topNode.childNodes.at(i);
			isSet = true;
		}
	}
	return bestNode->cardOption;
}


int main()
{
	srand(time(NULL));
	addCard("Copper", 0, 0, 12);
	addCard("Silver", 0, 3, 12);
	addCard("Gold", 0, 6, 12);
	addCard("Estate", 1, 2, 12);
	addCard("Duchy", 3, 5, 12);
	addCard("Province", 6, 8, 12);

	p.discard.push_back(getCardByName("Copper"));
	p.discard.push_back(getCardByName("Copper"));
	p.discard.push_back(getCardByName("Copper"));
	p.discard.push_back(getCardByName("Copper"));
	p.discard.push_back(getCardByName("Copper"));
	p.discard.push_back(getCardByName("Copper"));
	p.discard.push_back(getCardByName("Copper"));

	p.discard.push_back(getCardByName("Estate"));
	p.discard.push_back(getCardByName("Estate"));
	p.discard.push_back(getCardByName("Estate"));

	p.shuffle();
	p.endTurn();

	while (turns < 40)
	{
		int m = p.moneyInHand();
		std::cout << "With " << m << " coins buy:" << std::endl;
		card* nextCard = getNextOptionUCT(p);
		std::cout << nextCard->name << std::endl;
		p.buyCard(nextCard);
		p.endTurn();
		turns++;
	}

	std::cout << "Ended game with: " << p.victoryPoints() << " points." << std::endl;

	std::cout << "Works!" << std::endl;

	int t;
	std::cin >> t;
	return 0;
}






