/*******************************************************************************
 * Name        : unique.cpp
 * Author      : Nathaniel Escaro
 * Date        : 9/30/2023
 * Description : Determining uniqueness of chars with int as bit vector.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <cctype>

using namespace std;

bool is_all_lowercase(const string &s) {
	for(const char c : s) {
		// Check if c is a lowercase letter
		if(c < 'a' || c > 'z') {
			return 0;
		}
	}
	
	return 1;
}

bool all_unique_letters(const string &s) {
	unsigned int bitCharMap = 0;	// Our bit map, LSB is ignored

	for(const unsigned char c : s) {
		int shiftCount = c & 31; 	// 5 bits tell us how far from 'a'
		
		// Check if letter has been recorded in our bit map
		// Return false if yes
		if( (bitCharMap >> shiftCount) & 1 ) {
			return 0;
		}
		
		// Otherwise add it to the map
		bitCharMap |= 1 << shiftCount;
	}
	
	return 1;
}

int main(int argc, char * const argv[]) {
	// Check if number of arguments is enough
	if(argc != 2) {
		cerr << "Usage: ./unique <string>" << endl;
		return 1;
	}
	
	// Check if string is lowercase
	if( !is_all_lowercase(argv[1]) ) {
		cerr << "Error: String must contain only lowercase letters." << endl;
		return 1;
	}
	
	// Perform our uniqueness test
	if( all_unique_letters(argv[1]) ) {
		cout << "All letters are unique." << endl;
	}
	else {
		cout << "Duplicate letters found." << endl;
	}

	return 0;
}
