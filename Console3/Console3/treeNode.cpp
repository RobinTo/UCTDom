#include "treeNode.h"

treeNode::treeNode()
{
	value = 5.0;
	timesPropagated = 1;
	timesChosen = 1;
	totalValue = 0.0;
	isTop = false;
	treeDepth = 0;
	parent = NULL;
}

void treeNode::setParent(treeNode* n)
{
	parent = n;
	treeDepth = parent->treeDepth+1;
}

treeNode* treeNode::appendChild(card* cardOption, treeNode* parent)
{
	treeNode* newNode = new treeNode();
	newNode->cardOption = cardOption;
	newNode->setParent(parent);
	childNodes.push_back(newNode);
	return newNode;
}