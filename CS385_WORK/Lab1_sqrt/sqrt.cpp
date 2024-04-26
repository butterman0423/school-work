/*******************************************************************************
 * Filename: sqrt.cpp
 * Author  : Nathaniel Andre Escaro
 * Version : 1.0
 * Date    : September 6, 2023
 * Description: Computes the square root of a number (with an optional parameter epsilon)
 * Pledge  : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include<limits>
#include<cmath>
#include<iomanip>
#include<iostream>
#include<sstream>
using namespace std;

const double DEFAULT_EPSILON = 0.0000001;
const int PRECISION = 8;

double sqrt(double, double);

int main(int argc, char * argv[]) {
	double num, epsilon;
	istringstream iss;

	epsilon = DEFAULT_EPSILON; // Set Default Epsilon
	
	// Check if argument counts are correct
	if(argc < 2 || argc > 3) {
		cerr << "Usage: " << argv[0] << " <value> [epsilon]" << endl;
		return 1;
	}
	
	// Validate <value>
	iss.str(argv[1]);
	if( !( iss >> num ) ) {
		cerr << "Error: Value argument must be a double." << endl;
		return 1;
	} 
	
	// Validate [epsilon] if given
	if(argv[2] != NULL) {
		iss.clear();
		iss.str(argv[2]);
		
		double newEp;
		if( !( iss >> newEp )  || newEp <= 0 ) {
			cerr << "Error: Epsilon argument must be a positive double." << endl;
			return 1;
		}

		epsilon = newEp;
	}
	
	cout << fixed;
	cout << setprecision(PRECISION) << sqrt(num, epsilon) << endl;
	
	return 0;
}

double sqrt(double num, double epsilon) {
	if(num < 0.0) {
		return numeric_limits<double>::quiet_NaN();
	}

	if(num == 0.0 || num == 1.0) {
		return num;
	}

	double lastGuess;
	double nextGuess = num;

	do {
		lastGuess = nextGuess;
		nextGuess = ( lastGuess + ( num / lastGuess ) ) / 2;
	} while( abs(lastGuess - nextGuess) > epsilon );

	return nextGuess;
}
