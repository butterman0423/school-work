/*******************************************************************************
 * Name        : stairclimber.cpp
 * Author      : Nathaniel Escaro 
 * Date        : 10/7/2023
 * Description : Lists the number of ways to climb n stairs.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

vector< vector<int> > get_ways(int num_stairs) {
	vector< vector<int> > ways;
	
	// Return an empty nested array if no stairs are there
	if(num_stairs == 0) {
		vector<int> empty;
		ways.push_back(empty);
		return ways;
	}
	
	for(int i = 1; i <= min(3, num_stairs); i++) {
		// Fetch the number ways in the previous amount
		vector< vector<int> > prevWays = get_ways(num_stairs - i);
		
		for(vector<int> &nway : prevWays) {
			nway.insert( nway.begin(), i ); // Prepend i to all previous ways
			ways.push_back(nway); // Push into resulting array
		}
	}

	return ways;
}

void display_ways(const vector< vector<int> > &ways) {
	// Get the maxmium digit width that needs to be accounted
	int lstWidth = 0;
	size_t tmpSize = ways.size();
	
	while(tmpSize != 0) {
		lstWidth++;
		tmpSize /= 10;
	}
	
	// Print all ways
	for(size_t i = 0; i < ways.size(); i++) {
		cout << setw(lstWidth) << i + 1 << ". [";
		const vector<int> nway = ways[i];

		for(size_t j = 0; j < nway.size() - 1; j++) {
			cout << nway[j] << ", ";
		}

		cout << nway.back() << ']' << endl;
	}
}

int main(int argc, char * const argv[]) {
	istringstream iss;
	
	// Check if argument count is correct	
	if( argc != 2 ) {
		cerr << "Usage: ./stairclimber <number of stairs>" << endl;
		return 1;
	}

	int numStairs;
	
	// Check if number of stairs is valid
	iss.str(argv[1]);
	if( !(iss >> numStairs) || numStairs <= 0 ) {
		cerr << "Error: Number of stairs must be a positive integer." << endl;
		return 1;
	}
	
	// Retrieve the number of ways to climb
	vector< vector<int> > ways = get_ways(numStairs);
	
	// Display the result
	if( ways.size() == 1 ) {
		cout << "1 way to climb 1 stair." << endl;
	}
	else {
		cout << ways.size() << " ways to climb " << numStairs << " stairs." << endl;
	}
	
	display_ways(ways);

	return 0;
}
