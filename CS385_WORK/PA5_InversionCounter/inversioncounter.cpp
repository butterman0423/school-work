/*******************************************************************************
 * Name        : inversioncounter.cpp
 * Author      : Nathaniel Escaro
 * Version     : 1.0
 * Date        : 11/4/2023
 * Description : Counts the number of inversions in an array.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cctype>
#include <cstring>

using namespace std;

// Function prototype.
static long mergesort(int array[], int scratch[], int low, int high);

/**
 * Counts the number of inversions in an array in Theta(n^2) time using two nested loops.
 */
long count_inversions_slow(int array[], int length) {
    long inverts = 0;

	for(int i = 0; i < length - 1; i++) {
		for(int j = i + 1; j < length; j++) {
			inverts += array[i] > array[j];
		}
	}

	return inverts;
}

/**
 * Counts the number of inversions in an array in Theta(n lg n) time.
 */
long count_inversions_fast(int array[], int length) {
	int * scratch = new int[length];
	long inverts = mergesort(array, scratch, 0, length);
	delete [] scratch;

	return inverts;
}


static long mergesort(int array[], int scratch[], int low, int high) {
    int diff = high - low;

	if(diff == 1) {
		*scratch = array[low]; // Put our initial value in scratch
		return 0;
	}

	int midptn = low + diff / 2;
	int llen = midptn - low;
	int rlen = high - midptn;

	int * lScratch = new int[llen];
	long lMerge = mergesort(array, lScratch, low, midptn);

	int * rScratch = new int[rlen];
	long rMerge = mergesort(array, rScratch, midptn, high);

	// Combine left and right
	int * pLeft = lScratch;
	int * pRight = rScratch;
	
	long swaps = 0;
	
	int l, r;
	while( llen > 0 && rlen > 0 ) {
		l = *pLeft;
		r = *pRight;

		// l will determine swaps
		if(l > r) {
			*scratch = r;
			pRight++;
			rlen--;
			swaps += llen; // Every l element then is > than r
		}
		else {
			*scratch = l;
			pLeft++;
			llen--;
		}

		scratch++;
	}

	int * pRem = pRight;
	int olen = rlen;

	if(llen > 0) {
		pRem = pLeft;
		olen = llen;
	}

	while( olen > 0 ) {
		*scratch = *pRem;
		pRem++;
		olen--;
		scratch++;
	}

	// Free scratch arrays
	delete [] lScratch;
	delete [] rScratch;
	
	return swaps + lMerge + rMerge;
}

int main(int argc, char *argv[]) {
	if(argc > 2) {
		cerr << "Usage: " << argv[0] << " [slow]" << endl;
		return 1;
	}
	
	bool runSlow = argc == 2;	
	if( runSlow && strcmp(argv[1], "slow") != 0 ) {
		cerr << "Error: Unrecognized option '" << argv[1] << "'." << endl;
		return 1;
	}

    cout << "Enter sequence of integers, each followed by a space: " << flush;

    istringstream iss;
    int value, index = 0;
    vector<int> values;
    string str;
    str.reserve(11);
    char c;
    while (true) {
        c = getchar();
        const bool eoln = c == '\r' || c == '\n';
        if (isspace(c) || eoln) {
            if (str.length() > 0) {
                iss.str(str);
                if (iss >> value) {
                    values.push_back(value);
                } else {
                    cerr << "Error: Non-integer value '" << str
                         << "' received at index " << index << "." << endl;
                    return 1;
                }
                iss.clear();
                ++index;
            }
            if (eoln) {
                break;
            }
            str.clear();
        } else {
            str += c;
        }
    }
	
	if(values.size() == 0) {
		cerr << "Error: Sequence of integers not received." << endl;
		return 1;
	}

	long inverts;

	if( runSlow ) {
		inverts = count_inversions_slow(&values[0], index);
		cout << "Number of inversions (slow): " << inverts << endl;
		return 0;
	}

	inverts = count_inversions_fast(&values[0], index);
	cout << "Number of inversions (fast): " << inverts << endl;
    return 0;
}
