#include <iostream>
#include <array>
#include "UCTMonteCarlo.h"



Option UCTMonteCarlo::doUCT(int UCTPlayer, GameState gameState, std::vector<Move> moveHistory)
{
	// Create inital root node and its children.
	Node* rootNode = requestNewNode();
	rootNode->isRoot = true;
	rootNode->visited = 0;
	rootNode->currentState = gameState;
	rootNode->playerPlaying = UCTPlayer;

	if (moveHistory.size() > 2)
	{
		Move lastMove = moveHistory.back();
		int size = moveHistory.size();
		if (lastMove.player == UCTPlayer && lastMove.absoluteCardId == REMODEL && lastMove.type == ACTION)		// Time to trash a card from remodel.
		{
			rootNode->opt.type = ACTION;
			rootNode->opt.absoluteCardId = REMODEL;
		}
		else if (lastMove.player == UCTPlayer && moveHistory[size - 2].absoluteCardId == REMODEL && lastMove.type == TRASH) // Time to gain a card from remodel.
		{
			rootNode->flags = REMODELFLAG;
			rootNode->opt.type = TRASH;
			rootNode->opt.absoluteCardId = lastMove.absoluteCardId;
		}
		else if (lastMove.player == UCTPlayer && lastMove.absoluteCardId == THIEF && lastMove.type == ACTION)
		{
			rootNode->opt.type = ACTION;
			rootNode->opt.absoluteCardId = THIEF;
		}
		else if (lastMove.player != UCTPlayer && moveHistory[size - 2].absoluteCardId == THIEF && lastMove.type == THIEFFLIP)
		{
			rootNode->flags = THIEFDRAW;
			rootNode->opt.type = THIEFFLIP;
			rootNode->opt.absoluteCardId = lastMove.absoluteCardId;
			rootNode->opt.absoluteExtraCardId = lastMove.absoluteExtraCardId;
		}
	}

	createAllChildren(rootNode);

	if (rootNode->childrenPtrs.size() == 1)
		return rootNode->childrenPtrs.at(0)->opt;

	// Perform UCT
	for (int i = 0; i < SIMULATIONS; i++)
	{
		expand(select(rootNode), UCTPlayer);
		/*if (i > 7 && i < 20)
			printTree(i, UCTPlayer, rootNode);*/
	}

	// Return best option
	Option bestOption;
	int mostVisited = 0;
	for (int i = 0; i < rootNode->childrenPtrs.size(); i++)
	{
		std::cout << rootNode->childrenPtrs.at(i)->opt.type;
		std::cout << CardManager::cardLookup[rootNode->childrenPtrs.at(i)->opt.absoluteCardId].name;
		std::cout << " visited: " << rootNode->childrenPtrs.at(i)->visited;
		std::cout << " score: " << rootNode->childrenPtrs.at(i)->value << std::endl;

		if (rootNode->childrenPtrs.at(i)->visited > mostVisited)
		{
			bestOption = rootNode->childrenPtrs.at(i)->opt;
			mostVisited = rootNode->childrenPtrs.at(i)->visited;
		}
	}
	if (PRINTTREE)
		printTree(gameState.turnCounter, UCTPlayer, rootNode);
	resetNodes();
	return bestOption;
}

Node* UCTMonteCarlo::select(Node* root)
{
	Node* currentNode = root;

	while (currentNode->childrenPtrs.size() > 0 && getUntriedChildren(currentNode).size() == 0)
	{
		currentNode = UCTSelectChild(currentNode);
	}
	return currentNode;
}

void UCTMonteCarlo::expand(Node* node, int UCTPlayer)
{
	if (getUntriedChildren(node).size() > 0)
	{
		Node* randomNode = getRandomNode(getUntriedChildren(node));
		rollout(randomNode, randomNode->currentState, UCTPlayer);
	}
	else
	{

		if (node->currentState.gameFinished())
		{
			rollout(node, node->currentState, UCTPlayer);
		}
		else
		{
			createAllChildren(node);
			Node* randomNode = getRandomNode(node->childrenPtrs);
			rollout(randomNode, randomNode->currentState, UCTPlayer);
		}
	}
}

// UCTPlayer is which player we are currently performing UCT for.
void UCTMonteCarlo::rollout(Node* node, GameState gameState, int UCTPlayer)
{
	int currentPlayer = node->playerPlaying;
	if (node->opt.type == END_TURN)
		currentPlayer++;
	if (currentPlayer >= gameState.playerStates.size())
	{
		gameState.turnCounter++;
		currentPlayer = 0;
	}
	while (!gameState.gameFinished())
	{
		while (gameState.playerStates[currentPlayer].buys > 0 || gameState.playerStates[currentPlayer].actions > 0)
		{
			Option cardChosen = getCardPlayoutPolicy(gameState, currentPlayer);
			if (cardChosen.type == ACTION)
			{
				playActionCard(gameState, cardChosen.absoluteCardId, currentPlayer, true);
			}
			else if (cardChosen.type == BUY)
			{
				gameState.playerStates[currentPlayer].actions = 0; // Can not perform more actions after entering buy phase.
				gameState.playerStates[currentPlayer].buyCard(cardChosen.absoluteCardId);
				gameState.supplyPiles[CardManager::cardIndexer[cardChosen.absoluteCardId]] -= 1;
			}
			else if (cardChosen.type == END_TURN)
			{
				break;
			}
		}
		gameState.playerStates[currentPlayer].endTurn();
		currentPlayer++;
		if (currentPlayer >= gameState.playerStates.size())
			currentPlayer = 0;
		if (currentPlayer == 0)
			gameState.turnCounter++;
	}

	double score = 0;
	if (gameState.playerStates.size() == 2)
	{
		if (UCTPlayer == 0)
		{
			score = gameState.playerStates[UCTPlayer].calculateVictoryPoints() / 100.;
			score +=((gameState.playerStates[UCTPlayer].calculateVictoryPoints() > gameState.playerStates[1].calculateVictoryPoints()) ? 1 : 0);
		}
		else
		{
			score = gameState.playerStates[UCTPlayer].calculateVictoryPoints() / 100.;
			score += ((gameState.playerStates[UCTPlayer].calculateVictoryPoints() < gameState.playerStates[0].calculateVictoryPoints()) ? 1 : 0);
		}
	}
	else
		score = gameState.playerStates[UCTPlayer].calculateVictoryPoints();

	propagate(node, score);

}

