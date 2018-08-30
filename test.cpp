#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <vector>
#include <deque>

using namespace std;
 
deque<int>* Test(int num){
	int i;
	deque<int> *deq_test = new deque<int>;
	for(i = 0; i < num; i++){
		deq_test->push_front(i);
	}
	return deq_test;
}

int main(int argc, char *argv[]){
	deque<int> deq1(1);
	deque<int> *deq2;
	int *test;
	
	cout << sizeof(deq2) << endl;

	deq2 = Test(3);

	cout << sizeof(deq1) << endl;
	cout << sizeof(deq2) << endl;
	cout << "+++++++++" << endl;

	cout << deq1.size() << " " << deq2->size() << endl;

	cout << "~~~~~~" << endl;
	cout << test << " " << sizeof(test) << endl;
	test = new int(10);
	cout << *test << " " << test << " " << sizeof(test) << endl;
}
