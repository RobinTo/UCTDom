#include <iostream>
#include <ctime>

#include "BruteForce.h"




int main()
{
	
	int treeDepth = 3;


	std::cout << "Brute Force Tree" << std::endl;
	
	BruteForce bf;
	std::srand ( unsigned ( std::time(0) ) );
	bf.initialize(5000);
	bf.createTree(treeDepth);
	bf.printTree();

	int t;
	std::cin >> t;
	return 0;
}