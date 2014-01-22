#include "treeNode.h"

treeNode::treeNode()
{
	isRoot = false;
}


treeNode::treeNode(bool isRoot2, treeNode* pNodePtr)
{
	if (isRoot2)
	{
		isRoot = true;
		parentNodePtr = pNodePtr;
	}
	else
	{
		parentNodePtr = NULL;
	}
}

void treeNode::appendChild(treeNode* childNodePtr)
{
	childNodePtrs.push_back(childNodePtr);
}