void UCTMonteCarlo::propagate(Node* node, double score)
{
	node->sum += score;
	node->visited++;
	node->value = double(node->sum) / double(node->visited);
	if (node->opt.type == DRAW)
	{
		if (INCLUDESCOREINDRAW)
			node->value *= node->probability;
		else
			node->value = node->probability;
	}
	if (!node->isRoot)
		propagate(node->parentPtr, score);
}

// Returns best child according to UCT.
Node* UCTMonteCarlo::UCTSelectChild(Node* root)
{
	double bestValue = 0;
	Node* bestNode;
	for (int i = 0; i < root->childrenPtrs.size(); i++)
	{
		double value = double(root->childrenPtrs.at(i)->value) + C * sqrt(log(double(root->visited) / root->childrenPtrs.at(i)->visited));

		if (value >= bestValue || bestValue == 0)
		{
			bestValue = value;
			bestNode = root->childrenPtrs.at(i);
		}
	}

	return bestNode;
}

void UCTMonteCarlo::playActionCard(GameState &gameState, int absoluteCardId, int playerIndex, bool rollout)
{
	switch (absoluteCardId)
	{
	case WOODCUTTER:
		gameState.playerStates[playerIndex].playCard(absoluteCardId);
		gameState.playerStates[playerIndex].buys += 1;
		break;
	case FESTIVAL:
		gameState.playerStates[playerIndex].playCard(absoluteCardId);
		gameState.playerStates[playerIndex].buys += 2;
		gameState.playerStates[playerIndex].actions += 2;
		break;
	case MONEYLENDER:
		gameState.playerStates[playerIndex].playCard(absoluteCardId);
		if (gameState.playerStates[playerIndex].hand[CardManager::cardIndexer[COPPER]] > 0)
		{
			gameState.playerStates[playerIndex].hand[CardManager::cardIndexer[COPPER]]--;
			gameState.trash[CardManager::cardIndexer[COPPER]]++;
			gameState.playerStates[playerIndex].spentMoney -= 3;	// Take away 3 spent money to give money if trashed. TODO: Use less hacky way
		}
		break;
	case SMITHY:
		gameState.playerStates[playerIndex].playCard(absoluteCardId);
		if (rollout)
			gameState.playerStates[playerIndex].drawCards(3);
		// Do nothing if in the UCT, because draw nodes are created in tree by probability calculating function.
		break;
	case VILLAGE:
		gameState.playerStates[playerIndex].playCard(absoluteCardId);
		gameState.playerStates[playerIndex].actions += 2;
		if (rollout)
			gameState.playerStates[playerIndex].drawCards(1);
		break;
	case MARKET:
		gameState.playerStates[playerIndex].playCard(absoluteCardId);
		gameState.playerStates[playerIndex].buys += 1;
		gameState.playerStates[playerIndex].actions += 1;
		if (rollout)
			gameState.playerStates[playerIndex].drawCards(1);
		break;
	case LABORATORY:
		gameState.playerStates[playerIndex].playCard(absoluteCardId);
		gameState.playerStates[playerIndex].actions += 1;
		if (rollout)
			gameState.playerStates[playerIndex].drawCards(2);
		break;
	case THIEF:
		gameState.playerStates[playerIndex].playCard(absoluteCardId);
		if (rollout)
		{
			int enemyIndex = 0;
			int card1Turned = -1;
			int card2Turned = -1;
			if (playerIndex == 0)
				enemyIndex = 1;
			
			gameState.playerStates[enemyIndex].flipThiefCards(card1Turned, card2Turned);
			if (card1Turned == GOLD || card2Turned == GOLD)
			{
				gameState.playerStates[playerIndex].discard[CardManager::cardIndexer[GOLD]]++;
				gameState.playerStates[enemyIndex].discard[CardManager::cardIndexer[GOLD]]--;
			}
			else if (card1Turned == SILVER || card2Turned == SILVER)
			{
				gameState.playerStates[playerIndex].discard[CardManager::cardIndexer[SILVER]]++;
				gameState.playerStates[enemyIndex].discard[CardManager::cardIndexer[SILVER]]--;
			}
		}
		else
		{
			
		}
		break;
	case WITCH:
		gameState.playerStates[playerIndex].playCard(absoluteCardId);
		if (gameState.playerStates.size() > 1)
		{
			int tempIndex = (playerIndex == gameState.playerStates.size()-1 ? 0 : playerIndex+1);
			while (tempIndex != playerIndex)
			{
				if (gameState.supplyPiles[CardManager::cardIndexer[CURSE]] > 0)
				{
					gameState.playerStates[tempIndex].discard[CardManager::cardIndexer[CURSE]]++;
					gameState.supplyPiles[CardManager::cardIndexer[CURSE]]--;
				}
				tempIndex++;
				if (tempIndex >= gameState.playerStates.size())
					tempIndex = 0;
			}
		}
		if (rollout)
			gameState.playerStates[playerIndex].drawCards(2);
		break;
	case BUREAUCRAT:
		gameState.playerStates[playerIndex].playCard(absoluteCardId);
		if (gameState.supplyPiles[CardManager::cardIndexer[SILVER]] > 0)
		{
			gameState.playerStates[playerIndex].addToTopOfDeck(CardManager::cardIndexer[SILVER]);
			gameState.supplyPiles[CardManager::cardIndexer[SILVER]]--;
		}
		for (int i = 0; i < gameState.playerStates.size(); i++)
		{
			if (i != playerIndex)
			{
				if (gameState.playerStates[i].hand[CardManager::cardIndexer[ESTATE]] > 0)
				{
					gameState.playerStates[i].addToTopOfDeck(CardManager::cardIndexer[ESTATE]);
					gameState.playerStates[i].hand[CardManager::cardIndexer[ESTATE]]--;
				}
				else if (gameState.playerStates[i].hand[CardManager::cardIndexer[DUCHY]] > 0)
				{
					gameState.playerStates[i].addToTopOfDeck(CardManager::cardIndexer[DUCHY]);
					gameState.playerStates[i].hand[CardManager::cardIndexer[DUCHY]]--;
				}
				else if (gameState.playerStates[i].hand[CardManager::cardIndexer[PROVINCE]] > 0)
				{
					gameState.playerStates[i].addToTopOfDeck(CardManager::cardIndexer[PROVINCE]);
					gameState.playerStates[i].hand[CardManager::cardIndexer[PROVINCE]]--;
				}
				else if (gameState.playerStates[i].hand[CardManager::cardIndexer[GARDENS]] > 0)
				{
					gameState.playerStates[i].addToTopOfDeck(CardManager::cardIndexer[GARDENS]);
					gameState.playerStates[i].hand[CardManager::cardIndexer[GARDENS]]--;
				}
			}
		}
		break;
	case REMODEL:
		gameState.playerStates[playerIndex].playCard(absoluteCardId);
		if (rollout)
		{
			if (gameState.playerStates[playerIndex].hand[CardManager::cardIndexer[CURSE]] > 0 && gameState.supplyPiles[CardManager::cardIndexer[ESTATE]] > 0)
			{
				gameState.playerStates[playerIndex].hand[CardManager::cardIndexer[CURSE]]--;
				gameState.playerStates[playerIndex].hand[CardManager::cardIndexer[ESTATE]]++;
				gameState.trash[CardManager::cardIndexer[CURSE]]++;
			}
			else if (gameState.playerStates[playerIndex].hand[CardManager::cardIndexer[COPPER]] > 0 && gameState.supplyPiles[CardManager::cardIndexer[ESTATE]] > 0)
			{
				gameState.playerStates[playerIndex].hand[CardManager::cardIndexer[COPPER]]--;
				gameState.playerStates[playerIndex].hand[CardManager::cardIndexer[ESTATE]]++;
				gameState.trash[CardManager::cardIndexer[COPPER]]++;
			}
			else if (gameState.playerStates[playerIndex].hand[CardManager::cardIndexer[SILVER]] > 0 && gameState.supplyPiles[CardManager::cardIndexer[DUCHY]] > 0)
			{
				gameState.playerStates[playerIndex].hand[CardManager::cardIndexer[SILVER]]--;
				gameState.playerStates[playerIndex].hand[CardManager::cardIndexer[DUCHY]]++;
				gameState.trash[CardManager::cardIndexer[SILVER]]++;
			}
			else if (gameState.playerStates[playerIndex].hand[CardManager::cardIndexer[GOLD]] > 0 && gameState.supplyPiles[CardManager::cardIndexer[PROVINCE]] > 0)
			{
				gameState.playerStates[playerIndex].hand[CardManager::cardIndexer[GOLD]]--;
				gameState.playerStates[playerIndex].hand[CardManager::cardIndexer[PROVINCE]]++;
				gameState.trash[CardManager::cardIndexer[GOLD]]++;
			}
		}
		// Do nothing when played in tree, trashnodes will be created beneath.
		break;
	default:
		break;
	}
}

