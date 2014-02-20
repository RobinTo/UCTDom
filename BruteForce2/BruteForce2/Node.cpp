#include <iostream>
#include <string>
#include <array>
#include <fstream>

#include "Node.h"


Node::Node(BruteForce* bfPtr2, int id2)
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
	id = id2;
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

		// Check for shuffle
		int cardCounter = 0;
		for (int cardIndex = 0; cardIndex < 6; cardIndex++)
		{
			cardCounter += state.deck[cardIndex];
		}
		std::array<int, 6> guaranteedCards = { 0, 0, 0, 0, 0, 0 };
		if (cardCounter < 5)
		{
			for (int cardIndex = 0; cardIndex < 6; cardIndex++)
			{
				guaranteedCards[cardIndex] = state.deck[cardIndex];
				state.deck[cardIndex] = state.discard[cardIndex];
				state.discard[cardIndex] = 0;
			}
		}

		// Append each card in deck to string
		std::string s = "";
		for (int cardIndex = 0; cardIndex < 6; cardIndex++)
		{
			for (int j = 0; j < state.deck[cardIndex]; j++)
			{
				s.append(std::to_string(static_cast<long long>(cardIndex)));
			}
		}

		// If <= than 5 in cardCounter, then draw fewer cards.
		std::size_t k = 5;
		if (cardCounter < 5)
			k = 5 - cardCounter;
	
		// While there are still new combinations of chars in string, create a new draw.
		std::vector<std::array<int, 6>> draws;
		do
		{
			std::array<int, 6> draw = { 0, 0, 0, 0, 0, 0 };
			// First, add the guaranteedCards
			for (int cardIndex = 0; cardIndex < 6; cardIndex++)
			{
				draw[cardIndex] += guaranteedCards[cardIndex];
			}

			// This is the combinationString, containing a combination of cards.
			std::string combinationString = std::string(s.begin(), s.begin() + k); 

			// For each letter in the combinationString
			for (int stringIndex = 0; stringIndex < k; stringIndex++)
			{
				// Convert letter to int, and add to draw
				int cardNumber = atoi(std::string(combinationString.begin() + stringIndex, combinationString.begin() + stringIndex + 1).c_str());
				draw[cardNumber]++;
			}
			
			// Finally, add draw to collecction of draws
			draws.push_back(draw);
		} while (next_combination(s.begin(), s.begin() + k, s.end()));

		// For each draw, create child node
		for (std::vector<std::array<int, 6>>::iterator iterator = draws.begin(); iterator != draws.end(); ++iterator)
		{
			Node* newNodePtr = bfPtr->requestNewNodePtr();
			newNodePtr->isBuy = false;

			for (int cardIndex = 0; cardIndex < 6; cardIndex++)
			{
				newNodePtr->state.hand[cardIndex] = (*iterator)[cardIndex];
				newNodePtr->state.deck[cardIndex] = state.deck[cardIndex] - (*iterator)[cardIndex];
				newNodePtr->state.discard[cardIndex] = state.discard[cardIndex];
				newNodePtr->state.supplyPiles[cardIndex] = state.supplyPiles[cardIndex];
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


void Node::printSelf(int treeDepth)
{
	// For each child, print "counter self -> child"
	std::ofstream file;
	file.open(std::to_string(treeDepth) + "printFile.gv", std::ios::app);
	std::string text = "";

	for (std::vector<Node*>::iterator iterator = children.begin(); iterator != children.end(); ++iterator)
	{
		// Append *tchu tchu*
		text += "\"";

		// Append id
		text += std::to_string(id);

		// Append self
		if (isBuy)
		{
			if (boughtCard == 0)
				text += "Copper";
			else if (boughtCard == 1)
				text += "Silver";
			else if (boughtCard == 2)
				text += "Gold";
			else if (boughtCard == 3)
				text += "Estate";
			else if (boughtCard == 4)
				text += "Duchy";
			else if (boughtCard == 5)
				text += "Province";
			else if (boughtCard == 6)
				text += "Nothing";
		}
		else
		{
			text += "C:" + std::to_string(state.hand[0]) +
				", E:" + std::to_string(state.hand[3]) +
				", S:" + std::to_string(state.hand[1]) +
				", D:" + std::to_string(state.hand[4]) +
				", G:" + std::to_string(state.hand[2]) +
				", P:" + std::to_string(state.hand[5]);
		}

		// Append *tchu tchu*
		text += "\"";

		// Append the arrow
		text += " -> ";

		// Append *tchu tchu*
		text += "\"";

		// Append child id
		text += std::to_string((*iterator)->id);

		// Append child text
		if ((*iterator)->isBuy)
		{
			if ((*iterator)->boughtCard == 0)
				text += "Copper";
			else if ((*iterator)->boughtCard == 1)
				text += "Silver";
			else if ((*iterator)->boughtCard == 2)
				text += "Gold";
			else if ((*iterator)->boughtCard == 3)
				text += "Estate";
			else if ((*iterator)->boughtCard == 4)
				text += "Duchy";
			else if ((*iterator)->boughtCard == 5)
				text += "Province";
			else if ((*iterator)->boughtCard == 6)
				text += "Nothing";
		}
		else
		{
			text += "C:" + std::to_string((*iterator)->state.hand[0]) +
				", E:" + std::to_string((*iterator)->state.hand[3]) +
				", S:" + std::to_string((*iterator)->state.hand[1]) +
				", D:" + std::to_string((*iterator)->state.hand[4]) +
				", G:" + std::to_string((*iterator)->state.hand[2]) +
				", P:" + std::to_string((*iterator)->state.hand[5]);
		}

		// Append *tchu tchu*
		text += "\";";
		
		text += "\r\n";
	}
	

	file << text;
	file.close();

}


