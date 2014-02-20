#include <iostream>
#include <ctime>

#include "BruteForce.h"




int main()
{
	
	int treeDepth = 5;


	std::cout << "Brute Force Tree" << std::endl;
	
	BruteForce bf;
	std::srand ( unsigned ( std::time(0) ) );
	bf.initialize(5000000);
	bf.createTree(treeDepth);
	bf.printTree(treeDepth);


	std::cout << "Done printing tree" << std::endl;

	int t;
	std::cin >> t;
	return 0;
}