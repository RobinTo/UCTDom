#pragma once

#include <vector>
#include <fstream>

#include "GameState.h"
#include "Option.h"

// Flags
#define NOFLAG			0
#define REMODELFLAG		1

class Node
{
public:
	GameState currentState;
	int id;
	int playerPlaying;
	int visited;
	bool isRoot;
	double value;
	double probability;
	int sum;
	Option opt;
	//int propagateCounter;
	std::vector<Node*> childrenPtrs;
	Node* parentPtr;

	int flags;

	void setOption(Option o);
	//void setState(GameState gameState);
	Node();
	~Node();
	void reset();

	//void findMoves(CardManager cardManager);
	void printSelf(std::ofstream& file);
};