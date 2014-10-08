#pragma once

#include <vector>
#include "Player.h"
struct LogItem
{
	int turn;

	int p1Score;
	int p2Score;

	int p1Copper;
	int p1Silver;
	int p1Gold;
	int p1Estate;
	int p1Duchy;
	int p1Province;
	int p1Curse;
	int p1Adventurer;
	int p1Bureaucrat;
	int p1Cellar;
	int p1Chancellor;
	int p1Chapel;
	int p1Councilroom;
	int p1Feast;
	int p1Festival;
	int p1Gardens;
	int p1Laboratory;
	int p1Library;
	int p1Market;
	int p1Militia;
	int p1Mine;
	int p1Moat;
	int p1Moneylender;
	int p1Remodel;
	int p1Smithy;
	int p1Spy;
	int p1Thief;
	int p1Throneroom;
	int p1Village;
	int p1Witch;
	int p1Woodcutter;
	int p1Workshop;

	int p2Copper;
	int p2Silver;
	int p2Gold;
	int p2Estate;
	int p2Duchy;
	int p2Province;
	int p2Curse;
	int p2Adventurer;
	int p2Bureaucrat;
	int p2Cellar;
	int p2Chancellor;
	int p2Chapel;
	int p2Councilroom;
	int p2Feast;
	int p2Festival;
	int p2Gardens;
	int p2Laboratory;
	int p2Library;
	int p2Market;
	int p2Militia;
	int p2Mine;
	int p2Moat;
	int p2Moneylender;
	int p2Remodel;
	int p2Smithy;
	int p2Spy;
	int p2Thief;
	int p2Throneroom;
	int p2Village;
	int p2Witch;
	int p2Woodcutter;
	int p2Workshop;
};

class Game
{
public:
	GameState gameState;
	CardManager cardManager;
	Player players[PLAYERS];
	std::string logString;
	std::vector<LogItem> turnLog;
	std::vector<Move> moveHistory;

	void initialize();
	void play();
	void writeToFile(std::string outputFileName);
	void writeMoveHistoryToFile(std::string outputFileName);

	void writeCSVLog(std::string outputFileName);
	LogItem createLogItem(GameState* g);

	Game();
};