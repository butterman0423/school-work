/*******************************************************************************
 * Filename: fastmult.cpp
 * Author  : Nathaniel Escaro
 * Version : 1.0
 * Date    : November 18, 2023
 * Description: Performs Karatsuba Fast Multiplication on two integers.
 * Pledge  : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include <iostream>
#include <string>
using namespace std;

string add(const string &a, const string &b);
string sub(const string &a, const string &b);
string mult(const string &a, const string &b);
void equalize(string &a, string &b);

// Returns the product of integer a and b using
// Karatsuba Fast Multiplication (in string form)
// ASSUMPTION: a and b are equal in length and are powers of 2
// ASSUMPTION: a and b have lengths >= 1
string fmult(const string &a, const string &b) {
	size_t n = a.size();

	// If a len is 1, then b len is 1
	if(n == 1) {
		return mult(a, b);
	}

	size_t half_n = n >> 1;

	string a0 = a.substr(0, half_n);
	string a1 = a.substr(half_n + 1);

	string b0 = b.substr(0, half_n);
	string b1 = b.substr(half_n + 1);

	string c0 = fmult(a0, b0);

	string sumA = add(a1, a0);
	string sumB = add(b1, b0);
	equalize(sumA, sumB); // Num of digits varies

	string c1 = fmult(sumA, sumB);
	string c2 = fmult(a1, b1);
	
	string res;
	res = c2.insert(n, n, '0'); // c2 * B^n
	res = add( res, sub( c1, add(c2, c0) ).insert(n, half_n, '0') ); // + (c1 - c2 - c0) * B^n/2
	res = add( res, c0 );	// + c0
	return res;
}

// -------- MAIN -------- //

int main(int argc, char * argv[]) {
	// Listed: No need to error check
	
	string a = argv[1];
	string b = argv[2];
	
	// Prepend 0s (to closest power of 2)
	size_t max_size = max( a.size(), b.size() );	

	string res = fmult(a, b);
	cout << res << max_size << endl;
	
	return 0;
}

// ---------------------- //

void equalize(string &a, string &b) {
	if(a.size() == b.size()) {
		return;
	}
	
	size_t diff = a.size() - b.size();
	a.insert(0, diff > 0 ? diff : 0, '0');


	diff = b.size() - a.size();
	b.insert(0, diff > 0 ? diff : 0, '0');
}

// Returns the sum of two numbers (in string form)
string add(const string &a, const string &b) {
	string a_mod = a.substr();
	string b_mod = b.substr();
	equalize(a_mod, b_mod);

	int carry = 0;
	size_t i = a_mod.size() - 1;
	string res;

	res.reserve(a_mod.size());

	while(i > 0) {
		int added = ( a_mod[i] - '0' ) + ( b_mod[i] - '0' ) + carry;
		res.assign( to_string(added % 10), i, i + 1 );
		carry = added / 10;
		i--;
	}
	
	if(carry > 0) {
		res.insert(0, 1, (char) carry + '0');
	}

	return res;
}

// Returns the difference of two numbers (in string form)
// ASSUMPTION: a and b are equal in length
// ASSUMPTION: a > b
string sub(const string &a, const string &b) {
	string a_mod = a.substr();
	string b_mod = b.substr();
	equalize(a_mod, b_mod);
	
	int num_carry = 0;
	size_t i = a_mod.size() - 1;
	string res;

	res.reserve(a_mod.size());

	while(i > 0) {
		char diff;

		if(a_mod[i] < b_mod[i]) {
			diff = ( (a_mod[i] + 10) - b_mod[i] );
			num_carry++;
		}
		else {
			diff = ( a_mod[i] - b_mod[i] );
		}

		res.replace( i, i + 1, diff + '0' );
		i--;
	}
	
	// Fix Negatives
	while(num_carry > 0) {
		res.replace( i, i + 1, res[i] - 1 );
		num_carry--;
	}

	return res;
}

// Returns the product of two single digit numbers (in string form)
// ASSUMPTION: a and b only are only between [0 - 9]
// RESULT: Product in string form (2 digits max)
string mult(const string &a, const string &b) {
	int aDigit = (int) ( a.back() - '0' );
	int bDigit = (int) ( b.back() - '0' );

	string res = to_string( aDigit * bDigit );
	return res;
}
