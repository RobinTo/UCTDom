#include <iostream>

int main()
{
	std::cout << "Dominion AI using UCT and Monte Carlo." << std::endl;
	int t;
	std::cin >> t;
	int tempmem = 0;
	while (true)
	{
		std::cout << t << std::endl;
		std::cin >> tempmem;
		t += tempmem;
	}
	return 0;
}