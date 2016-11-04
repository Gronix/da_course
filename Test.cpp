#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <vector>

using namespace std;

list<string> f(){
	list<string> dummy;
	return dummy;
}

int main(){	

/*
	string *array[2];
	array[0] = new string("$");
	
	vector<int> vec;
	vec.push_back(1);
	vec.push_back(2);
	
	//array[1] = (string*)vec[2];
	array[1] = (string*)&vec.back();
	
	cout << *array[0] << ' ' << *(int*)array[1] << '\n';
	
	//array[0] = string("one");
	//array[1] = vec[0];
	
	//cout << array[0] << " " << array[1] << '\n';
*/


	ifstream fdict ("count_1w.txt");
	//ifstream fdict ("anc_dict.txt");
	
	if(!fdict.is_open()){
		cout << "Error Bro: can't open file, help me, pls\n";
		return 2;
	}

	//while(getline(fdict,line)){
	int delim, nxtDelim;
	string line;
	for(int i = 0; i < 10; i++){
		getline(fdict,line);
		delim = line.find(9);
		cout << line.substr(0,delim);
		//
		// WARNING!!! : BELOW IS CONTEXT-SENSITIVE REALIZATION!!!
		//
		nxtDelim = delim;
		do{
			delim = nxtDelim;
			nxtDelim = line.find('\t',nxtDelim + 1);
		}while(0 <= nxtDelim);
		
		cout << ' ' << line.substr(delim + 1) << '\n';
	}
	
	
	fdict.close();
	

	return 0;
}
