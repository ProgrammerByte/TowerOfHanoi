// TowerOfHanoiSolver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

using namespace std;

int main() {
	cout << "Enter tower height between 1 and 25 inclusive:" << endl;
	int n;
	cin >> n;
	if (n < 1 || n > 25) {
		cout << "Invalid" << endl;
	}
	else {
		cout << "The moves required to move " << n << " discs from peg A to peg C are:" << endl;
		string result = " ";
		char base;
		if (n % 2 == 0) {
			base = 'B';
		}
		else {
			base = 'C';
		}

		for (int x = 0; x < n; x++) {
			//add a new base
			result += 'A';
			result += base;
			for (int i = result.size() - 3; i >= 0; i--) { //reverse everything but change pegs
				if (result[i] == base) {
					result.push_back('A');
				}
				else if (result[i] == 'A') {
					result.push_back(base);
				}
				else {
					result.push_back(result[i]);
				}
			}
			if (base == 'B') {
				base = 'C';
			}
			else {
				base = 'B';
			}
		}
		cout << result << endl;
		cout << "Minimum number of moves is " << result.size() / 3 << endl;
	}
}