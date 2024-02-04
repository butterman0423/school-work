/*******************************************************************************
 * Filename: student.cpp
 * Author  : Nathaniel Escaro 
 * Version : 1.0
 * Date    : September 12, 2023
 * Description: Accepts a list of students and their info and prints any student that is failing.
 * Pledge  : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

class Student {
	private:
		string first_;
		string last_;
		float gpa_;
		int id_;

	public:
		Student(string first, string last, float gpa, int id) : first_{first}, last_{last}, gpa_{gpa}, id_{id} {}

		string full_name() const {
			return first_ + " " + last_;
		}

		int id() const {
			return id_;
		}

		float gpa() const {
			return gpa_;
		}

		void print_info() const {
			cout << full_name();
			cout << ", GPA: " << fixed << setprecision(2) << gpa_; 
			cout << ", ID: " << id_ << endl;
		}
};


vector<Student> find_failing_students(const vector<Student>&);
void print_students(const vector<Student>&);

int main() {
	string first_name, last_name;
	float gpa;
	int id;
	char repeat;
	vector<Student> students;
	do {
		cout << "Enter student's first name: ";
		cin >> first_name;
		cout << "Enter student's last name: ";
		cin >> last_name;
		gpa = -1;
		while (gpa < 0 || gpa > 4) {
			cout << "Enter student's GPA (0.0-4.0): ";
			cin >> gpa;
		}
		cout << "Enter student's ID: ";
		cin >> id;
		students.push_back(Student(first_name, last_name, gpa, id));
		cout << "Add another student to database (Y/N)? ";
		cin >> repeat;
	} while (repeat == 'Y' || repeat == 'y');
	
	cout << endl << "All students:" << endl;
	print_students(students);
	
	cout << endl << "Failing students:";


	// TODO
	// Print a space and the word 'None' on the same line if no students are failing.
	// Otherwise, print each failing student on a separate line.
	
	vector<Student> failedStudents = find_failing_students(students);
	
	if(failedStudents.size() == (size_t) 0) {
		cout << " None";
		return 0;
	}
	
	cout << endl; // Start on new line
	print_students(failedStudents);
	
	return 0;
}

vector<Student> find_failing_students(const vector<Student> &students) {
	vector<Student> failing;

	for(const Student &student : students) {
		if(student.gpa() < 1.0) {
			failing.push_back(student);
		}
	}

	return failing;
}

void print_students(const vector<Student> &students) {
	for(const Student &student : students) {
		student.print_info();
	}
}
