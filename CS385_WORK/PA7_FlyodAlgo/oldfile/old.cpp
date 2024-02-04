#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

// Display function provided
void display_table(long** const matrix, const string &label, const bool use_letters = false) {
	cout << label << endl;
	long max_val = 0;
	for (int i = 0; i < num_vertices; i++) {
		for (int j = 0; j < num_vertices; j++) {
			long cell = matrix[i][j];
			if (cell > -1 && cell > max_val) {
				max_val = matrix[i][j];
			}
		}
	}

	int max_cell_width = use_letters ? len(max_val) : len(max(static_cast<long>(num_vertices), max_val));
	cout << ' ';

	for (int j = 0; j < num_vertices; j++) {
		cout << setw(max_cell_width + 1) << static_cast<char>(j + 'A');
	}
	cout << endl;
	
	for (int i = 0; i < num_vertices; i++) {
		cout << static_cast<char>(i + 'A');
		for (int j = 0; j < num_vertices; j++) {
			cout << " " << setw(max_cell_width);
			if (matrix[i][j] == -1) {
				cout << "-";
			} else if (use_letters) {
				cout << static_cast<char>(matrix[i][j] + 'A');
			} else {
				cout << matrix[i][j];
			}
		}
	cout << endl;
	}

	cout << endl;
}

int main(int argc, const char *argv[]) {
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

		iss >> nodes; // Assume that file is formatted correctly

		if( !(nodes >= 0 && nodes <= 26) ) {
			cerr << "Error: Invalid number of vertices '" << nodes << "' on line 1." << endl;
			return 1;
		}

		// Create matrix
		node_mat = new long*[nodes];
		for(int i = 0; i < nodes; i++) {
			node_mat[i] = new long[nodes];
			for(int j = 0; j < nodes; j++) {
				node_mat[i][j] = i == j ? 0 : -1; // Empty val
			}
		}

		iss.clear();
		unsigned int nline = 2;

		while (getline(input_file, line)) {
			string data[3];
			int i = 0;
			size_t spos = 0;

			// Fetch data from line
			
			while(true) {
				size_t hitpos = line.find(' ', spos);
				
				if( (hitpos != npos && i == 3) || (hitpos == npos && i < 3) ) {
					cerr << "Error: Invalid edge data '" << line << "' ";
					cerr << "on line " << nline << '.' << endl;
					return 1;
				}
				if(i == 3) {
					break;
				}

				data[i] = line.substr(spos, hitpos);
				spos = hitpos + 1;
				i++;
			}

			// Node check
			char start, end;
			long dist;

			iss.str(data[0]);
			if( !(iss >> start) || start < 'A' || start > ('A' + nodes) ) {
				cerr << "Error: Starting vertex '" << data[0] << "' on line ";
				cerr << nline << " is not among valid values A-";
				cerr << ('A' + nodes) << "." << endl;
				return 1;
			}

			iss.clear();

			iss.str(data[1]);
			if( !(iss >> end) || end < 'A' || end > ('A' + nodes) ) {	
				cerr << "Error: Ending vertex '" << data[1] << "' on line ";
				cerr << nline << " is not among valid values A-";
				cerr << ('A' + nodes) << "." << endl;
				return 1;
			}

			iss.clear();
			if( !(iss >> dist) || dist <= 0) {
				cerr << "Error: Invalid edge weight '" << data[2];
				cerr << "' on line " << nline << '.' << endl;
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
	
	// TODO ALGORITHM
	

	// Cleanup
	for(int i = 0; i < nodes; i++) {
		for(int j = 0; j < nodes; j++) {
			delete [] node_mat[i][j];
		}

		delete [] node_mat[i];
	}
    return 0;
}
