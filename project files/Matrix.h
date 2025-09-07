#pragma once
#include <random>
#include <iostream>
#include <iomanip>
#include <windows.h>  
using namespace std;


class Matrix
{
public:
	int sizeX;
	int sizeY;
	vector<vector<int>> matrix;

	// constructor of class Matrix, which stores X and Y size but does not fill in matrix data
	Matrix(int x, int y)
	{
		sizeX = x;
		sizeY = y;
	}

	// simple function that fills a matrix wnadomly with 1 and 0. 
	void fill()
	{
		for (int i = 0; i < sizeY; i++)
		{
			vector<int> row;
			matrix.push_back(row);
			for (int k = 0; k < sizeX; k++)
			{
				matrix[i].push_back(rand() % 2);
			}
		}
	}

	// functions that simplify the generation of colored text, to be used to improve clarity of results
	static void greenType(const std::string& input) {
		// ANSI escape code for green text
		std::cout << "\033[32m" << input << "\033[0m";
	}

	static void blueType(const std::string& input) {
		// ANSI escape code for green text
		std::cout << "\033[34m" << input << "\033[0m";
	}

	static void redType(const std::string& input) {
		// ANSI escape code for green text
		std::cout << "\033[31m" << input << "\033[0m";
	}

	// Fuction that prints an input matrix. Has an optional field for a vector of islands so that islands can be visuallized
	void printMatrix(vector<vector<pair<int, int>>> islands = {})
	{
		// identify the largest island, if possible. It will be printed using a different color than the rest
		auto largestIsland = getLargestIsland(islands);

		// Iterate through entire matix with nested for loop, the outer loop uses the matrix Y dimension, but decrements down to 1 instead of the usual increment
		// because the matrix is printed top down, and this way the value of i matches the Y index of the row being printed. 
		for (int i = sizeY; i >= 1; i--)
		{
			// set output width for Y index so both 1 and 2 digit values are evenly printed
			cout << setw(3) << i; cout << "   ";

			// In the inner loop the iterator is incremented normally because printing is already done left to right
			for (int k = 1; k <= sizeX; k++)
			{

				// check if a particular coordinate is in an island, the largest island, or not part of an island, and print using an appropriately unique color 
				if (containsPair(largestIsland, { k, i }))
				{
					cout << " ";
					if (get(k, i) == 1) blueType("1");
					else greenType("0");
					cout << " ";
				}
				else if (containsPair(islands, { k, i }))
				{
					cout << " ";
					if (get(k, i) == 1) greenType("1");
					else greenType("0");
					cout << " ";
				}
				else 
				{
					cout << " " << get(k, i) << " ";
				}
			}
			cout << "\n";
		}
		cout << "\n     ";

		// For the last line of the printed matrix, print out an index for the X axis
		for (int k = 0; k < sizeX; k++)
		{
			cout << setw(3) << k + 1;
		}
		cout << "\n\n";

		// report the size of the largest island
		if (islands.size() > 0)
		{
			cout << "Size of largest island (blue) = " << largestIsland[0].size();
			cout << "\n\n";
		}
	}

	// fuction to retrieve that value of a given matrix coordinate, optionally printing it 
	int get(int x, int y, bool print = false)
	{

		// because indexes start at 0, subtract 1 from the X and Y target integer values
		int targetY = y - 1;
		int targetX = x - 1;

		// check for illegal target values by comparing them to minimum and maximum size of given matrix 
		if (targetY >= sizeY || targetX >= sizeX || targetY < 0 || targetX < 0)
		{
			return -1;
		}
		else
		{
			if (print)
			{
				cout << "targetX = " << targetX << "\n";
				cout << "targetY = " << targetY << "\n";
			}

			// it is important to return [Y][X] and not [X][Y], because the row (Y), must be speciifed before the column (X)
			return matrix[targetY][targetX];
		}
	}

	// function that given a target coordinate, will search the matrix for any immediately adjacent cells (up, down, left, right) of the same value, and returns a vector of these new coordinates in a vector
	vector<pair<int, int>> inspect(int x, int y)
	{

		// initialize an output vector of pairs to store results. A vector of pairs can be thought of as a "patch"
		vector<pair<int, int>> patch;
		int target = get(x, y);

		// create vector of pairs to store all the coordinates to be searched 
		vector<pair<int, int>> searchCoords = { make_pair(x, y), make_pair(x, y + 1), make_pair(x - 1, y), make_pair(x + 1, y), make_pair(x, y - 1) };

		// iterate through search coordinates and add those that contain the target to the output vector 
		for (int i = 0; i < searchCoords.size(); i++)
		{
			int searchX = searchCoords[i].first;
			int searchY = searchCoords[i].second;
			if (target == get(searchX, searchY))
			{
				patch.push_back(searchCoords[i]);
				//cout << "new point found: " << searchX << ", " << searchY << "\n";
			}
		}
		return patch;
	}

