// Testing
#define GAMES					100
#define UCT_PRINTTREE			0
#define UCT_PRINTSIMULATIONS	0
#define UCB_PRINTTREE			0
#define UCB_PRINTSIMULATIONS	0

// Experimenting
#define WOODCUTTERMONEY			2 // Normally 2
#define WITCHNERF				0 // Normally 0

// Dominion
#define PLAYERS					2
#define MAXTURNS				999
#define SUPPLYPILESTOGO			5 // (3+2) First number is actual supplypiles to go. Second number is number of zeroes below.
#define FIRSTPLAYERAI			UCTAI
#define	SECONDPLAYERAI			SINGLEWITCH

// AIs
#define UCTAI					0
#define BIGMONEYAI				1
#define HUMANPLAYER				2
#define FLATUCB					3
#define RANDOM					4
#define SINGLEWITCH				5

// Kingdom Cards
#define BUREAUCRATINGAME	0	// Cost: 4
#define FESTIVALINGAME		1	// Cost: 5
#define GARDENSINGAME		1	// Cost: 4
#define LABORATORYINGAME	1	// Cost: 5
#define MARKETINGAME		1	// Cost: 5
#define MONEYLENDERINGAME	1	// Cost: 4
#define REMODELINGAME		1	// Cost: 4
#define SMITHYINGAME		1	// Cost: 4
#define THIEFINGAME			0	// Cost: 4
#define VILLAGEINGAME		1	// Cost: 3
#define WITCHINGAME			1	// Cost: 5
#define WOODCUTTERINGAME	1	// Cost: 3

// UCT
#define UCT_NODESTOALLOCATE			2000000
#define UCT_SIMULATIONS				100000
#define UCT_C						0.7
#define UCT_PERCFACTOR				1		// The factor we multiply the percentage of score with, to play a part in calculating a node's value.
#define UCT_WINPOINT				1		// How many points to give a node's value for winning
#define UCT_LOSEPOINT				0		// How many points to give a node's value for losing
#define UCT_AVERAGEPROPAGATE		1		// Whether to use 'old' averaging- or the 'new' best-propagation
#define UCT_INCLUDESCOREINDRAW		0		// 'Old-system': Use only proability, or multiply with score, in draw-nodes for their value.
#define UCT_PLAYPLUSACTIONSFIRST	1
#define UCT_VISITROOTCHILDRENMIN	1
#define UCT_MINIMUMVISITS			10
#define UCT_THREADITERATIONS		1
#define UCT_THREADS					1
#define UCT_PLAYOUTPOLICY			UCT_EPSILONGREEDY
	#define UCT_RANDOMPLAYOUT			0
	#define UCT_EPSILONGREEDY			1
		#define UCT_EPSILON					15		// Percentage in integer value, so 10 means 0.1, or 10 %
	#define UCT_HEURISTICGREEDY			2


//FLATUCB
#define UCB_NODESTOALLOCATE			50
#define UCB_SIMULATIONS				100000
#define UCB_C						0.7
#define UCB_PERCFACTOR				1		// The factor we multiply the percentage of score with, to play a part in calculating a node's value.
#define UCB_WINPOINT				1		// How many points to give a node's value for winning
#define UCB_LOSEPOINT				0		// How many points to give a node's value for losing
#define UCB_AVERAGEPROPAGATE		1		// Whether to use 'old' averaging- or the 'new' best-propagation
#define UCB_INCLUDESCOREINDRAW		0		// 'Old-system': Use only proability, or multiply with score, in draw-nodes for their value.
#define UCB_PLAYPLUSACTIONSFIRST	1
#define UCB_VISITROOTCHILDRENMIN	1
#define UCB_MINIMUMVISITS			1
#define UCB_THREADITERATIONS		1
#define UCB_THREADS					1
#define UCB_PLAYOUTPOLICY			UCB_EPSILONGREEDY
	#define UCB_RANDOMPLAYOUT			0
	#define UCB_EPSILONGREEDY			1
		#define UCB_EPSILON					15		// Percentage in integer value, so 10 means 0.1, or 10 %
	#define UCB_HEURISTICGREEDY			2