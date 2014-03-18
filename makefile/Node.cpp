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
	isRoot = false;
	value = 0;
	visited = 0;
	sum = 0;
	probability = 0;
	value = 0;
	//propagateCounter = 0;
	childrenPtrs.clear();
	parentPtr = nullptr;
	untriedMoves.clear();
	playerPlaying = -1;
	opt.absoluteCardId = -1;
	opt.type = -1;
}

void Node::setState(GameState state)
{
	currentState = state;
}

void Node::setOption(Option o)
{
	opt = o;
}

void Node::printSelf(std::ofstream &file) // TODO: Add support for printing each option, such as multiple action cards. (Print to multiple files)
{
	// For each child, print "counter self -> child"
	std::string text = "";

	for (std::vector<Node*>::iterator iterator = childrenPtrs.begin(); iterator != childrenPtrs.end(); ++iterator)
	{
		// Append *tchu tchu*
		text += "\"";

		// Append id
		text += std::to_string(id);

		// Append self
		if (opt.absoluteCardId == COPPER)
			text += "Copper";
		else if (opt.absoluteCardId == SILVER)
			text += "Silver";
		else if (opt.absoluteCardId == GOLD)
			text += "Gold";
		else if (opt.absoluteCardId == ESTATE)
			text += "Estate";
		else if (opt.absoluteCardId == DUCHY)
			text += "Duchy";
		else if (opt.absoluteCardId == PROVINCE)
			text += "Province";
		else if (opt.absoluteCardId == CURSE)
			text += "Curse";
		else if (opt.absoluteCardId == MARKET)
			text += "Market";
		else if (opt.absoluteCardId == VILLAGE)
			text += "Village";
		else if (opt.absoluteCardId == LABORATORY)
			text += "Laboratory";
		else if (opt.absoluteCardId == SMITHY)
			text += "Smithy";
		else if (opt.absoluteCardId == WOODCUTTER)
			text += "Woodcutter";
		else if (opt.absoluteCardId == WITCH)
			text += "Witch";
		else if (opt.absoluteCardId == MONEYLENDER)
			text += "Moneylender";
		else if (opt.absoluteCardId == FESTIVAL)
			text += "Festival";
		else if (opt.absoluteCardId == GARDENS)
			text += "Gardens";
		else if (opt.type == DRAW)
			text += "DrawCards";
		else
			text += "EndTurn";
		
		text += "C:" + std::to_string(currentState.playerStates[playerPlaying].hand[COPPER]) +
			", E:" + std::to_string(currentState.playerStates[playerPlaying].hand[ESTATE]) +
			", S:" + std::to_string(currentState.playerStates[playerPlaying].hand[SILVER]) +
			", D:" + std::to_string(currentState.playerStates[playerPlaying].hand[DUCHY]) +
			", G:" + std::to_string(currentState.playerStates[playerPlaying].hand[GOLD]) +
			", P:" + std::to_string(currentState.playerStates[playerPlaying].hand[PROVINCE]) +
			", Cur:" + std::to_string(currentState.playerStates[playerPlaying].hand[CURSE]);

		// Append visited
		text += " Vis:" + std::to_string(visited);

		// Append probability
		text += " Prob:" + std::to_string(probability);

		// Append currentplayer
		text += " Player:" + std::to_string(playerPlaying);

		// Append *tchu tchu*
		text += "\"";

		// Append the arrow
		text += " -> ";

		// Append *tchu tchu*
		text += "\"";

		// Append child id
		text += std::to_string((*iterator)->id);

		// Append child text
		if ((*iterator)->opt.absoluteCardId == COPPER)
			text += "Copper";
		else if ((*iterator)->opt.absoluteCardId == SILVER)
			text += "Silver";
		else if ((*iterator)->opt.absoluteCardId == GOLD)
			text += "Gold";
		else if ((*iterator)->opt.absoluteCardId == ESTATE)
			text += "Estate";
		else if ((*iterator)->opt.absoluteCardId == DUCHY)
			text += "Duchy";
		else if ((*iterator)->opt.absoluteCardId == PROVINCE)
			text += "Province";
		else if ((*iterator)->opt.absoluteCardId == CURSE)
			text += "Curse";
		else if ((*iterator)->opt.absoluteCardId == MARKET)
			text += "Market";
		else if ((*iterator)->opt.absoluteCardId == VILLAGE)
			text += "Village";
		else if ((*iterator)->opt.absoluteCardId == LABORATORY)
			text += "Laboratory";
		else if ((*iterator)->opt.absoluteCardId == SMITHY)
			text += "Smithy";
		else if ((*iterator)->opt.absoluteCardId == WOODCUTTER)
			text += "Woodcutter";
		else if ((*iterator)->opt.absoluteCardId == WITCH)
			text += "Witch";
		else if ((*iterator)->opt.absoluteCardId == MONEYLENDER)
			text += "Moneylender";
		else if ((*iterator)->opt.absoluteCardId == FESTIVAL)
			text += "Festival";
		else if ((*iterator)->opt.absoluteCardId == GARDENS)
			text += "Gardens";
		else if ((*iterator)->opt.type == DRAW)
			text += "DrawCards";
		else
			text += "EndTurn";
		
		text += "C:" + std::to_string((*iterator)->currentState.playerStates[(*iterator)->playerPlaying].hand[COPPER]) +
			", E:" + std::to_string((*iterator)->currentState.playerStates[(*iterator)->playerPlaying].hand[ESTATE]) +
			", S:" + std::to_string((*iterator)->currentState.playerStates[(*iterator)->playerPlaying].hand[SILVER]) +
			", D:" + std::to_string((*iterator)->currentState.playerStates[(*iterator)->playerPlaying].hand[DUCHY]) +
			", G:" + std::to_string((*iterator)->currentState.playerStates[(*iterator)->playerPlaying].hand[GOLD]) +
			", P:" + std::to_string((*iterator)->currentState.playerStates[(*iterator)->playerPlaying].hand[PROVINCE]) +
			", Cur:" + std::to_string((*iterator)->currentState.playerStates[(*iterator)->playerPlaying].hand[CURSE]);

		
		// Append visited
		text += " Vis:" + std::to_string((*iterator)->visited);

		// Append probability
		text += " Prob:" + std::to_string((*iterator)->probability);

		// Append currentplayer
		text += " Player:" + std::to_string((*iterator)->playerPlaying);

		// Append *tchu tchu*
		text += "\";";

		text += "\r\n";
	}

	file << text;
}







