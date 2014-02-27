#include <iostream>
#include <ctime>

#include "BruteForce.h"




int main()
{
	
	int treeDepth = 3;


	std::cout << "Brute Force Tree" << std::endl;
	
	BruteForce bf;
	std::srand ( unsigned ( std::time(0) ) );
	std::cout << "Initializing, please wait." << std::endl;
	bf.initialize(50000);
	std::cout << "Creating tree." << std::endl;
	//bf.createTree(treeDepth);
	bf.loadTree("tree.txt");
	std::cout << "Done creating tree." << std::endl;
	std::cout << "Nodes used: " << bf.usedNodePtrs.size() << std::endl;
	//bf.printSmallTreeAccordingToInput();
	
	bf.printTree(treeDepth);
	std::cout << "Done printing tree." << std::endl;


	int t;
	std::cin >> t;
	return 0;
}