void UCTMonteCarlo::createAllChildren(Node* node)
{
	// If parent is end turn, end old players turn and increment turn counter.

	int currentlyPlaying = node->playerPlaying;
	GameState currentState = node->currentState;

	if (node->opt.type == DRAW && node->parentPtr->opt.type == END_TURN)
	{
		currentlyPlaying++;
		if (currentlyPlaying >= node->currentState.playerStates.size())
		{
			currentlyPlaying = 0;
			currentState.turnCounter++;
		}
	}

	if (node->opt.type == END_TURN)
	{
		// Clean-up
		for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
		{
			currentState.playerStates[currentlyPlaying].discard[cardIndex] += (currentState.playerStates[currentlyPlaying].hand[cardIndex]);
			currentState.playerStates[currentlyPlaying].hand[cardIndex] = 0;
			currentState.playerStates[currentlyPlaying].discard[cardIndex] += currentState.playerStates[currentlyPlaying].inPlay[cardIndex];
			currentState.playerStates[currentlyPlaying].inPlay[cardIndex] = 0;
		}
		currentState.playerStates[currentlyPlaying].buys = 1;
		currentState.playerStates[currentlyPlaying].actions = 1;
		currentState.playerStates[currentlyPlaying].spentMoney = 0;

		createDrawNodes(node, currentState, currentlyPlaying, 5, false);

	}	// If action and a card which wants to create new nodes beneath.
	else if (node->opt.type == ACTION && (node->opt.absoluteCardId == SMITHY || node->opt.absoluteCardId == VILLAGE || node->opt.absoluteCardId == MARKET || node->opt.absoluteCardId == LABORATORY || node->opt.absoluteCardId == WITCH || node->opt.absoluteCardId == REMODEL || node->opt.absoluteCardId == THIEF))
	{
		switch (node->opt.absoluteCardId)
		{
		case SMITHY:
			createDrawNodes(node, currentState, currentlyPlaying, 3, false);
			break;
		case VILLAGE:
			createDrawNodes(node, currentState, currentlyPlaying, 1, false);
			break;
		case MARKET:
			createDrawNodes(node, currentState, currentlyPlaying, 1, false);
			break;
		case LABORATORY:
			createDrawNodes(node, currentState, currentlyPlaying, 2, false);
			break;
		case WITCH:
			createDrawNodes(node, currentState, currentlyPlaying, 2, false);
			break;
		case REMODEL:
			createTrashNodes(node, currentState, currentlyPlaying);
			break;
		case THIEF:
			createDrawNodes(node, currentState, currentlyPlaying == 0 ? 1 : 0, 2, true); // Currently playing set to not self.
			break;
		default:
			break;
		}
	}
	else if (node->flags == THIEFDRAW)
	{
		int enemyPlayer = currentlyPlaying;
		if (enemyPlayer == 0)
			currentlyPlaying = 1;	// Currently playing is not the one set as "player" when drawing for thief.
		
		if (node->opt.absoluteCardId == CardManager::cardLookup[GOLD].id || node->opt.absoluteExtraCardId == CardManager::cardLookup[GOLD].id)
		{
			Node* newNode = requestNewNode();
			newNode->currentState = currentState;
			newNode->currentState.playerStates[enemyPlayer].deck[CardManager::cardIndexer[GOLD]]--;
			if (node->opt.absoluteCardId == CardManager::cardLookup[GOLD].id)
				node->currentState.playerStates[enemyPlayer].discard[CardManager::cardIndexer[node->opt.absoluteExtraCardId]]++;
			else
				node->currentState.playerStates[enemyPlayer].discard[CardManager::cardIndexer[node->opt.absoluteCardId]]++;
			newNode->currentState.trash[CardManager::cardIndexer[GOLD]]++;
			newNode->opt.type = THIEFTRASH;
			newNode->opt.absoluteCardId = CardManager::cardLookup[GOLD].id;
			newNode->parentPtr = node;
			node->childrenPtrs.push_back(newNode);
			newNode->playerPlaying = currentlyPlaying;
			newNode->flags = NOFLAG;

			Node* newGainNode = requestNewNode();
			newGainNode->currentState = currentState;
			newGainNode->currentState.playerStates[enemyPlayer].deck[CardManager::cardIndexer[GOLD]]--;
			if (node->opt.absoluteCardId == CardManager::cardLookup[GOLD].id)
				node->currentState.playerStates[enemyPlayer].discard[CardManager::cardIndexer[node->opt.absoluteExtraCardId]]++;
			else
				node->currentState.playerStates[enemyPlayer].discard[CardManager::cardIndexer[node->opt.absoluteCardId]]++;
			newGainNode->currentState.playerStates[currentlyPlaying].discard[CardManager::cardIndexer[GOLD]]++;
			newGainNode->opt.type = THIEFGAIN;
			newGainNode->opt.absoluteCardId = CardManager::cardLookup[GOLD].id;
			newGainNode->parentPtr = node;
			node->childrenPtrs.push_back(newGainNode);
			newGainNode->playerPlaying = currentlyPlaying;
			newGainNode->flags = NOFLAG;
		}
		if (node->opt.absoluteCardId == CardManager::cardLookup[SILVER].id || node->opt.absoluteExtraCardId == CardManager::cardLookup[SILVER].id)
		{
			Node* newNode = requestNewNode();
			newNode->currentState = currentState;
			newNode->currentState.playerStates[enemyPlayer].deck[CardManager::cardIndexer[SILVER]]--;
			if (node->opt.absoluteCardId == CardManager::cardLookup[SILVER].id)
				node->currentState.playerStates[enemyPlayer].discard[CardManager::cardIndexer[node->opt.absoluteExtraCardId]]++;
			else
				node->currentState.playerStates[enemyPlayer].discard[CardManager::cardIndexer[node->opt.absoluteCardId]]++;
			newNode->currentState.trash[CardManager::cardIndexer[SILVER]]++;
			newNode->opt.type = THIEFTRASH;
			newNode->opt.absoluteCardId = CardManager::cardLookup[SILVER].id;
			newNode->parentPtr = node;
			node->childrenPtrs.push_back(newNode);
			newNode->playerPlaying = currentlyPlaying;
			newNode->flags = NOFLAG;

			Node* newGainNode = requestNewNode();
			newGainNode->currentState = currentState;
			newGainNode->currentState.playerStates[enemyPlayer].deck[CardManager::cardIndexer[SILVER]]--;
			if (node->opt.absoluteCardId == CardManager::cardLookup[SILVER].id)
				node->currentState.playerStates[enemyPlayer].discard[CardManager::cardIndexer[node->opt.absoluteExtraCardId]]++;
			else
				node->currentState.playerStates[enemyPlayer].discard[CardManager::cardIndexer[node->opt.absoluteCardId]]++;
			newGainNode->currentState.playerStates[currentlyPlaying].discard[CardManager::cardIndexer[SILVER]]++;
			newGainNode->opt.type = THIEFGAIN;
			newGainNode->opt.absoluteCardId = CardManager::cardLookup[SILVER].id;
			newGainNode->parentPtr = node;
			node->childrenPtrs.push_back(newGainNode);
			newGainNode->playerPlaying = currentlyPlaying;
			newGainNode->flags = NOFLAG;
		}
		if (node->opt.absoluteCardId == CardManager::cardLookup[COPPER].id || node->opt.absoluteExtraCardId == CardManager::cardLookup[COPPER].id)
		{
			Node* newNode = requestNewNode();
			newNode->currentState = currentState;
			newNode->currentState.playerStates[enemyPlayer].deck[CardManager::cardIndexer[COPPER]]--;
			if (node->opt.absoluteCardId == CardManager::cardLookup[COPPER].id)
				node->currentState.playerStates[enemyPlayer].discard[CardManager::cardIndexer[node->opt.absoluteExtraCardId]]++;
			else
				node->currentState.playerStates[enemyPlayer].discard[CardManager::cardIndexer[node->opt.absoluteCardId]]++;
			newNode->currentState.trash[CardManager::cardIndexer[COPPER]]++;
			newNode->opt.type = THIEFTRASH;
			newNode->opt.absoluteCardId = CardManager::cardLookup[COPPER].id;
			newNode->parentPtr = node;
			node->childrenPtrs.push_back(newNode);
			newNode->playerPlaying = currentlyPlaying;
			newNode->flags = NOFLAG;

			Node* newGainNode = requestNewNode();
			newGainNode->currentState = currentState;
			newGainNode->currentState.playerStates[enemyPlayer].deck[CardManager::cardIndexer[COPPER]]--;
			if (node->opt.absoluteCardId == CardManager::cardLookup[COPPER].id)
				node->currentState.playerStates[enemyPlayer].discard[CardManager::cardIndexer[node->opt.absoluteExtraCardId]]++;
			else
				node->currentState.playerStates[enemyPlayer].discard[CardManager::cardIndexer[node->opt.absoluteCardId]]++;
			newGainNode->currentState.playerStates[currentlyPlaying].discard[CardManager::cardIndexer[COPPER]]++;
			newGainNode->opt.type = THIEFGAIN;
			newGainNode->opt.absoluteCardId = CardManager::cardLookup[COPPER].id;
			newGainNode->parentPtr = node;
			node->childrenPtrs.push_back(newGainNode);
			newGainNode->playerPlaying = currentlyPlaying;
			newGainNode->flags = NOFLAG;
		}
		if (node->childrenPtrs.size() <= 0)
		{
			node->flags = NOFLAG;
			createAllChildren(node);
			return;
		}
	}
	else if (node->flags == REMODELFLAG)
	{
		int cost = CardManager::cardLookup[node->opt.absoluteCardId].cost;

		std::vector<Option> possibleGains = getBuyOptions(&currentState, cost+2);
		if (possibleGains.size() == 0)
		{
			node->flags = NOFLAG;
			createAllChildren(node);
			return;
		}
		for (int i = 0; i < possibleGains.size(); i++)
		{
			possibleGains.at(i).type = GAIN;
			Node* newGainNode = requestNewNode();
			newGainNode->opt = possibleGains.at(i);
			newGainNode->parentPtr = node;
			newGainNode->currentState = currentState;
			newGainNode->playerPlaying = currentlyPlaying;
			newGainNode->currentState.playerStates[currentlyPlaying].actions = 0;

			newGainNode->currentState.playerStates[currentlyPlaying].hand[CardManager::cardIndexer[newGainNode->opt.absoluteCardId]]++;
			newGainNode->currentState.supplyPiles[CardManager::cardIndexer[newGainNode->opt.absoluteCardId]] -= 1;

			node->childrenPtrs.push_back(newGainNode);
		}
	}
	else
	{
		// Create end turn node to enable doing nothing.
		Node* endTurnChild = requestNewNode();
		Option o;
		o.type = END_TURN;
		o.absoluteCardId = -1;
		endTurnChild->opt = o;
		endTurnChild->parentPtr = node;
		endTurnChild->currentState = currentState;
		endTurnChild->playerPlaying = currentlyPlaying;
		node->childrenPtrs.push_back(endTurnChild);

		// Add all possible actions.
		if (node->currentState.playerStates[currentlyPlaying].actions > 0)
		{
			std::vector<Option> possibleActions = getActionOptions(&node->currentState, node->currentState.playerStates[currentlyPlaying].hand);
			for (int i = 0; i < possibleActions.size(); i++)
			{
				Node* newActionNode = requestNewNode();
				newActionNode->opt = possibleActions.at(i);
				newActionNode->parentPtr = node;
				newActionNode->currentState = currentState;
				newActionNode->playerPlaying = currentlyPlaying;
				playActionCard(newActionNode->currentState, newActionNode->opt.absoluteCardId, newActionNode->playerPlaying, false);
				node->childrenPtrs.push_back(newActionNode);
			}
		}

		// Add all possible buy nodes.
		if (node->currentState.playerStates[currentlyPlaying].buys > 0)
		{
			int currentMoney = getCurrentMoney(&node->currentState, currentlyPlaying);
			std::vector<Option> possibleBuys = getBuyOptions(&node->currentState, currentMoney);
			for (int i = 0; i < possibleBuys.size(); i++)
			{
				Node* newBuyNode = requestNewNode();
				newBuyNode->opt = possibleBuys.at(i);
				newBuyNode->parentPtr = node;
				newBuyNode->currentState = currentState;
				newBuyNode->playerPlaying = currentlyPlaying;
				newBuyNode->currentState.playerStates[currentlyPlaying].actions = 0;
				// Perform the buy action of this node.
				newBuyNode->currentState.playerStates[currentlyPlaying].buyCard(newBuyNode->opt.absoluteCardId);
				newBuyNode->currentState.supplyPiles[CardManager::cardIndexer[newBuyNode->opt.absoluteCardId]] -= 1;
				node->childrenPtrs.push_back(newBuyNode);
			}
		}
	
	}
}

