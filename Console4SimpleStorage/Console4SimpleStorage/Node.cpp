#include "Node.h"

Node::Node()
{
	reset();
}

Node::~Node()
{
	
}

void Node::reset()
{
	childrenPtrs.clear();
	std::vector<Node*>().swap(childrenPtrs);
	currentState.~currentState();

	isRoot = false;
	value = 0;
	visited = 0;
	sum = 0;
	probability = 0;
	parentPtr = nullptr;
	playerPlaying = -1;
	opt.absoluteCardId = -1;
	opt.type = -1;

	flags = NOFLAG;
}

void Node::setOption(Option o)
{
	opt = o;
}

std::string Node::getName(){
	return CardManager::cardLookup[opt.absoluteCardId].name;
}

void Node::printSelf(std::ofstream &file)
{
	if (false)
	{

		// For each child, print "counter self -> child"
		std::string text = "";

		for (std::vector<Node*>::iterator iterator = childrenPtrs.begin(); iterator != childrenPtrs.end(); ++iterator)
		{
			/*if ((*iterator)->visited == 0)
				continue;*/

			// Append *tchu tchu*
			text += "\"";

			// Append id
			text += std::to_string(id);

			// Append self
			//text += " Type:" + std::to_string(opt.type);
			//text += CardManager::cardLookup[opt.absoluteCardId].name;

			if (opt.type == END_TURN)
			{
				text += " End Turn";
			}
			else if (opt.type == BUY)
			{
				text += " Buy " + CardManager::cardLookup[opt.absoluteCardId].name;
			}
			else if (opt.type == ACTION)
			{
				text += " Play " + CardManager::cardLookup[opt.absoluteCardId].name;
			}
			else if (opt.type == DRAW)
			{
				for (int index = 0; index < INSUPPLY; index++)
				{
					int cardsOfType = currentState.playerStates[playerPlaying].hand[index];
					if (cardsOfType > 0)
					{
						text += " " + CardManager::cardLookupByIndex[index].name + ":" + std::to_string(cardsOfType);
					}
				}
			}

			// Append visited
			//text += " Vis:" + std::to_string(visited);

			// Append value
			//text += " Val:" + std::to_string(value);

			// Append probability
			//text += " Prob:" + std::to_string(probability);

			// Append currentplayer
			//text += " Player:" + std::to_string(playerPlaying);

			// Append turnCounter
			//text += " Turn:" + std::to_string(currentState.turnCounter);

			// Append *tchu tchu*
			text += "\"";

			// Append the arrow
			text += " -> ";

			// Append *tchu tchu*
			text += "\"";

			// Append child id
			text += std::to_string((*iterator)->id);

			// Append child text
			//text += " Type:" + std::to_string((*iterator)->opt.type);
			//text += CardManager::cardLookup[(*iterator)->opt.absoluteCardId].name;

			if ((*iterator)->opt.type == END_TURN)
			{
				text += " End Turn";
			}
			else if ((*iterator)->opt.type == BUY)
			{
				text += " Buy " + CardManager::cardLookup[(*iterator)->opt.absoluteCardId].name;
			}
			else if ((*iterator)->opt.type == ACTION)
			{
				text += " Play " + CardManager::cardLookup[(*iterator)->opt.absoluteCardId].name;
			}
			else if ((*iterator)->opt.type == DRAW)
			{
				for (int index = 0; index < INSUPPLY; index++)
				{
					int cardsOfType = (*iterator)->currentState.playerStates[(*iterator)->playerPlaying].hand[index];
					if (cardsOfType > 0)
					{
						text += " " + CardManager::cardLookupByIndex[index].name + ":" + std::to_string(cardsOfType);
					}
				}
			}

			// Append visited
			//text += " Vis:" + std::to_string((*iterator)->visited);

			// Append value
			//text += " Val:" + std::to_string((*iterator)->value);

			// Append probability
			//text += " Prob:" + std::to_string((*iterator)->probability);

			// Append currentplayer
			//text += " Player:" + std::to_string((*iterator)->playerPlaying);

			// Append turnCounter
			//text += " Turn:" + std::to_string((*iterator)->currentState.turnCounter);

			// Append *tchu tchu*
			text += "\";";

			text += "\r\n";
		}

		file << text;
	}
	else
	{
		std::string text = "";


		// Append "Start"
		if (isRoot)
		{
			text += std::to_string(id) + "[label=\"";
			text += "";
			text += "\"];";
			text += std::to_string(id) + "[color = lightblue2]";
			text += std::to_string(id) + "[shape = point]";

			// Add support for showing a start node with draw
		}
		else
		{

			// Append self
			if (opt.type == END_TURN)
			{
				text += std::to_string(id) + "[label=\"";

				text += "End Turn and Draw: ";
				for (int index = 0; index < INSUPPLY; index++)
				{
					int cardsOfType = currentState.playerStates[playerPlaying].hand[index];
					if (cardsOfType > 0)
					{
						text += CardManager::cardLookupByIndex[index].name + ":" + std::to_string(cardsOfType);
					}
				}

				text += "\"];";
				text += std::to_string(id) + "[color = red]";
				text += std::to_string(id) + "[shape = box]";
			}
			else if (opt.type == BUY)
			{
				text += std::to_string(id) + "[label=\"";

				text += "Buy " + CardManager::cardLookup[opt.absoluteCardId].name;

				text += "\"];";
				text += std::to_string(id) + "[color = blue]";
				text += std::to_string(id) + "[shape = ellipse]";
			}
			else if (opt.type == ACTION)
			{
				text += std::to_string(id) + "[label=\"";

				text += "Play " + CardManager::cardLookup[opt.absoluteCardId].name;

				text += "\"];";
				text += std::to_string(id) + "[color = lightblue2]";
				text += std::to_string(id) + "[shape = octagon]";

				// Can add support for showing which cards are drawn, if + x Card was played
			}
		}

		text += "\r\n";

		for (std::vector<Node*>::iterator iterator = childrenPtrs.begin(); iterator != childrenPtrs.end(); ++iterator)
		{
			text += std::to_string(id) + " -> " + std::to_string((*iterator)->id);
			text += ";";
			text += "\r\n";
		}

		file << text;
	}
}







