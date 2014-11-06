#pragma once

#include <vector>
#include <fstream>

#include "GameState.h"
#include "Option.h"

// Flags
#define NOFLAG			0
#define REMODELFLAG		1
#define THIEFDRAW		2

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
	double sum;
	Option opt;
	std::vector<Node*> childrenPtrs;
	Node* parentPtr;

	int flags;

	void setOption(Option o);
	Node();
	~Node();
	void reset();

	std::string getName();
	void printSelf(std::ofstream& file);
};