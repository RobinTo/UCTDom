#include "tree.h"

tree::tree()
{

}

tree::tree(treeNode* initialNode)
{
	initial = *initialNode;
	initial.isRoot = true;
	initial.parentNodePtr = NULL;
}