void UCTMonteCarlo::createTrashNodes(Node* parentNode, GameState& currentState, int currentlyPlaying)
{
	for (int i = 0; i < INSUPPLY; i++)
	{
		GameState  copyState = currentState;
		if (currentState.playerStates[currentlyPlaying].hand[i] > 0)
		{
			Node* newNode = requestNewNode();
			Option o;
			o.type = TRASH;
			o.absoluteCardId = CardManager::cardLookupByIndex[i].id;
			copyState.playerStates[currentlyPlaying].hand[i]--;
			copyState.trash[i]++;
			newNode->opt = o;
			newNode->currentState = copyState;
			newNode->parentPtr = parentNode;
			newNode->flags = REMODELFLAG;
			newNode->playerPlaying = currentlyPlaying;
			parentNode->childrenPtrs.push_back(newNode);
		}
	}
}

Node* UCTMonteCarlo::getRandomNode(std::vector<Node*> nodes)
{
	int randomNode = rand() % nodes.size();

	return nodes.at(randomNode);
}

std::vector<Node*> UCTMonteCarlo::getUntriedChildren(Node* parent)
{
	std::vector<Node*> untriedNodes;

	for (int i = 0; i < parent->childrenPtrs.size(); i++)
	{
		if (parent->childrenPtrs.at(i)->visited <= 0)
			untriedNodes.push_back(parent->childrenPtrs.at(i));
	}

	return untriedNodes;
}

