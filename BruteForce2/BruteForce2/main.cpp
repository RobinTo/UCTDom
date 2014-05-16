#include <iostream>
#include <ctime>

#include "BruteForce.h"




int main()
{
	int turns = 3;

	std::cout << "Brute Force Tree" << std::endl;
	
	BruteForce bf;
	std::srand ( unsigned ( std::time(0) ) );
	bf.initialize(3000);
	bf.createTree(turns);
	//bf.serializeTree();
	//bf.loadTree("tree.txt");
	std::cout << "Nodes used: " << bf.handedOutNodes << std::endl;
	//bf.printSmallTreeAccordingToInput();
	bf.printTree(turns);


	int t;
	std::cin >> t;
	return 0;
}



