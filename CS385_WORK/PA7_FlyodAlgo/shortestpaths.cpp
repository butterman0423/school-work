/*******************************************************************************
 * Filename: shortestpaths.cpp
 * Author  : Nathaniel Escaro, Saikarthik Mummadisingu
 * Version : 1.0
 * Date    : December 4, 2023
 * Description: Computes the shortest paths of each vertex in a graph text file.
 * Pledge  : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <climits>

using namespace std;

// UINT_MAX to allow INT_MAX distances
const long LIMIT = (long) UINT_MAX;

int len(unsigned int num) {
	int n = 1; // 0 is considered 1 digit
	while(num >= 10) {
		n++;
		num /= 10;
	}

	return n;
}

void display_table(long** const matrix, const string &label,bool use_letters,int num_vertices) 
{
	cout << label << endl;
	long max_val = 0;
	for (int i = 0; i < num_vertices; i++) 
	{
		for (int j = 0; j < num_vertices; j++) 
		{
			long cell = matrix[i][j];
			if (cell < LIMIT && cell > max_val) {
				max_val = matrix[i][j];
			}
		}
	}
	int max_cell_width = use_letters ? len(max_val) : len(max(static_cast<long>(num_vertices), max_val));
	cout << ' ';
	for (int j = 0; j < num_vertices; j++) 
	{
		cout << setw(max_cell_width + 1) << static_cast<char>(j + 'A');
	}
	cout << endl;
	for (int i = 0; i < num_vertices; i++) 
	{
		cout << static_cast<char>(i + 'A');
		for (int j = 0; j < num_vertices; j++) 
		{
			cout << " " << setw(max_cell_width);
			if (matrix[i][j] == LIMIT) 
			{
				cout << "-";
			}
			 else if (use_letters) 
			 {
				cout << static_cast<char>(matrix[i][j] + 'A');
			} else 
			{
				cout << matrix[i][j];
			}
		}	
		cout << endl;
	}
	cout << endl;
}

void delete_table(long **tbl, int size) {
	for(int i = 0; i < size; i++) {
		delete [] tbl[i];
	}

	delete [] tbl;
}

void floyd_alg(long **matrix, int nodes, long **lengtharr, long **backtrack)
{
    for (int k = 0; k < nodes; k++)
    {
        for (int i = 0; i < nodes; i++)
        {
            for (int j = 0; j < nodes; j++)
            {
                if (i == k || j == k){
                    lengtharr[i][j] = lengtharr[i][j];
                }
                else if (i == j){
                    lengtharr[i][j] = 0;
                }
                else{
                    //utilize the intermediate values
                    if (lengtharr[i][k] + lengtharr[k][j] < lengtharr[i][j])
                    {
                        lengtharr[i][j] = lengtharr[i][k] + lengtharr[k][j];
                        backtrack[i][j] = k;                        
                    }
                }
            }
        }
    }
}

string tracePath(int from, int to, long **iverts) {
	long vertex = iverts[from][to];
	
	if(vertex == LIMIT) {
		return string( 1, (char) (to + 'A') );
	}

	return tracePath(from, vertex, iverts) \
		+ " -> " \
		+ tracePath(vertex, to, iverts);
}

int main(int argc, const char *argv[]) {
	//one program name and text file
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
	
	// Modified File Read from PA7 filereader.cpp
    
	ifstream input_file(argv[1]);
    if (!input_file) {
        cerr << "Error: Cannot open file '" << argv[1] << "'." << endl;
        return 1;
    }
    
	input_file.exceptions(ifstream::badbit);
    string line;
	int nodes;
	long ** node_mat;

    try {
		istringstream iss;

		// Read first line
		getline(input_file, line);
		iss.str(line);

		if( !(iss >> nodes) || !(nodes > 0 && nodes <= 26) ) {
			cerr << "Error: Invalid number of vertices '" << line << "' on line 1." << endl;
			return 1;
		}

		// Create matrix
		node_mat = new long*[nodes];
		for(int i = 0; i < nodes; i++) {
			node_mat[i] = new long[nodes];
			for(int j = 0; j < nodes; j++) {
				node_mat[i][j] = i == j ? 0 : LIMIT; // Empty val
			}
		}

		unsigned int nline = 2;

		while (getline(input_file, line)) {
			string data[3];
			int i = 0;
			size_t spos = 0;
			size_t hitpos;

			do {
				hitpos = line.find(' ', spos);
				data[i] = line.substr(spos, hitpos - spos);

				i++;
				spos = hitpos + 1;
			} while(i < 3 && hitpos != string::npos);

			if(i < 3 || hitpos != string::npos) {
				cerr << "Error: Invalid edge data '" << line << "' ";
				cerr << "on line " << nline << '.' << endl;

				delete_table(node_mat, nodes);
				return 1;	
			}

			// Node check
			char start, end;
			long dist;
			
			iss.clear();
			iss.str(data[0]);
			if( !(iss >> start) || start < 'A' || start >= (char) ('A' + nodes) ) {
				cerr << "Error: Starting vertex '" << data[0] << "' on line ";
				cerr << nline << " is not among valid values A-";
				cerr << (char) ('A' + nodes - 1) << "." << endl;
				
				delete_table(node_mat, nodes);
				return 1;
			}

			iss.clear();

			iss.str(data[1]);
			if( !(iss >> end) || end < 'A' || end >= ('A' + nodes) ) {	
				cerr << "Error: Ending vertex '" << data[1] << "' on line ";
				cerr << nline << " is not among valid values A-";
				cerr << (char) ('A' + nodes - 1) << "." << endl;
				
				delete_table(node_mat, nodes);
				return 1;
			}

			iss.clear();
			iss.str(data[2]);
			if( !(iss >> dist) || dist <= 0) {
				cerr << "Error: Invalid edge weight '" << data[2];
				cerr << "' on line " << nline << '.' << endl;
				
				delete_table(node_mat, nodes);
				return 1;
			}

			// Place in table
			node_mat[start - 'A'][end - 'A'] = dist;

			nline++;
		}
        
		input_file.close();
    } catch (const ifstream::failure &f) {
        cerr << "Error: An I/O error occurred reading '" << argv[1] << "'.";
        return 1;
    }
	
    //array for lengths of the shortest paths
    long** lengtharr = new long* [nodes];
    for (int i = 0; i < nodes; i++) {
        lengtharr[i] = new long[nodes];
    }
    for (int i = 0; i < nodes; i++){
        for (int j = 0; j < nodes; j++){
            lengtharr[i][j] = node_mat[i][j];
        }
    }

    //for backtracking the nodes for intermediates
    long** backtrack = new long* [nodes];
    for (int i = 0; i < nodes; i++){
        backtrack[i] = new long[nodes];
    }
    for (int i = 0; i < nodes; i++){
        for (int j = 0; j < nodes; j++){
            backtrack[i][j] = LIMIT;
        }
    }

	floyd_alg(node_mat, nodes, lengtharr, backtrack);

	// Display Tables
	display_table(node_mat, "Distance matrix:", false, nodes);
	display_table(lengtharr, "Path lengths:", false, nodes);
	display_table(backtrack, "Intermediate vertices:", true, nodes);

	// Display Routes
	for(int r = 0; r < nodes; r++) {
		for(int c = 0; c < nodes; c++) {
			// Print intial data
			cout << (char) (r + 'A') << " -> " << (char) (c + 'A') << ", ";
			long dist = lengtharr[r][c];

			if(dist == LIMIT) {
				cout << "distance: infinity, path: none" << endl;
				continue;
			}

			cout << "distance: " << dist << ", path: ";
			
			cout << (char) (r + 'A'); // Initial vertex
			
			// Print path if possible
			if(dist != 0) {
				cout << " -> " << tracePath(r, c, backtrack);
			}

			cout << endl;
		}
	}

	delete_table(node_mat, nodes);
	delete_table(lengtharr, nodes);
	delete_table(backtrack, nodes);

    return 0;
}