	// matrix wide implementation of the inspect method. Iterates through every matrix coordinate and returns all corresponding patches
	vector<vector<pair<int, int>>> findPatches(int target = 1)
	{
		// initialize output container
		vector<vector<pair<int, int>>> patches;

		for (int y = 1; y <= sizeY; y++)
		{
			for (int x = 1; x <= sizeX; x++)
			{
				if (get(x, y) == target)
				{
					auto patch = inspect(x, y);
					patches.push_back(patch);
				}
			}
		}
		return patches;
	}

	// function to check if a given pair is in a given vector, practically this returns true if a coordinate is in a given island. 
	bool containsPair(const vector<vector<pair<int, int>>>& vec, const pair<int, int>& target) 
	{
		return any_of(vec.begin(), vec.end(), [&](const vector<pair<int, int>>& innerVec) 
			{
				return find(innerVec.begin(), innerVec.end(), target) != innerVec.end();
			});
	}

	// function to check if two vectors of pairs, or patches, shair at least one pair (coordinate). 
	static bool hasMatch(vector<pair<int, int>> patch1, vector<pair<int, int>> patch2)
	{
		return std::find_first_of(patch1.begin(), patch1.end(), patch2.begin(), patch2.end()) != patch1.end();
	}

	// function to combine two vectors of pairs
	static vector<pair<int, int>> combine(vector<pair<int, int>> patch1, vector<pair<int, int>> patch2)
	{
		// initialize output container
		vector<pair<int, int>> combined;

		// insert both input vectors into the output vector via indexes
		combined.insert(combined.end(), patch1.begin(), patch1.end());
		combined.insert(combined.end(), patch2.begin(), patch2.end());

		return combined;
	}

	// function to remove duplicates from a vector of pairs
	static vector<pair<int, int>> removeDuplicates(vector<pair<int, int>> patch)
	{
		vector<pair<int, int>> output = patch;
		// first, sort the vector. Vectors of pairs are conveniently sorted by the first, then second value of the pair. 
		sort(output.begin(), output.end());

		// next, you can create an iterator at the spot beyond which all the non-unique elements are dumped by the unique function. 
		vector<pair<int, int>>::iterator itr = unique(output.begin(), output.end());

		// lastly, erase everything between the element the iterator points to, and the pointer to the last element
		output.erase(itr, output.end());

		return output;
	}

	// Where the magic happens. Takes a vector of vectors of pairs, representing all patches found in the matrix.
	// Combines all patches that share at least one coordinate, until only patches with unique coordinates remain. 
	// Each remaining patch can then be considered an island. 
	static vector<vector<pair<int, int>>> generateIslands(vector<vector<pair<int, int>>> patches)
	{

		// initialize output container 
		vector<vector<pair<int, int>>> islands = patches;

		// keep track of the number of combines performed. This is not vital to the function but an interesting insight
		int combined = 0;

		// to compare all patches to eachother, delcare two iterators, i and k that both start at 0
		for (int i = 0; i < islands.size(); i++)
		{
			for (int k = 0; k < islands.size(); k++)
			{
				// if i and k are equal, skip the comparison because there is no use in comparing a patch to itself 
				if (i == k)
				{
					continue;
				}

				// if i and k are not equal, check for duplicate coordinates in the patches of indexes i and k
				// no action is needed if no match is found, if hasMatch returns false
				if (hasMatch( islands[i], islands[k]) )
				{
					// create a new patch that stores the combination of the two patches in question
					auto combinedPatch = combine(islands[i], islands[k]);

					// erase both of the patches from the main vector, erasing the one with the heigher index first in order to not shift the index of erasure for the lower index patch. 
					if (k > i) {
						islands.erase(islands.begin() + k);
						islands.erase(islands.begin() + i);
					}
					else {
						islands.erase(islands.begin() + i);
						islands.erase(islands.begin() + k);
					}

					// remove dupliates from the combined patch and insert it into the back of the main vector
					islands.push_back(removeDuplicates(combinedPatch));

					// once a combine and replace is performed successfully, reset i to 0, because the indexes of all remaining elements are now backshifted  
					i = 0;
					combined++;
				}
			}
		}
		cout << combined << " combines performed\n";
		return islands;
	}


	// finds largest islands in a vector of islands 
	static vector<vector<pair<int, int>>> getLargestIsland(vector<vector<pair<int, int>>> islands)
	{
		vector<vector<pair<int, int>>> largestIsland;
		for (auto island : islands)
		{
			if (island.size() > largestIsland.size())
			{
				largestIsland.clear();
				largestIsland.push_back(island);
			}
		}		
		return largestIsland;
	}

	// prints all islands in a given matrix. for debugging purposes
	static void printIslands(vector<vector<pair<int, int>>> islands)
	{
		cout << "Printing islands...\n\n";
		int counter = 1;
		for (auto island : islands)
		{
			cout << "Island #" << counter << ":\n";
			for (auto pair : island)
			{
				cout << pair.first << ", " << pair.second << "\n";
			}
			counter++;
			cout << "\n";
		}
	}
};
