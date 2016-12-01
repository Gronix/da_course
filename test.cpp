#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <vector>

char **domainZones3len;
char **domainZones2len;

using namespace std;


int strLen(const char *str){
	int len = -1;
	while(str[++len]);
	return len;
}

	
char* strCpyCreate(const char *origin, int start = 0, int len = 0){
	int plen = strLen(origin);
	char *res;
	if(len == 0){
		res = new char[plen - start + 1];
	}else{
		if(plen - len < 0){
			cerr << "Error in strCpy: plen - clen < 0: " << 
				plen << ' ' << len << ' ' << origin << '\n';
			return NULL;
		}
		res = new char[len + 1];
		plen = start + len - 1;
		res[len] = 0;
	}
	while(plen >= start){
		res[plen - start] = origin[plen];
		plen--;
	}
	return res;
}
 
int main(int argc, char *argv[]){
	
/* 	if(argc < 1){
		cerr << "Sorry bro - i need a input string" << endl;
		return 1;
	}
 */	
	char domain[4];
	char dzFileName[] = "domain zones.txt";
	
	ifstream dzFile;
	dzFile.open(dzFileName);
	if(!dzFile.is_open()){
		cerr << "can't open domain zone file!" << endl;
		return 2;
	}
	
	// READ DOMAIN ZONES:
	
	// count len of 'domain zones' file:
	
	int id = 0;
	while(dzFile.getline(domain, 5)){
		if(domain[0] == '-'){
			break;
		}
		id++;
	}
	domainZones3len = new char* [id + 1];
	domainZones3len[id] = NULL;
	id = 0;
	while(dzFile.getline(domain, 5)){
		id++;
	}
	domainZones2len = new char* [id + 1];
	domainZones2len[id] = NULL;
	
	// end read of lenght. Now start read domain zones:
	
	dzFile.clear();
	dzFile.seekg(0);
	while(dzFile.getline(domain, 4)){
		if(domain[0] == '-'){
			break;
		}
		domainZones3len[++id] = strCpyCreate(domain);
		cout << domainZones3len[id] << endl;
	}
	id = -1;
	while(dzFile.getline(domain, 3)){
		domainZones2len[++id] = strCpyCreate(domain);
		cout << domainZones2len[id] << endl;
	}
	dzFile.close();
	// END OF READ DOMAIN ZONES.
	
	return 0;
}