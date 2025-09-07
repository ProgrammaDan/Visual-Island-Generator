/*
Written by Daniel Cohen.This program is my solution to the challenge of finding the largest island of the same number (1 by default),
connected by the cardinal directions (up, down, left, right), in a randomly filled matrix of ones and zeros, with the additional stipulation
that traditional BFS and DFS search methods cannot be used.
The solution works by checking every element in the matrix for adjacent elements of the target value, and saving these "patches" to memory.
Then, an algorithm combines all patches with at least one identical element until only unique islands remain, a result that lends itself to
easy analysis of island size once duplicates are removed.

Practical application: This program visually isolates the largest island generated. Often this island may look quite asthetic and be reminiscent
of actual earthly islands. So, this program may serve as a source of inspiration, or a creative generator for artists or game designers that require
a supply of original islandesque shapes.
*/

#include "generalFunctions.h"
#include "Matrix.h"
#include <iostream>
#include <random>
#include <vector>
using namespace std;
using namespace general;


// driver code 
int main() 
{

	// default parameters
	int x = 25 ;
	int y = 25 ;
	int c = 1;

	// determine user preferences 
	beepBoopType("Use default parameters of (25 x 25 x 1)? y / n\n");
	string input; 
	cin >> input;
	cout << "\n";

	// collect user defined parameters 
	if (input == "n" || input == "N")
	{
		while (true)
		{
			beepBoopType("Enter X size for map (0 - 30): ");
			cin >> x;
			if (x > 0 && x < 31) break;
			else beepBoopType("invalid input\n");
		}

		while (true)
		{
			beepBoopType("Enter Y size for map (0 - 30): ");
			cin >> y;
			if (y > 0 && y < 31) break;
			else beepBoopType("invalid input\n");
		}

		while (true)
		{
			beepBoopType("How many maps to generate? (1 - 10): "); 
			cin >> c;
			if (c > 0 && c < 10) break;
			else beepBoopType("invalid input\n");
		}
	}

	cout << "\n";
	beepBoopType("Generating...\n\n", 100);



	// generate, analyze, and visualize maps
	while (c > 0)
	{
		// generate map of size X * Y
		Matrix map(x, y);

		// fill map with randomly with 1 and 0
		map.fill();

		// present raw map matrix
		map.printMatrix();


		// identify patches in map
		auto patches = map.findPatches(1);

		// generate islands from patches
		auto islands = Matrix::generateIslands(patches);

		// visualize map again, this time with islands colored and largest island shown
		cout << "\nVisualization of Islands: \n\n";
		map.printMatrix(islands);

		// decrement counter 
		c--;
	}

	// fin 
	return 0;
}
