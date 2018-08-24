#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <vector>

using namespace std;
 
int main(int argc, char *argv[]){
	char ch1 = 'a';
	char ch2 = 'Z';
	char ch3 = '\n';
	char ch0 = '\0';
	int i1 = 10;
	int i0 = 0;
	int im = -2;
	cout << (bool)ch1 << " " << (bool)ch2 << " " << (bool)ch3 << " " << (bool)ch0 << endl;
	cout << (bool)i1 << " " << (bool)i0 << " " << (bool)im << endl;
}
