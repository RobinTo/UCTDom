// Testing
#define GAMES				1000
#define PRINTTREE			0	// Possibly is something wrong. Check if hand in rootnode is correct. TODO.
#define PRINTSIMULATIONS	0

// Dominion
#define PLAYERS				2
#define MAXTURNS			40
#define SUPPLYPILESTOGO		5 // (3+2) First number is actual supplypiles to go. Second number is number of zeroes below.

	// Kingdom Cards
#define BUREAUCRATINGAME	0
#define FESTIVALINGAME		1
#define GARDENSINGAME		0
#define LABORATORYINGAME	1
#define MARKETINGAME		1
#define MONEYLENDERINGAME	1
#define REMODELINGAME		1
#define SMITHYINGAME		1
#define THIEFINGAME			1
#define VILLAGEINGAME		1
#define WITCHINGAME			1
#define WOODCUTTERINGAME	1

// UCT
#define NODESTOALLOCATE			12000000
#define SIMULATIONS				100000
#define C						0.8
#define PERCFACTOR				1		// The factor we multiply the percentage of score with, to play a part in calculating a node's value.
#define WINLOSESCORING			1		// Whether or not to give a node more value for winning and losing
#define WINPOINT				1		// How many points to give a node's value for winning
#define LOSEPOINT				0		// How many points to give a node's value for losing
#define AVERAGEPROPAGATE		1		// Whether to use 'old' averaging- or the 'new' best-propagation
#define INCLUDESCOREINDRAW		0		// 'Old-system': Use only proability, or multiply with score, in draw-nodes for their value.
#define EPSILON					0		// Percentage in integer value, so 10 means 0.1, or 10 %
#define PLAYPLUSACTIONSFIRST	0