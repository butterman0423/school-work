/*******************************************************************************
 * Name          : quickselect.cpp
 * Author        : Nathaniel Escaro
 * Pledge        : I pledge my honor that I have abided by the Stevens Honor System.
 * Date          : 10/27/2023
 * Description   : Implements the quickselect algorithm.
 ******************************************************************************/
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>

using namespace std;

size_t lomuto_partition(int array[], size_t left, size_t right) {
    int pivot = array[left];
	size_t lpIdx = left + 1; // Left partition max index
	
	for(size_t i = lpIdx; i <= right; i++) {
		if( array[i] < pivot ) {
			swap(array[i], array[lpIdx]);
			lpIdx++; // Increase partition size
		}
	}
	
	lpIdx--; // Target the last lp value	
	swap(array[left], array[lpIdx]);	

	return lpIdx;
}

int quick_select(int array[], size_t left, size_t right, size_t k) {
	size_t pivIdx = lomuto_partition(array, left, right);
	
	if( k < pivIdx ) {
		return quick_select(array, left, pivIdx - 1, k);
	}

	if( k > pivIdx ) {
		return quick_select(array, pivIdx + 1, right, k);
	}

	return array[pivIdx];
}

int quick_select(int array[], const size_t length, size_t k) {
    return quick_select(array, 0, length - 1, k);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <k>" << endl;
        return 1;
    }

    int k;
    istringstream iss;
    iss.str(argv[1]);
    if ( !(iss >> k) || k <= 0 ) {
        cerr << "Error: Invalid value '" << argv[1] << "' for k." << endl;
        return 1;
    }

    cout << "Enter sequence of integers, each followed by a space: " << flush;
    int value, index = 0;
    vector<int> values;
    string str;
    str.reserve(11);
    char c;
    iss.clear();
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

    int num_values = values.size();
    if (num_values == 0) {
        cerr << "Error: Sequence of integers not received." << endl;
        return 1;
    }
	
	if (k > num_values) {
		cerr << "Error: Cannot find smallest element " << k;
		cerr << " with only " << num_values << " value";
		cerr << ( (num_values > 1) ? "s." : "." ) << endl;  
		return 1;
	}

	// We'll fit k into range [0, k - 1]
	int kSmall = quick_select((int *) &values[0], (size_t) num_values, (size_t) k - 1);
	cout << "Smallest element " << k << ": " << kSmall << endl;

    return 0;
}
