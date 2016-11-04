#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>

#include "BigInt.h"

#define ALPHABET 26 // |Alphabet|
using namespace std;

//typdef struct Trie Trie

class TTrie{
public:
	char c;
	TTrie *child[27]; // child[26] = '$' symbol: end_of_string_char !!! OR COUNT - ITS VERY DIRTY TRICK !!!
	
	TTrie(void){
		c = '_';
		for(int i = 0; i < 27; i++){
			child[i] = NULL;
		}
	};
	
	TTrie(char ch){
		c = ch;
		for(int i = 0; i < 27; i++){
			child[i] = NULL;
		}
	};
	
	void Include(string str, TTrie *count = NULL){
		if(child[_CharId(str[0])]){
			child[_CharId(str[0])]->_IncludeRecursive(0, str, count);
		}else{
			child[_CharId(str[0])] = _CreateBranch(0, str, count);
		}
	};
	
	int Search(string str){
		int i;
		int answer = 0;
		TTrie *node = this;
		
		for(i = 0; i < str.size() - 1; i++){
			node = node->child[_CharId(str[i])];
			if(node == NULL){
				break;
			}
		}
		if(node != NULL && node->child[_CharId(str[i])] != NULL){
			node = node->child[_CharId(str[i])];
			
			if((i == str.size() - 1) and node->child[26]){
				answer = *(int*)(node->child[26]);
			}
		}
		return answer;
	};
	
	void Print(int lvl = 0){
		for(int j = 0; j < lvl; j++){
			cout << "__";
		}
		cout << c;
		if(child[26]){
			cout << '$';
		}
		cout << "\n";
		
		for(int i = 0; i < 26; i++){
			if(child[i]){
				child[i]->Print(lvl + 1);
			}
		}
	}
private:
	void _IncludeRecursive(int pos, string str, TTrie *count){
		if((pos == str.size() - 1)){
			if(child[26]){
				cerr << "DUPLICATE: " << str << " " << str[pos] << " " << pos << "\n";
			}else{
				if(count != NULL){
					child[26] = count; // dirty trick... be careful
				}else{
					child[26] = this;
				}
			}
		}else{
			if(child[_CharId(str[pos + 1])]){
				child[_CharId(str[pos + 1])]->_IncludeRecursive(pos + 1, str, count);
			}else{
				child[_CharId(str[pos + 1])] = _CreateBranch(pos + 1, str, count);
			}
		}
	};

	TTrie* _CreateBranch(int pos, string str, TTrie *count){
		TTrie *root = new TTrie(str[pos]);
		
		if(pos == str.size() - 1){
			if(count != NULL){
				root->child[26] = count; // dirty trick... be careful
			}else{
				root->child[26] = this;
			}
		}else{
			root->child[_CharId(str[pos + 1])] = _CreateBranch(pos + 1, str, count);
		}
		
		return root;
	};
	
	int _CharId(char c){
		if((65 <= c) and (c < 91)){
			c -= 65;
		}else
		if((97 <= c) and (c < 123)){
			c -= 97;
		}
		else{
			cerr << "WRONG CHAR! : " << (int)c << " " << c << "\n";
		}
		return c;
	};
	
	char _ToLow(char c){
		if((65 <= c) and (c <= 90)){
			c += 22;
		}
		return c;
	};
};

int GetInt(string str){
	int i = -1, x = 0;
	while(str[++i]){
		x = str[i] - 48 + x*10;
	}
	return x;
}

int GetInt(char *str){
	int i = -1, x = 0;
	while(str[++i]){
		x = str[i] - 48 + x*10;
	}
	return x;
}


list<string> _SimilarRecursive(string pattern){
	string str;
	list<string> similarList;
	int i, n = pattern.size();
	
	// Transpositions
	if(n > 1){
		char c;
		
		for(i = 0; i < str.size() - 1; i++){
			str = string(pattern);
			c = str[i+1];
			str[i+1] = str[i];
			str[i] = c;
			
			similarList.push_back(str);
			//similarTrie.Include(str);
		}
	}
	similarList.push_back(string("$"));
	// Deletions
	{
		str = string(pattern,0,n - 1);
		similarList.push_back(str);
		//similarTrie.Include(str);
		
		for(i = 1; i < n - 1; i++){
			str = string(pattern,0,i);
			str += string(pattern,i+1,n - str.size() - 1);
			
			similarList.push_back(str);
			//similarTrie.Include(str);
		}
		
		str = string(pattern,1);
		similarList.push_back(str);
		//similarTrie.Include(str);
	}
	similarList.push_back(string("$"));
	// Substitutions
	{
		string part2(pattern,1);
		for(i = 0; i < ALPHABET; i++){
			str = (char)(97 + i) + part2;
			similarList.push_back(str);
		}
		
		string part1;
		for(int j = 1; j < n - 1; j++){ // first and last positions processed separately
			part1 = string(pattern, 0, j);
			part2 = string(pattern, j, n - j - 1);
			for(i = 0; i < ALPHABET; i++){
				str = part1 + (char)(i + 97) + part2;
				similarList.push_back(str);
			}
		}
		
		part1 = string(pattern, 0, n - 1);
		for(i = 0; i < ALPHABET; i++){
			str = part1 + (char)(97 + i);
			similarList.push_back(str);
		}
	}
	similarList.push_back(string("$"));
	// Insertions
	{
		string part2(pattern);
		for(i = 0; i < ALPHABET; i++){
			str = (char)(97 + i) + part2;
			similarList.push_back(str);
		}
		
		string part1;
		for(int j = 1; j < n; j++){ // first and last positions processed separately
			part1 = string(pattern, 0, j);
			part2 = string(pattern, j, n - j);
			for(i = 0; i < ALPHABET; i++){
				str = part1 + (char)(i + 97) + part2;
				similarList.push_back(str);
			}
		}
		
		part1 = string(pattern);
		for(i = 0; i < ALPHABET; i++){
			str = part1 + (char)(97 + i);
			similarList.push_back(str);
		}
	}
	
	return similarList;
}

