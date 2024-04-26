/*******************************************************************************
 * Name        : sieve.cpp
 * Author      : Nathaniel Escaro
 * Date        : September 12, 2023
 * Description : Sieve of Eratosthenes
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

class PrimesSieve {
public:
    PrimesSieve(int limit);

    ~PrimesSieve() {
        delete [] is_prime_;
    }

    void display_primes() const;

private:
    // Instance variables
    bool * const is_prime_;
    const int limit_;
    int num_primes_, max_prime_;

    // Method declarations
    void sieve();
    static int num_digits(int num);
};

PrimesSieve::PrimesSieve(int limit) :
        is_prime_{new bool[limit + 1]}, limit_{limit} {
    sieve();
}

void PrimesSieve::display_primes() const {
	const int max_prime_width = num_digits(max_prime_);
	const int primes_per_row = 80 / (max_prime_width + 1);
	
	// Write our headers
	cout << "Number of primes found: " << num_primes_ << endl;
	cout << "Primes up to " << limit_ << ':' << endl;

	int rowSize = 0;
	int numWidth = num_primes_ <= primes_per_row ? 1 : max_prime_width;
	
	for(int i = 2; i <= limit_; i++) {
		if(!is_prime_[i]) { continue; } // Skip all non-prime nums
		
		//If size == 0: print num without space
		//If 0 < size < primes_per_row: print num with LEADING space
		//If size == primes_per_row: add endl and follow first condition [size == 0]
		
		if(rowSize == primes_per_row) {
			cout << endl;
			rowSize = 0;
		}

		if(rowSize == 0) {
			cout << setw(numWidth) << i;
		}
		else {
			cout << ' ' << setw(numWidth) << i;
		}

		rowSize++;
	}

	cout << endl;
}

void PrimesSieve::sieve() {
	for(int i = 0; i <= limit_; i++) {
		is_prime_[i] = true;
	}

	num_primes_ = limit_ - 1; // n - 1 since 1 is not counted
	
	int sqrtN = sqrt(limit_);
	
	// 0 = prime, 1 = not prime
	for(int i = 2; i <= sqrtN; i++) {
		if(is_prime_[i]) {
			for(int j = i * i; j <= limit_; j += i) {
				if(is_prime_[j]) {
					is_prime_[j] = false;
					num_primes_--;
				}
			}
		}
	}

	// Find our max prime
	
	for(int i = limit_; i >= 2; i--) {
		if(is_prime_[i]) {
			max_prime_ = i;
			break;
		}
	}
}

int PrimesSieve::num_digits(int num) {	
	int digits = 0;
	while(num != 0) {
		num /= 10;
		digits++;
	}
	
	return digits;
}

int main() {
    cout << "**************************** " <<  "Sieve of Eratosthenes" <<
            " ****************************" << endl;
    cout << "Search for primes up to: ";
    string limit_str;
    cin >> limit_str;
    int limit;

    // Use stringstream for conversion. Don't forget to #include <sstream>
    istringstream iss(limit_str);

    // Check for error.
    if ( !(iss >> limit) ) {
        cerr << "Error: Input is not an integer." << endl;
        return 1;
    }
    if (limit < 2) {
        cerr << "Error: Input must be an integer >= 2." << endl;
        return 1;
    }

	cout << endl; // Add our whitespace

	PrimesSieve userSieve(limit);
	userSieve.display_primes(); // Print the display

    return 0;
}
