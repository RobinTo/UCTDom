
#include "Node.h"

#define SEPARATOR ":"

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
	score = 0;
}

Node::Node()
{
	for (int index = 0; index < 6; index++)
	{
		state.deck[index] = 0;
		state.discard[index] = 0;
		state.hand[index] = 0;
		state.supplyPiles[index] = 0;
	}
		
	boughtCard = -1;
	isRoot = false;
	score = 0;
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

		State copyState = state;

		// Check for shuffle
		int cardCounter = 0;
		for (int cardIndex = 0; cardIndex < 6; cardIndex++)
		{
			cardCounter += copyState.deck[cardIndex];
		}
		std::array<int, 6> guaranteedCards = { 0, 0, 0, 0, 0, 0 };
		if (cardCounter < 5)
		{
			for (int cardIndex = 0; cardIndex < 6; cardIndex++)
			{
				guaranteedCards[cardIndex] = copyState.deck[cardIndex];
				copyState.deck[cardIndex] = copyState.discard[cardIndex];
				copyState.discard[cardIndex] = 0;
			}
		}

		// Append each card in deck to string
		std::string s = "";
		for (int cardIndex = 0; cardIndex < 6; cardIndex++)
		{
			for (int j = 0; j < copyState.deck[cardIndex]; j++)
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
				newNodePtr->state.deck[cardIndex] = copyState.deck[cardIndex] - (*iterator)[cardIndex] + guaranteedCards[cardIndex];
				newNodePtr->state.discard[cardIndex] = copyState.discard[cardIndex];
				newNodePtr->state.supplyPiles[cardIndex] = copyState.supplyPiles[cardIndex];
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


void Node::printSelf(std::ofstream& file)
{
	if (false)
	{

		std::string text = "";
		text += std::to_string(id) + "[label=\"";

		// Append "Start"
		if (id == 0)
			text += "Start";

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
			for (int index = 0; index < 6; index++)
			{
				std::string cardName;
				switch (index)
				{
				case 0: cardName = "Copper";
					break;
				case 1: cardName = "Silver";
					break;
				case 2: cardName = "Gold";
					break;
				case 3: cardName = "Estate";
					break;
				case 4: cardName = "Duchy";
					break;
				case 5: cardName = "Province";
					break;
				default: cardName = "Error";
					std::cout << "Error printing cardname" << std::endl;
					break;
				}

				int cardsOfType = state.hand[index];
				if (cardsOfType > 0)
				{
					text += cardName + ":" + std::to_string(cardsOfType) + " ";
				}
			}
		}
		text += "\"];";

		text += "\r\n";

		for (std::vector<Node*>::iterator iterator = children.begin(); iterator != children.end(); ++iterator)
		{
			text += std::to_string(id) + " -> " + std::to_string((*iterator)->id);
			text += ";";
			text += "\r\n";
		}

		file << text;
	}
	else
	{
		std::string text = "";
		

		// Append "Start"
		if (id == 0)
		{
			text += std::to_string(id) + "[label=\"";
			text += "";
			text += "\"];";
			text += std::to_string(id) + "[color = lightblue2]";
			text += std::to_string(id) + "[shape = point]";
		}
		else
		{

			// Append self
			if (isBuy)
			{
				text += std::to_string(id) + "[label=\"";
				text += "";
				text += "\"];";
				text += std::to_string(id) + "[color = blue]";
				text += std::to_string(id) + "[shape = ellipse]";
			}
			else
			{
				text += std::to_string(id) + "[label=\"";
				text += "";
				text += "\"];";
				text += std::to_string(id) + "[color = red]";
				text += std::to_string(id) + "[shape = box]";
			}
		}

		text += "\r\n";

		for (std::vector<Node*>::iterator iterator = children.begin(); iterator != children.end(); ++iterator)
		{
			text += std::to_string(id) + " -> " + std::to_string((*iterator)->id);
			text += ";";
			text += "\r\n";
		}

		file << text;
	}
	
}


bool Node::deserialize(std::string serialized)
{
	std::vector<std::string> stringParts = split(serialized, ':');
	
	id = std::atoi(stringParts.at(0).c_str());
	isRoot = std::atoi(stringParts.at(1).c_str());
	isBuy = std::atoi(stringParts.at(2).c_str());
	boughtCard = std::atoi(stringParts.at(3).c_str());

	std::vector<std::string> handValues = split(stringParts.at(4), '-');
	for (int i = 0; i < 6; i++)
	{
		state.hand[i] = std::atoi(handValues.at(i).c_str());
	}

	std::vector<std::string> childIDs = split(stringParts.at(5), '-');
	for (int i = 0; i < childIDs.size(); i++)
	{
		tempIDvector.push_back(std::atoi(childIDs.at(i).c_str()));
	}

	score = std::atof(stringParts.at(6).c_str());

	return true;
}

std::vector<std::string> Node::split(std::string s, char delim)
{
	std::stringstream test(s);
	std::string segment;
	std::vector<std::string> seglist;

	while (std::getline(test, segment, delim))
	{
		seglist.push_back(segment);
	}

	return seglist;
}

std::string Node::serialize()
{
	std::string toReturn = "";
	toReturn.append(std::to_string(id));
	toReturn.append(SEPARATOR);
	toReturn.append(std::to_string(isRoot));
	toReturn.append(SEPARATOR);
	toReturn.append(std::to_string(isBuy));
	toReturn.append(SEPARATOR);
	toReturn.append(std::to_string(boughtCard));
	toReturn.append(SEPARATOR);
	for (int i = 0; i < 6; i++) // Hand size is 6
	{
		toReturn.append(std::to_string(state.hand[i]));
		toReturn.append("-");
	}
	// Append state hand
	toReturn.append(SEPARATOR);
	for (int i = 0; i < children.size(); i++)
	{
		toReturn.append(std::to_string(children.at(i)->id));
		toReturn.append("-");
	}
	toReturn.append(SEPARATOR);
	toReturn.append(std::to_string(score));

	return toReturn;
}

void Node::calculateScore()
{
	/*
	root.calculatevp()

	calculatevp():
		if children.size() > 0:
			for each child:
				child.calculatevp()
				self.vp += child.vp
			self.vp = self.vp/children.size()
		else:
			self.vp = hand[4] * 2 + hand[5] * 5 + hand[6] * 8 + deck...
	*/

	if (children.size() > 0)
	{
		for (std::vector<Node*>::iterator iter = children.begin(); iter != children.end(); ++iter)
		{
			(*iter)->calculateScore();
			score += (*iter)->score;
		}
		score = score / children.size();
	}
	else
	{
		score += state.deck[3] * 1;
		score += state.deck[4] * 3;
		score += state.deck[5] * 6;
		score += state.discard[3] * 1;
		score += state.discard[4] * 3;
		score += state.discard[5] * 6;
		score += state.hand[3] * 1;
		score += state.hand[4] * 3;
		score += state.hand[5] * 6;
	}
}
