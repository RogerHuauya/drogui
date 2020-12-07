
#include <iostream>
using namespace std;

int main(void) {

    char name = "r";
    int col = 3, row = 3;

	for(int i = 0 ;i < row ; i++){
		cout << "syms ";
		for(int j = 0 ;j < col ; j++)
			cout << name<<i<<j << " ";
		cout << " real" << endl;
	}	
	
	cout << toupper(name) << " = [";	
	for(int i = 0 ;i < row ; i++){
		cout << "\t\t";
		for(int j = 0 ;j < col ; j++)
			cout << name<<i<<j << " ";
		cout << (i<row-1?";":"") << endl;
	}
	cout << "];\n";
	
	
	// your code goes here
	return 0;
}