int UCTMonteCarlo::getCurrentMoney(GameState* gameState, int playerIndex)
{
	int currentMoney = 0;
	currentMoney += gameState->playerStates[playerIndex].hand[CardManager::cardIndexer[COPPER]];
	currentMoney += gameState->playerStates[playerIndex].hand[CardManager::cardIndexer[SILVER]] * 2;
	currentMoney += gameState->playerStates[playerIndex].hand[CardManager::cardIndexer[GOLD]] * 3;
	currentMoney += gameState->playerStates[playerIndex].inPlay[CardManager::cardIndexer[WOODCUTTER]] * 2;
	currentMoney += gameState->playerStates[playerIndex].inPlay[CardManager::cardIndexer[FESTIVAL]] * 2;
	currentMoney += gameState->playerStates[playerIndex].inPlay[CardManager::cardIndexer[MARKET]] * 1;
	currentMoney -= gameState->playerStates[playerIndex].spentMoney;
	return currentMoney;
}

std::vector<Option> UCTMonteCarlo::getBuyOptions(GameState* gameState, int currentMoney)
{
	std::vector<Option> options;
	for (int i = 0; i < INSUPPLY; i++)
	{
		if (CardManager::cardLookupByIndex[i].cost <= currentMoney && gameState->supplyPiles[i] > 0)
		{
			Option o;
			o.type = BUY;
			o.absoluteCardId = CardManager::cardLookupByIndex[i].id;
			options.push_back(o);
		}
	}

	return options;
}

