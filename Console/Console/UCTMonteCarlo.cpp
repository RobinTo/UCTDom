#include "UCTMonteCarlo.h"
#include <list>
#include <math.h>
#include <stdlib.h>
#include "treasureCard.h"

UCTMonteCarlo::UCTMonteCarlo(game g)
{
	g = g;
	treeNode newNode;
	tree t(newNode);
}

void UCTMonteCarlo::rollout()
{
	for (int i = 0; i < 500; i++)
	{
		treeNode n = selectBestLeaf(t.initial);

		//for each in get options()

		int currentMoney = 3;
		std::list<card*> hand = g.players[g.playerTurn].hand;
		std::list<card*>::const_iterator iterator;
		//for (iterator = hand.begin(); iterator != hand.end(); ++iterator)
		//{
		//	if (((*iterator))->name == "Copper" || ((*iterator))->name == "Silver" || ((*iterator))->name == "Gold")
		//	{
				//treasureCard tc = *iterator;

				//card* c_ptr = *iterator;
				//card c = *c_ptr;
				//treasureCard tc = dynamic_cast<treasureCard>(c);



				//card* c_ptr = &cat;
				//treasureCard* t_ptr = dynamic_cast<treasureCard*>(c_ptr); 
		//	}
			
		//}
		std::list<card*> options = g.getOptions(currentMoney);
		std::list<card*>::const_iterator iterator;
		for (iterator = options.begin(); iterator != options.end(); ++iterator)
		{
			//Ad child
			//Simulate

		}



		propagate();
	}
}

double simulate()
{
	//While game not finished
		//Buy Random
		//Endturn
		//Draw Cards
		
	//return hHeuristicValue
}


treeNode UCTMonteCarlo::selectBestLeaf(treeNode ofNode)
{
	std::list<treeNode> childs = ofNode.childNodes;
	
	std::list<treeNode>::const_iterator iterator;
	double bestValue = 0.0;
	double thisValue = 0.0;
	treeNode bestNode;

	for (iterator = childs.begin(); iterator != childs.end(); ++iterator) {
		thisValue = (*iterator).value + 1 * sqrt(log( (double)((*iterator).parentNode)->visited) / (double)((*iterator).visited));
		if (thisValue > bestValue || bestValue == 0.0)
		{
			bestValue = thisValue;
			bestNode = (*iterator);
		}
	}

	if(bestNode.childNodes.size() > 0)
	{
		return selectBestLeaf(bestNode);
	}
	else
	{
		return bestNode;
	}
}

void UCTMonteCarlo::propagate()
{

}