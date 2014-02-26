#include <iostream>
#include <ctime>

#include "BruteForce.h"




int main()
{
	
	int treeDepth = 5;


	std::cout << "Brute Force Tree" << std::endl;
	
	BruteForce bf;
	std::srand ( unsigned ( std::time(0) ) );
	std::cout << "Initializing, please wait." << std::endl;
	bf.initialize(5000000);
	std::cout << "Creating tree." << std::endl;
	bf.createTree(treeDepth);
	std::cout << "Done creating tree." << std::endl;
	bf.printSmallTreeAccordingToInput();
	std::cout << "Done printing tree." << std::endl;
	//bf.printTree(treeDepth);

	int t;
	std::cin >> t;
	return 0;
}