void UCTMonteCarlo::createDrawNodes(Node* parentNode, GameState& currentState, int currentlyPlaying, int numberOfCards, bool createThiefDraws)
{
	// Create as many draw nodes as there are combinations of cards, based on numberOfCards to draw, and the current deck.
	// If there are cards on top, then draw as many of these needed first, before moving on to draw from deck.
	// If deck does not have enough cards, then first draw the ones in draw, then shuffle and draw the rest.

	GameState copyState = currentState;

	std::array<int, INSUPPLY> guaranteedCards;
	for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++) // Initialize guaranteedCards array to zeroes.
		guaranteedCards[cardIndex] = 0;

	// If there are any cards on top, put them in guaranteed cards, and decrement numberOfCards.
	// If numberOfCards reaches zero, then no more cards should be drawn.
	while (numberOfCards > 0 && copyState.playerStates[currentlyPlaying].topOfDeckAsIndex.size() > 0)
	{
		numberOfCards--; // Decrement number of cards for each draw, based on the number of guaranteed cards.
		guaranteedCards[copyState.playerStates[currentlyPlaying].topOfDeckAsIndex.top()]++;
		copyState.playerStates[currentlyPlaying].deck[copyState.playerStates[currentlyPlaying].topOfDeckAsIndex.top()]--;
		copyState.playerStates[currentlyPlaying].topOfDeckAsIndex.pop();
	}
	

	// Check for shuffle
	int cardCounter = 0;
	for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
	{
		cardCounter += copyState.playerStates[currentlyPlaying].deck[cardIndex];
	}
	// "Shuffle" after making current cards guaranteedcards.
	if (cardCounter < numberOfCards)
	{
		// Put the remaining cards in deck over in guaranteedCards, put discard in deck and set discard to zero.
		for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
		{
			guaranteedCards[cardIndex] += copyState.playerStates[currentlyPlaying].deck[cardIndex];
			copyState.playerStates[currentlyPlaying].deck[cardIndex] = copyState.playerStates[currentlyPlaying].discard[cardIndex];
			copyState.playerStates[currentlyPlaying].discard[cardIndex] = 0;
		}
	}

	std::vector<std::array<int, INSUPPLY>> draws;
	std::vector<double> probabilities;

	// Create one draw if there are enough known cards to draw.
	if (numberOfCards == 0)
	{
		std::array<int, INSUPPLY> draw;
		for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
			draw[cardIndex] = guaranteedCards[cardIndex]; // The draw is equal to each card in guaranteedCards
		draws.push_back(draw);
		probabilities.push_back(1.0);
	}
	else
	{
		// Finding n and k for draw probability
		int n = 0; // Total cards - those in guaranteedCards and discard. Possible cards to draw.
		std::size_t k = numberOfCards;
		// If less than numberOfCards in cardCounter, then draw fewer cards.
		if (cardCounter < numberOfCards)
			k = numberOfCards - cardCounter;

		// Append each card in deck to string
		std::string s = "";
		for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
		{
			for (int j = 0; j < copyState.playerStates[currentlyPlaying].deck[cardIndex]; j++)
			{
				s.append(CardManager::cardLookupByIndex[cardIndex].charId);
				n++;
			}
		}

		do // While there are still new combinations of chars in string, create a new draw.
		{
			double probability = 0, nkInCardComboPossibilities = 1, nkPossibilities = 1;

			std::array<int, INSUPPLY> draw;
			for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++) // Dynamic initialization
				draw[cardIndex] = 0;

			// This is the combinationString, containing a combination of cards.
			std::string combinationString = std::string(s.begin(), s.begin() + k);

			// For each letter in the combinationString
			for (int stringIndex = 0; stringIndex < k; stringIndex++)
			{
				// Convert letter to int, and add to draw
				std::string cardCharId = std::string(combinationString.begin() + stringIndex, combinationString.begin() + stringIndex + 1);
				draw[CardManager::cardLookupCharToIndex[cardCharId]]++;
			}

			// Find possible ways to draw the draw
			for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
			{
				int nInCardCombo = copyState.playerStates[currentlyPlaying].deck[cardIndex];
				int kInCardCombo = draw[cardIndex];
				nkInCardComboPossibilities *= choose(nInCardCombo, kInCardCombo);
			}

			// Add the guaranteedCards
			for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
			{
				draw[cardIndex] += guaranteedCards[cardIndex];
			}

			// Find the total possible outcomes
			nkPossibilities = choose(n, k);

			// Calculate probability
			probability = nkInCardComboPossibilities / nkPossibilities;

			// Finally, add draw to collection of draws, and probability to collection of probabilities (associate a probability with each draw)
			draws.push_back(draw);
			probabilities.push_back(probability);
		} while (next_combination(s.begin(), s.begin() + k, s.end()));
	}


	if (createThiefDraws)
	{
		// For each draw, create child node
		for (int drawCounter = 0; drawCounter < draws.size(); drawCounter++)
		{
			Node* newNodePtr = requestNewNode();
			newNodePtr->currentState = currentState;
			newNodePtr->opt.absoluteCardId = 0;
			newNodePtr->opt.absoluteExtraCardId = 0;
			for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
			{
				if (newNodePtr->opt.absoluteCardId == 0 && draws[drawCounter][cardIndex] > 0)
					newNodePtr->opt.absoluteCardId = CardManager::cardLookupByIndex[cardIndex].id;
				else if (draws[drawCounter][cardIndex] > 0)
					newNodePtr->opt.absoluteExtraCardId = CardManager::cardLookupByIndex[cardIndex].id;

				newNodePtr->currentState.playerStates[currentlyPlaying].deck[cardIndex] = copyState.playerStates[currentlyPlaying].deck[cardIndex] + guaranteedCards[cardIndex];
				newNodePtr->currentState.playerStates[currentlyPlaying].discard[cardIndex] = copyState.playerStates[currentlyPlaying].discard[cardIndex];
				newNodePtr->currentState.supplyPiles[cardIndex] = copyState.supplyPiles[cardIndex];
			}
			newNodePtr->currentState.playerStates[currentlyPlaying].topOfDeckAsIndex = copyState.playerStates[currentlyPlaying].topOfDeckAsIndex;
			parentNode->childrenPtrs.push_back(newNodePtr);
			newNodePtr->parentPtr = parentNode;
			newNodePtr->playerPlaying = currentlyPlaying;
			newNodePtr->opt.type = THIEFFLIP;
			newNodePtr->flags = THIEFDRAW;
			newNodePtr->probability = probabilities[drawCounter];
		}
	}
	else
	{
		// For each draw, create child node
		for (int drawCounter = 0; drawCounter < draws.size(); drawCounter++)
		{
			Node* newNodePtr = requestNewNode();
			newNodePtr->currentState = currentState;

			for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
			{
				newNodePtr->currentState.playerStates[currentlyPlaying].hand[cardIndex] += draws[drawCounter][cardIndex];
				newNodePtr->currentState.playerStates[currentlyPlaying].deck[cardIndex] = copyState.playerStates[currentlyPlaying].deck[cardIndex] + guaranteedCards[cardIndex] - draws[drawCounter][cardIndex];
				newNodePtr->currentState.playerStates[currentlyPlaying].discard[cardIndex] = copyState.playerStates[currentlyPlaying].discard[cardIndex];
				newNodePtr->currentState.supplyPiles[cardIndex] = copyState.supplyPiles[cardIndex];
			}
			newNodePtr->currentState.playerStates[currentlyPlaying].topOfDeckAsIndex = copyState.playerStates[currentlyPlaying].topOfDeckAsIndex;
			parentNode->childrenPtrs.push_back(newNodePtr);
			newNodePtr->parentPtr = parentNode;
			newNodePtr->playerPlaying = currentlyPlaying;
			newNodePtr->opt.absoluteCardId = -2;
			newNodePtr->opt.type = DRAW;
			newNodePtr->visited = 1;
			newNodePtr->value = probabilities[drawCounter];
			newNodePtr->probability = probabilities[drawCounter];
		}
	}
}

