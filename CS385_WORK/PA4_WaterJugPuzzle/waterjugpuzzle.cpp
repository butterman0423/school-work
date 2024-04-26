/*******************************************************************************
 * Filename: waterjugpuzzle.cpp
 * Author  : Nathaniel Escaro
 * Version : 1.0
 * Date    : October 21, 2023
 * Description: Attempts to solve a given water jug puzzle.
 * Pledge  : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
using namespace std;

struct State {
	int a, b, c;
	string directions;
	State * parent;
	int outdegree;

	State(int _a, int _b, int _c) : 
		a{_a}, b{_b}, c{_c}, directions{""}, parent{nullptr}, outdegree{0} {}
	
	~State() {
		// If this delete causes parent to have no children, delete the parent
		if( this->parent != nullptr && --(this->parent->outdegree) == 0 ) {
			delete this->parent;
		}
	}
	
	void transfer(int, int, State &);
	bool equals(State &) const;
	int get(int) const;
	void set(int, int);
	string to_string() const;
};

bool validate_args(char * [], ostringstream &, int *, int *);

State * solve_water_jug(State &cap, State &goal, bool ** hist) {
	queue<State *> nodes;
	nodes.push( new State(0, 0, cap.c) ); // Push initial state
	
	( nodes.front() )->directions = "Initial state.";

	State * solution = nullptr; // Container for the solution
	hist[0][0] = true; // Mark first state
	
	// Start the search
	while( !nodes.empty() ) {
		State * curr = nodes.front();
		nodes.pop();
		
//		cout << curr->to_string() << endl;

		if( curr->equals(goal) ) {
			solution = curr;
			break;	
		}

		// Push valid child nodes
		for(int i = 0; i <= 2; i++) {
			for(int j = 2; j >= 0; j--) {
				// If same jug, skip
				if( i == j ) continue;
				
				State tmpState(curr->a, curr->b, curr->c);
				tmpState.transfer(j, i, cap);

				// If null state, skip
				if( tmpState.a == -1 ) continue;

				// If state is marked, skip
				if( hist[tmpState.a][tmpState.b] ) continue;
				
				// Mark the state
				hist[tmpState.a][tmpState.b] = true;

				State * childState = new State(tmpState.a, tmpState.b, tmpState.c);
				childState->directions = tmpState.directions;
				childState->parent = curr;

				curr->outdegree++;
				nodes.push(childState);
			}
		}
		
		// Delete curr and its ancestors (if they become islands) if no children were pushed
		if( curr->outdegree == 0 ) {
			delete curr;
		}
	}

	// Delete any island nodes
	while( !nodes.empty() ) {
		delete nodes.front();
		nodes.pop();
	}
	
	return solution;
}

int main(int argc, char * argv[]) {
	if(argc != 7) {
		cerr << "Usage: ./waterjugpuzzle <cap A> <cap B> <cap C> "; 
		cerr << "<goal A> <goal B> <goal C>" << endl; 

		return 1;
	}

	ostringstream oss;
	int caps[3];
	int goals[3];

	if( !validate_args(argv, oss, caps, goals) ) {
		cerr << oss.str() << endl;
		return 1;
	}

	State capStates(caps[0], caps[1], caps[2]);
	State goalStates(goals[0], goals[1], goals[2]);
	bool ** hist = new bool* [caps[0] + 1];

	for(int i = 0; i < caps[0] + 1; i++) {
		hist[i] = new bool[caps[1] + 1];
		for(int j = 0; j < caps[1] + 1; j++) {
			hist[i][j] = false;
		}
	}

	State * solution = solve_water_jug(capStates, goalStates, hist);
	
	for(int i = 0; i < caps[0] + 1; i++) {
		delete [] hist[i];
	}
	delete [] hist;
	
	if( solution == nullptr ) {
		cout << "No solution." << endl;
		return 0;
	}

	vector<State *> path;
	while(solution != nullptr) {
		path.push_back(solution);
		solution = solution->parent;
	}
	
	for(auto iter = path.crbegin(); iter != path.crend(); iter++) {
		State * stateStep = *iter;
		cout << stateStep->directions << " " << stateStep->to_string() << endl;
	}

	delete path.front();

	return 0;
}

// ------------------- HELPER FUNCTION START ---------------------- //

// ----- STRUCT STATE ----- //

void State::transfer(int from, int to, State &capacity) {
	int spaceLeft = capacity.get(to) - this->get(to);
	
	// If jug to fill is full mark as a null sate
	if(spaceLeft == 0) {
		this->set(0, -1);
		return;
	}

	int amountMoved = min(this->get(from), spaceLeft);
	this->set(from, this->get(from) - amountMoved);
	this->set(to, this->get(to) + amountMoved);

	// Make directions
	ostringstream oss;

	oss << "Pour " << amountMoved << " ";
	oss << (amountMoved == 1 ? "gallon " : "gallons ") << "from ";
	oss << (char) (from + 'A') << " to " << (char) (to + 'A') << ".";

	this->directions = oss.str();
}

bool State::equals(State &other) const {
	return this->a == other.a && this->b == other.b && this->c == other.c;
}

int State::get(int idx) const {
	switch(idx) {
		case 0:
			return this->a;
		case 1:
			return this->b;
		case 2:
			return this->c;
	}
	return -1;
}

void State::set(int idx, int val) {
	switch(idx) {
		case 0:
			this->a = val;
			return;
		case 1:
			this->b = val;
			return;
		case 2:
			this->c = val;
			return;
	}
}

string State::to_string() const {
		ostringstream oss;
		oss << '(' << a << ", " << b << ", " << c << ")";
		return oss.str();
	}

// ------------------------ //

bool validate_args(char * argv[], ostringstream &oss, int * caps, int * goals) {
	istringstream iss;
	int goalSum = 0;
	
	for(int i = 1; i <= 3; i++) {
		char slotX = 'A' + i - 1;

		iss.str(argv[i]);
		if( !(iss >> caps[i - 1]) || caps[i - 1] <= 0 ) {
			oss << "Error: Invalid capacity '" << argv[i] << "' for jug " << slotX << ".";
			return false;
		}
		iss.clear();
	}

	for(int i = 4; i <= 6; i++) {
		char slotX = 'A' + i - 4;

		iss.str(argv[i]);
		if( !(iss >> goals[i - 4]) || goals[i - 4] < 0 ) {
			oss << "Error: Invalid goal '" << argv[i] << "' for jug " << slotX << ".";
			return false;
		}
		
		goalSum += goals[i - 4];
		iss.clear();
	}

	for(int i = 0; i < 3; i++) {
		char slotX = 'A' + i;

		if( goals[i] > caps[i] ) {
			oss << "Error: Goal cannot exceed capacity of jug " << slotX << ".";
			return false;
		}
	}
	
	if( caps[2] != goalSum ) {
		oss << "Error: Total gallons in goal state must be equal to the capacity of jug C.";
		return false;
	}

	return true;
}
