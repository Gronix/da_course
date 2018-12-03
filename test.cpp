#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <fstream>
#include <vector>
#include <deque>

//#include <iostream>
//#include <fstream>

using namespace std;

typedef struct{
	double n;
	char *s;
} st;

int main(int argc, char *argv[]){
	list<st*> *l = new list<st*>;

	char *s = new char[4];
	s[0] = 'x';
	s[1] = 'y';
	s[2] = 'u';
	s[3] = '\0';


	st *temp = new st;
	temp->n = 100;
	temp->s = s;
	l->push_front(temp);
	cout << l->size() << endl;
}