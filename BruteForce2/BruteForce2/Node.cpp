#include <iostream>
#include <string>

#include "Node.h"


Node::Node(BruteForce* bfPtr2)
{
	for (int index = 0; index < 6; index++)
	{
		state.deck[index] = 0;
		state.discard[index] = 0;
		state.hand[index] = 0;
		state.supplyPiles[index] = 0;
	}
	bfPtr = bfPtr2;
	boughtCard = -1;
	isRoot = false;
}

void Node::doYourThing()
{
	if (isBuy)
	{
		// Discard hand after buy
		for (int cardIndex = 0; cardIndex < 6; cardIndex++)
		{
			state.discard[cardIndex] += state.hand[cardIndex];
			state.hand[cardIndex] = 0;
		}
		// Add cards so that there are at least five cards in deck
		int cardCounter = 0;
		for (int cardIndex = 0; cardIndex < 6; cardIndex++)
		{
			cardCounter += state.deck[cardIndex];
		}
		std::vector<std::vector<int>> draws;
		if (cardCounter <= 5)
		{
			for (int cardIndex = 0; cardIndex < 6; cardIndex++)
			{
				state.hand[cardIndex] = state.deck[cardIndex];
				state.deck[cardIndex] = state.discard[cardIndex];
				state.discard[cardIndex] = 0;
			}
		}

		std::string s = "";
		for (int cardIndex = 0; cardIndex < 6; cardIndex++)
		{
			for (int cardCounter2 = 0; cardCounter2 < state.deck[cardIndex]; cardCounter2++)
			{
				s.append(std::to_string(static_cast<long long>(cardIndex)));
			}
		}

		std::size_t k = 5 - (cardCounter <= 5 ? cardCounter : 0);
		do
		{
			std::vector<int> draw;
			for (int i = 0; i < 6; i++)
			{
				if (state.hand[i] > 0)
				{
					for (int n = 0; n < state.hand[i]; n++)
						draw.push_back(i);
				}
			}
			for (int i = 0; i < (cardCounter <= 5 ? cardCounter : 0); i++)
			{
				draw.push_back(atoi(std::string(s.begin() + i, s.begin() + i + 1).c_str()));
			}
			draws.push_back(draw);
		} while (next_combination(s.begin(), s.begin() + k, s.end()));

		// For each draw, create child node
		for (std::vector<std::vector<int>>::iterator iterator = draws.begin(); iterator != draws.end(); ++iterator)
		{
			Node* newNodePtr = bfPtr->requestNewNodePtr();
			newNodePtr->isBuy = false;


			for (int cardIndex = 0; cardIndex < 6; cardIndex++)
			{
				newNodePtr->state.deck[cardIndex] = state.deck[cardIndex];
				newNodePtr->state.discard[cardIndex] = state.discard[cardIndex];
				newNodePtr->state.supplyPiles[cardIndex] = state.supplyPiles[cardIndex];
			}
			for (int i = 0; i < 5 - (cardCounter <= 5 ? cardCounter : 0); i++)
			{
				newNodePtr->state.hand[(*iterator)[i]] += 1;
				newNodePtr->state.deck[(*iterator)[i]] -= 1;
			}
			children.push_back(newNodePtr);
		}
	}
	else
	{
		// Find money available
		int money = 0;
		money += state.hand[0]*1;
		money += state.hand[1]*2;
		money += state.hand[2]*3;

		// Find available choices
		bool choices[] = {false,false,false,false,false,false,false};	// If a choice is false, then it is not an option.
		choices[0] = true;							// Can always choose to not buy
		choices[6] = true;							// Can always buy copper
		if (money >= 2)
			choices[3] = true;
		if (money >= 3)
			choices[1] = true;
		if (money >= 5)
			choices[4] = true;
		if (money >= 6)
			choices[2] = true;
		if (money >= 8)
			choices[5] = true;

		// Create a child node for each choice
		for (int choiceIndex = 0; choiceIndex < 7; choiceIndex ++)
		{
			if (choices[choiceIndex] == false)
				continue;

			Node* newNodePtr = bfPtr->requestNewNodePtr();
			newNodePtr->isBuy = true;
			for (int cardIndex = 0; cardIndex < 6; cardIndex++)
			{
				newNodePtr->state.deck[cardIndex] = state.deck[cardIndex];
				newNodePtr->state.hand[cardIndex] = state.hand[cardIndex];
				newNodePtr->state.discard[cardIndex] = state.discard[cardIndex];
				newNodePtr->state.supplyPiles[cardIndex] = state.supplyPiles[cardIndex];
			}

			newNodePtr->boughtCard = choiceIndex;
			if (newNodePtr->boughtCard != 6)										// If a card was bought, then put one in discard, and remove one from supply
			{
				newNodePtr->state.discard[newNodePtr->boughtCard] ++;
				newNodePtr->state.supplyPiles[newNodePtr->boughtCard] --;
			}
			
			children.push_back(newNodePtr);
		}
	}
}


void Node::printSelf(int currentDepth)
{
	if (isBuy)
	{
		if (boughtCard == 0)
			std::cout << "Buy:" << "Copper" << std::endl;
		else if (boughtCard == 1)
			std::cout << "Buy:" << "Silver" << std::endl;
		else if (boughtCard == 2)
			std::cout << "Buy:" << "Gold" << std::endl;
		else if (boughtCard == 3)
			std::cout << "Buy:" << "Estate" << std::endl;
		else if (boughtCard == 4)
			std::cout << "Buy:" << "Duchy" << std::endl;
		else if (boughtCard == 5)
			std::cout << "Buy:" << "Province" << std::endl;
		else if (boughtCard == 6)
			std::cout << "Buy:" << "Nothing" << std::endl;
	}
	else
	{
		std::cout << "Copper: " << state.hand[0] << ", Estate: " << state.hand[3] << ", Silver: " << state.hand[1] << ", Duchy: " << state.hand[4] << ", Gold: " << state.hand[2] << ", Province: " << state.hand[5] << std::endl;
		std::cout << "Copper: " << state.deck[0] << ", Estate: " << state.deck[3] << ", Silver: " << state.deck[1] << ", Duchy: " << state.deck[4] << ", Gold: " << state.deck[2] << ", Province: " << state.deck[5] << std::endl;
		std::cout << "Copper: " << state.discard[0] << ", Estate: " << state.discard[3] << ", Silver: " << state.discard[1] << ", Duchy: " << state.discard[4] << ", Gold: " << state.discard[2] << ", Province: " << state.discard[5] << std::endl;

	}
	
	if (children.size() > 0)
	{
		for (std::vector<Node*>::iterator iterator = children.begin(); iterator != children.end(); ++iterator)
		{
			for (int counter = 0; counter < currentDepth; counter++)
				std::cout << "->";
			(*iterator)->printSelf(currentDepth+1);
		}
	}
}