std::vector<Option> UCTMonteCarlo::getActionOptions(GameState* gameState, const int(&hand)[INSUPPLY])
{
	std::vector<Option> actionOptions;
	if (hand[CardManager::cardIndexer[WOODCUTTER]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = WOODCUTTER;
		actionOptions.push_back(o);
	}
	if (hand[CardManager::cardIndexer[FESTIVAL]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = FESTIVAL;
		actionOptions.push_back(o);
	}
	if (hand[CardManager::cardIndexer[MONEYLENDER]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = MONEYLENDER;
		actionOptions.push_back(o);
	}
	if (hand[CardManager::cardIndexer[SMITHY]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = SMITHY;
		actionOptions.push_back(o);
	}

	if (hand[CardManager::cardIndexer[VILLAGE]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = VILLAGE;
		actionOptions.push_back(o);
	}
	if (hand[CardManager::cardIndexer[MARKET]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = MARKET;
		actionOptions.push_back(o);
	}
	if (hand[CardManager::cardIndexer[LABORATORY]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = LABORATORY;
		actionOptions.push_back(o);
	}
	if (hand[CardManager::cardIndexer[WITCH]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = WITCH;
		actionOptions.push_back(o);
	}
	if (hand[CardManager::cardIndexer[BUREAUCRAT]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = BUREAUCRAT;
		actionOptions.push_back(o);
	}
	if (hand[CardManager::cardIndexer[REMODEL]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = REMODEL;
		actionOptions.push_back(o);
	}
	if (hand[CardManager::cardIndexer[THIEF]] > 0)
	{
		Option o;
		o.type = ACTION;
		o.absoluteCardId = THIEF;
		actionOptions.push_back(o);
	}
	return actionOptions;
}

Option UCTMonteCarlo::getCardPlayoutPolicy(GameState& gameState, int playerIndex)
{
	std::vector<Option>::iterator iter;

	Option cardToReturn;
	cardToReturn.type = END_TURN;
	cardToReturn.absoluteCardId = -1;
	int highestCost = 0;

	if (gameState.playerStates[playerIndex].actions > 0)
	{
		std::vector<Option> actionOptions = getActionOptions(&gameState, gameState.playerStates[playerIndex].hand);
		if (actionOptions.size() > 0)
		{
			for (iter = actionOptions.begin(); iter != actionOptions.end(); iter++)
			{
				if (CardManager::cardLookup[(*iter).absoluteCardId].cost > highestCost || highestCost == 0)
				{
					highestCost = CardManager::cardLookup[(*iter).absoluteCardId].cost;
					cardToReturn.type = ACTION;
					cardToReturn.absoluteCardId = (*iter).absoluteCardId;

					// If has an action card which gives +actions, play it.
					if (cardToReturn.absoluteCardId == MARKET || cardToReturn.absoluteCardId == LABORATORY || cardToReturn.absoluteCardId == VILLAGE || cardToReturn.absoluteCardId == FESTIVAL)
						return cardToReturn;
				}
			}

			return cardToReturn;	// Return early if there was an action card.
		}
	}
	if (gameState.playerStates[playerIndex].buys > 0)
	{
		std::vector<Option> buyOptions = getBuyOptions(&gameState, getCurrentMoney(&gameState, playerIndex));
		cardToReturn.absoluteCardId = -1;
		double highestScore = 0;

		for (iter = buyOptions.begin(); iter != buyOptions.end(); iter++)
		{
			if (cardHeuristic(gameState, playerIndex, (*iter).absoluteCardId) > highestScore || highestScore == 0 && CardManager::cardLookup[(*iter).absoluteCardId].name != "Curse")
			{
				highestScore = CardManager::cardLookup[(*iter).absoluteCardId].cost;
				cardToReturn.absoluteCardId = (*iter).absoluteCardId;
				cardToReturn.type = BUY;
			}
		}
	}

	return cardToReturn;	// -1 if there were no buys or actions available.
}


double UCTMonteCarlo::cardHeuristic(GameState currentState, int playerIndex, int absoluteCardId)
{
	double score = 0;
	PlayerState currentPlayerState = currentState.playerStates[playerIndex];
	PlayerState enemyPlayerState = currentState.playerStates[playerIndex == 0 ? 1 : 0];
	switch (absoluteCardId)
	{
	case BUREAUCRAT:
		score -= currentState.turnCounter / 5;
		score += enemyPlayerState.countCards(ESTATE);
		score += enemyPlayerState.countCards(DUCHY);
		score += enemyPlayerState.countCards(PROVINCE);
		if (GARDENSINGAME)
			score += currentPlayerState.countCards(GARDENS);
		break;
	case FESTIVAL:
		score -= currentState.turnCounter / 10;
		score += currentPlayerState.calculateCurrentMoney();
		if (SMITHYINGAME)
			score += currentPlayerState.countCards(SMITHY);
		if (WITCHINGAME)
			score += currentPlayerState.countCards(WITCH);
		if (LABORATORYINGAME)
			score += currentPlayerState.countCards(LABORATORY);
		if (GARDENSINGAME)
			score += currentPlayerState.countCards(GARDENS);
		break;
	case GARDENS:
		score += floor( (currentPlayerState.countCards() + currentPlayerState.buys) / 10);
		score += currentState.turnCounter / 10;
		break;
	case LABORATORY:
		score -= currentState.turnCounter / 10;
		if (VILLAGEINGAME)
			score += currentPlayerState.countCards(VILLAGE);
		if (FESTIVALINGAME)
			score += currentPlayerState.countCards(FESTIVAL);
		if (MARKETINGAME)
			score += currentPlayerState.countCards(MARKET);
		break;
	case MARKET:
		score -= currentState.turnCounter / 10;
		if (GARDENSINGAME)
			score += currentPlayerState.countCards(GARDENS);
		break;
	case MONEYLENDER:
		score -= currentState.turnCounter;
		score += currentPlayerState.countCards(COPPER);
		break;
	case REMODEL:
		break;
	case SMITHY:
		break;
	case THIEF:
		break;
	case VILLAGE:
		break;
	case WITCH:
		if (currentState.supplyPiles[CardManager::cardIndexer[CURSE]] > 0)
			score = 40;
		break;
	case WOODCUTTER:
		break;
	default:
		break;

	}


	return score;
}


// Tree printing
void UCTMonteCarlo::printTree(int turnCounter, int player, Node* rootNodePtr)
{
	std::string fileName = std::to_string(turnCounter) + "_" + std::to_string(player) + "uctTree.gv";
	remove(fileName.c_str());
	std::ofstream file;
	file.open(fileName, std::ios::app);
	std::string text = "digraph ucttree{\r\n size = \"10000000000!, 1000000000\";\r\n ratio = \"expand\";\r\n node[color = lightblue2, style = filled];";
	file << text << std::endl;
	printNode(rootNodePtr, file);
	text = "\r\n }";
	file << text << std::endl;
	file.close();
}
void UCTMonteCarlo::printNode(Node* nodePtr, std::ofstream& file)
{
	nodePtr->printSelf(file);
	for (std::vector<Node*>::iterator it = nodePtr->childrenPtrs.begin(); it != nodePtr->childrenPtrs.end(); ++it)
		printNode(*it, file);
}


// Node allocation
UCTMonteCarlo::UCTMonteCarlo()
{
	nodeAllocationPtr = new Node[NODESTOALLOCATE];
	handedOutNodes = 0;
	for (int counter = 0; counter < NODESTOALLOCATE; counter++)
	{
		nodeAllocationPtr[counter].id = counter;
		nodeAllocationPtr[counter].reset();
	}
}
UCTMonteCarlo::~UCTMonteCarlo()
{
	delete[] nodeAllocationPtr;
	//std::vector<Node*>().swap(usedNodes);
}
Node* UCTMonteCarlo::requestNewNode()
{
	if (handedOutNodes >= NODESTOALLOCATE)
	{
		std::cout << "No more nodes!" << std::endl;
	}
	handedOutNodes++;
	return &nodeAllocationPtr[handedOutNodes-1];
}
void UCTMonteCarlo::resetNodes()
{
	for (int counter = 0; counter < NODESTOALLOCATE; counter++)
	{
		nodeAllocationPtr[counter].reset();
	}
	handedOutNodes = 0;
}

unsigned long long UCTMonteCarlo::choose(unsigned long long n, unsigned long long k)
{
	if (k > n) {
		return 0;
	}
	unsigned long long r = 1;
	for (unsigned long long d = 1; d <= k; ++d) {
		r *= n--;
		r /= d;
	}
	return r;
}


// TODO: Score on the last turn is wrong, because the UCT simulates on too many turns, after moving of turncounter incrementation.
// TODO: Bureaucrat cheats!
// TODO: Visited:1 Cases are weird
// TODO: Possibly use const & instead of copying gamestates, where we do not change them. To increase speed.
// TODO: Accomodate for all cards
// TODO: Support for not drawing more cards, if both discard and deck is empty.