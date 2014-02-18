#pragma once

#include <vector>

#include "Node.h"
#include "GameState.h"
#include "Option.h"


class UCT2
{
private:
	std::vector<Node*> emptyNodes;
	std::vector<Node*> usedNodes;
	CardManager cardManager;

	Node* requestNewNode();
	Node* UCTSelectChild(Node* rootNode);
	void resetNodes();
	void doMove(GameState& gameState, Option option);

public:
	int simulations;

	Option getNextOption(GameState rootState, int maximumIterations);
	std::vector<Option> UCT2::getBuyOptions(const GameState& gameState);
	void initialize(CardManager& cardManager, int simulations2, std::vector<Node*>& emptyNodePtrs);
};






/*
def UCT(rootstate, itermax, verbose = False):
""" Conduct a UCT search for itermax iterations starting from rootstate.
Return the best move from the rootstate.
Assumes 2 alternating players (player 1 starts), with game results in the range [0.0, 1.0]."""

rootnode = Node(state = rootstate)

for i in range(itermax):
node = rootnode
state = rootstate.Clone()

# Select
while node.untriedMoves == [] and node.childNodes != []: # node is fully expanded and non-terminal
node = node.UCTSelectChild()
state.DoMove(node.move)

# Expand
if node.untriedMoves != []: # if we can expand (i.e. state/node is non-terminal)
m = random.choice(node.untriedMoves)
state.DoMove(m)
node = node.AddChild(m,state) # add child and descend tree

# Rollout - this can often be made orders of magnitude quicker using a state.GetRandomMove() function
while state.GetMoves() != []: # while state is non-terminal
state.DoMove(random.choice(state.GetMoves()))

# Backpropagate
while node != None: # backpropagate from the expanded node and work back to the root node
node.Update(state.GetResult(node.playerJustMoved)) # state is terminal. Update node with result from POV of node.playerJustMoved
node = node.parentNode

# Output some information about the tree - can be omitted
if (verbose): print rootnode.TreeToString(0)
else: print rootnode.ChildrenToString()

return sorted(rootnode.childNodes, key = lambda c: c.visits)[-1].move # return the move that was most visited



*/