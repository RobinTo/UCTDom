#include "card.h"
#include <vector>

#ifndef __TREENODE_H__
#define __TREENODE_H__

class treeNode
{
public:
	treeNode();
	double value;
	int timesPropagated;
	double totalValue;
	card* cardOption;
	int timesChosen;
	std::vector<treeNode*> childNodes;
	treeNode* parent;
	bool isTop;
	int treeDepth;

	void setParent(treeNode* n);
	treeNode* appendChild(card* cardOption, treeNode* parent);
};

#endif