list<string> _ComplexConcat(list<string> List1, list<string> List2){
	list<string>::iterator str1 = List1.begin();
	list<string>::iterator str2 = List2.begin();
	list<string> List;
	
	for(int j = 0; j < 4; j++){
		for(; *str1 != "$" && str1 != List1.end(); str1++){
			List.push_back(*str1);
		}
		for(; *str2 != "$" && str2 != List2.end(); str2++){
			List.push_back(*str2);
		}
		if(str1 != List1.end() && str2 != List2.end()){
			List.push_back(string("$"));
		}
		str1++;
		str2++;
	}
	List.unique();
	return List;
}

list<string> _UnicueList(list<string> List){
	list<string> ClearList;
	list<string>::iterator str = List.begin(), newS;
	string cstr;
	int j;
	bool brIt;
	for(; str != List.end();){
		for(; *str != "$" && str != List.end(); str++){
			// check for uniquennes:
			brIt = false;
			newS = ClearList.begin();
			for(; newS != ClearList.end() && brIt != true;){
				for(; *newS != "$" && newS != ClearList.end(); newS++){
					if(*newS == *str){
						brIt = true;
						break;
					}
				}
				if(newS != ClearList.end()){
					newS++;
				}
			}
			if(!brIt){
				ClearList.push_back(*str);
			}
		}
		if(str != List.end()){
			ClearList.push_back(*str); // becouse now: *str = string("$")
			str++;
		}
	}
	
	return ClearList;
}

list<string> SimilarTo(string pattern, TTrie *dict, int diff = 1){
	list<string> similarList;
	
	if(pattern.size() == 0){
		return similarList;
	}
	
	cout << "k = 1";
	list<string> wordsList1 = _SimilarRecursive(pattern);
	
	// Transpositions | Deletions | Substitutions | Insertions : 4 stages
	 
	cout << "...  completed\n";
	list<string>::iterator str;
	if(diff > 1){
		int i;
		for(int j = 2; j < diff + 1; j++){
			cout << "k = " << j;
			list<string> wordsList2; // triple '$' need for correct concatination
			wordsList2.push_back(string("$"));
			wordsList2.push_back(string("$"));
			wordsList2.push_back(string("$"));
			
			for(i = 0; i < 4; i++){
				for(str = wordsList1.begin(); *str != "$" && str != wordsList1.end(); str++){
					wordsList2 = _ComplexConcat(wordsList2,_SimilarRecursive(*str));
				}
				str++;
			}
			wordsList1 = _ComplexConcat(wordsList1,_UnicueList(wordsList2));
			//wordsList1 = _UnicueList(wordsList2);
			cout << "...  completed\n";
		}
	}
	
	wordsList1 = _UnicueList(wordsList1);
	wordsList1.remove(pattern);
	
	str = wordsList1.begin();
	for(; str != wordsList1.end();){
		for(; (*str != "$") && (str != wordsList1.end()); str++){
			//cout << *str << endl;//'\n';
			if(dict->Search(*str)){
				similarList.push_back(*str);
			}
		}
		if(str != wordsList1.end()){
			str++;
		}
	}
	
	cout << similarList.size() << '\n';
	
	return similarList;
}
	
int main(int argc, char *argv[]){
	if(argc < 3){
		cout << "Error Bro: set correct input, pls: prog file pattern\n";
		return 1;
	}
	
	ifstream fdict (argv[1]);
	
	if(!fdict.is_open()){
		cout << "Error Bro: can't open file, help me, pls\n";
		return 2;
	}
	//*/
	
	string line, str;
	vector<int> counts;
	
	TTrie *Trie = new TTrie();
	string pattern = string(argv[2]);
	
	int nxtDelim, delim;
	
	// extract word and count:
	while(getline(fdict,line)){
	//for(int i = 0; i < 10; i++){
		//getline(fdict,line);
		delim = line.find(9);
		str = line.substr(0,delim);
		
		// count usualy places in last part of file
		nxtDelim = delim;
		do{
			delim = nxtDelim;
			nxtDelim = line.find('\t',nxtDelim + 1);
		}while(0 <= nxtDelim);
		
		counts.push_back(GetInt(line.substr(delim + 1)));
		//cout << " :: " << line.substr(delim + 1) << " | " << GetInt(line.substr(delim + 1)) << " | " << counts.back();
		
		//TTrie *t = (TTrie*)&counts.back();
		//cout << " || " << *(int*)t << '\n';
		
		Trie->Include(str, (TTrie*)&counts.back());
	}
	
	list<string> List = SimilarTo(pattern,Trie,2);
	
	//for(int i = )
	
	if(List.size() > 0){
		for(list<string>::iterator it = List.begin(); it != List.end(); it++){
			cout << *it << ' ' << Trie->Search(*it) << "\n";
		}
	}else{
		cout << "Something not work - nothing is finded... help me, pls\n";
	}
	
	
	
	fdict.close();
	//*/
	return 0;
}