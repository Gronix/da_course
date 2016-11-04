#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>

#define ALPHABET 26 // |Alphabet|
#define MAXINT 2147483647

using namespace std;

typedef struct {
	string word;
	int count;
} TWordNCount;

class TTrie{
public:
	char c;
	int count;
	TTrie *child[26]; 
	
	
	TTrie(void){
		c = '_';
		count = 0;
		for(int i = 0; i < 26; i++){
			child[i] = NULL;
		}
	};
	
	TTrie(char ch){
		c = ch;
		count = 0;
		for(int i = 0; i < 26; i++){
			child[i] = NULL;
		}
	};
	
	void Include(string str, int countNum = 0){
		if(child[_CharId(str[0])]){
			child[_CharId(str[0])]->_IncludeRecursive(0, str, countNum);
		}else{
			child[_CharId(str[0])] = _CreateBranch(0, str, countNum);
		}
	};
	
	int Search(string str){
		int i, answer = 0;
		TTrie *node = this;
		
		for(i = 0; i < str.size() - 1; i++){
			node = node->child[_CharId(str[i])];
			if(node == NULL){
				break;
			}
		}
		if(node != NULL && node->child[_CharId(str[i])] != NULL){
			node = node->child[_CharId(str[i])];
			
			if((i == str.size() - 1) and node->count){
				answer = node->count;
			}
		}
		return answer;
	};
	
	TTrie* SearchTypos(string str){
		int i;
		TTrie *node = this;
		
		for(i = 0; i < str.size() - 1; i++){
			node = node->child[_CharId(str[i])];
			if(node == NULL){
				break;
			}
		}
		if(node != NULL && node->child[_CharId(str[i])] != NULL){
			node = node->child[_CharId(str[i])];
			
			if(not ((i == str.size() - 1) and node->count) ){
				node = NULL;
			}
		}
		return node;
	};
	
	void Print(int lvl = 0){
		for(int j = 0; j < lvl; j++){
			cout << "__";
		}
		cout << c;
		if(count){
			cout << " " << count;
		}
		cout << "\n";
		
		for(int i = 0; i < 26; i++){
			if(child[i]){
				child[i]->Print(lvl + 1);
			}
		}
	}
private:
	void _IncludeRecursive(int pos, string str, int countNum, bool genTypos){
		if(pos == str.size() - 1){
			if(count){
				if(genTypos){
					count = countNum + count * 10;
				}else{
					cerr << "DUPLICATE: " << str << " " << str[pos] << " " << pos << "\n";
				}
			}else{
				if(countNum != 0){
					count = countNum; // dirty trick... be careful
				}else{
					count = -1;
				}
			}
		}else{
			if(child[_CharId(str[pos + 1])]){
				child[_CharId(str[pos + 1])]->_IncludeRecursive(pos + 1, str, countNum);
			}else{
				child[_CharId(str[pos + 1])] = _CreateBranch(pos + 1, str, countNum);
			}
		}
	};

	TTrie* _CreateBranch(int pos, string str, int countNum){
		TTrie *root = new TTrie(str[pos]);
		
		if(pos == str.size() - 1){
			if(countNum != 0){
				root->count = countNum; // dirty trick... be careful
			}else{
				root->count = -1;
			}
		}else{
			root->child[_CharId(str[pos + 1])] = _CreateBranch(pos + 1, str, countNum);
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
	
	int p = 0;
	list<string>::iterator it;
	
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
			part2 = string(pattern, j + 1, n - j);
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

// IF WORD be length 1 its so bad.
TTrie* __SimilarRecursive(string pattern, TTrie *db){
	string str;
	string spStr[(pattern.size() - 1) * 2] // splitted string without delimiter char: Cnk*2 - 2 = N!/(N-K)!*K!
	//list<string> similarList;
	int i, n = pattern.size(), pos;
	
	// Transpositions
	{
		char c;
		
		for(i = 0; i < str.size() - 1; i++){
			str = string(pattern);
			c = str[i+1];
			str[i+1] = str[i];
			str[i] = c;
			
			//similarList.push_back(str);
			db.Include(str,4);
		}
	}
	//similarList.push_back(string("$"));
	// Deletions
	{
		str = string(pattern,0,n - 1);
		//similarList.push_back(str);
		db.Include(str, 2);
		pos = -1;
		spStr[++pos] = str;
		
		for(i = 1; i < n - 1; i++){
			str = string(pattern,0,i);
			spStr[++pos] = str;
			spStr[++pos] = string(pattern,i+1,n - str.size() - 1);
			str += spStr[pos];
			
			//similarList.push_back(str);
			bd.Include(str,2);
		}
		
		str = string(pattern,1);
		spStr[++pos] = str;
		//similarList.push_back(str);
		bd.Include(str,2);
	}
	
	//similarList.push_back(string("$"));
	// Substitutions
	{
		//string part2(pattern,1);
		//string part2 = spStr[pos];
		for(i = 0; i < ALPHABET; i++){
			str = (char)(97 + i) + spStr[0];//part2;
			db.Include(str,3);
			//similarList.push_back(str);
		}
		
		//string part1;
		pos = 1;
		for(int j = 1; j < n - 1; j++){ // first and last positions processed separately
			//part1 = string(pattern, 0, j);
			//part2 = string(pattern, j + 1, n - j ); // ещё -1 должно быть...
			for(i = 0; i < ALPHABET; i++){
				//str = part1 + (char)(i + 97) + part2;
				str = spStr[pos] + (char)(i + 97) + spStr[pos + 1];
				db.Include(str, 3);
				//similarList.push_back(str);
			}
			pos += 2;
		}
		
		
		cout << pos << '\n';
		// pos должен быть равен 2n - 1
		//part1 = string(pattern, 0, n - 1);
		for(i = 0; i < ALPHABET; i++){
			//str = part1 + (char)(97 + i);
			str = spStr[pos] + (char)(97 + i);
			db.Include(str,3);
			//similarList.push_back(str);
		}
	}
	
	//similarList.push_back(string("$"));
	// Insertions
	{
		//string part2(pattern);
		for(i = 0; i < ALPHABET; i++){
			str = (char)(97 + i) + pattern;//part2;
			db.Include(str, 4);
			//similarList.push_back(str);
		}
		
		//string part1;
		pos = 1;
		for(int j = 1; j < n - 1; j++){ // first and last positions processed separately
			//part1 = string(pattern, 0, j);
			//part2 = string(pattern, j, n - j);
			for(i = 0; i < ALPHABET; i++){
				//str = part1 + (char)(i + 97) + part2;
				str = spStr[pos]+ (char)(i + 97) + pattern[j] + spStr[pos + 1];
				pos += 2;
				db.Include(str,4);
				//similarList.push_back(str);
			}
		}
		pos += 2;
		for(i = 0; i < ALPHABET; i++){
			str = spStr[pos] + (char)(97 + i) + spStr[pos - 1];
			db.Include(str, 4);
		}
		
		//part1 = string(pattern);
		for(i = 0; i < ALPHABET; i++){
			str = pattern + (char)(97 + i);
			db.Include(str, 4);
			//similarList.push_back(str);
		}
	}
	
	return db;
}

// нужно модифицировать так - отслеживать из какой секции текущий *str и 
// и оставлять тот экземпляр, чья вероятность выше.
list<string> _UnicueList(list<string> List){
	list<string> ClearList;
	list<string>::iterator str, newS;
	
	for(str = List.begin(); str != List.end(); str++){
		if(*str == "$"){
			ClearList.push_back(*str); // becouse now: *str = string("$")
			continue;
		}
		// check for uniquennes:
		for(newS = ClearList.begin(); newS != ClearList.end(); newS++){
			if(*newS == *str){
				break;
			}
		}
		if(newS == ClearList.end()){
			ClearList.push_back(*str);
		}
	}
	
	return ClearList;
}

list<TWordNCount> SimilarTo(string pattern, TTrie *dict, int diff = 1){
	list<TWordNCount> similarList;
	
	if(pattern.size() == 0){
		return similarList;
	}
	
	TTrie *temp = new TTrie();
	
	
	cout << "k = 1";
	list<string> wordsList1 = _SimilarRecursive(pattern);
	
	// Transpositions | Deletions | Substitutions | Insertions : 4 stages
	 
	cout << "...  completed. size: " << wordsList1.size() << '\n';
	list<string>::iterator str;
	if(diff > 1){
		int i;
		for(int j = 2; j < diff + 1; j++){
			cout << "k = " << j;
			list<string> wordsList2; // triple '$' need for correct concatination
			wordsList2.push_back(string("$"));
			wordsList2.push_back(string("$"));
			wordsList2.push_back(string("$"));
			
			for(str = wordsList1.begin(); str != wordsList1.end(); str++){
				if(*str == "$"){
					continue;
				}
				wordsList2 = _ComplexConcat(wordsList2,_SimilarRecursive(*str));
			}
			wordsList1 = _ComplexConcat(wordsList1,_UnicueList(wordsList2));
			//wordsList1 = _UnicueList(wordsList2);
			cout << "...  completed. size: " << wordsList1.size() << '\n';
		}
	}
	
	wordsList1 = _UnicueList(wordsList1);
	wordsList1.remove(pattern);
	
	//cout << "shifted size: " << wordsList1.size() << '\n';
	
	TWordNCount pair;
	int count;
	
	for(str = wordsList1.begin(); str != wordsList1.end(); str++){
		if(*str == "$"){
			continue;
		}
		count = dict->Search(*str);
		/*
		if(*str == "april"){
			cout << "april exist: " << count << '\n';
		}else
		if(*str == "apeil"){
			cout << "apeil exist: " << count << '\n';
		}
		else
		if(*str == "aprwl"){
			cout << "aprwl exist: " << count << '\n';
		}
		*/
		if(count){
			pair.word = *str;
			pair.count = count;
			similarList.push_back(pair);
		}
	}
	
	cout << similarList.size() << '\n';
	
	return similarList;
}


list<TWordNCount> Traverse(list<TWordNCount> dictSet, char *pattern, TTrie *trie){
	for(int i = 0; i < 26; i++){
		return 
	}
}


list<TWordNCount> _SimilarTo(string pattern, TTrie *dict, int diff = 1){
	list<TWordNCount> similarList;
	
	if(pattern.size() == 0){
		return similarList;
	}
	
	TTrie *typos = new TTrie();
	
	
	cout << "k = 1";
	typos = __SimilarRecursive(pattern,typos);
	
	// Transpositions | Deletions | Substitutions | Insertions : 4 stages
	 
	cout << "...  completed.\n";
	
	typos->Print();
	
	//list<string>::iterator str;
	if(diff > 1){
		int i;
		for(int j = 2; j < diff + 1; j++){
			cout << "k = " << j;
			list<string> wordsList2; // triple '$' need for correct concatination
			wordsList2.push_back(string("$"));
			wordsList2.push_back(string("$"));
			wordsList2.push_back(string("$"));
			
			for(str = wordsList1.begin(); str != wordsList1.end(); str++){
				if(*str == "$"){
					continue;
				}
				typos = __SimilarRecursive(typos);
			}
			wordsList1 = _ComplexConcat(wordsList1,_UnicueList(wordsList2));
			//wordsList1 = _UnicueList(wordsList2);
			cout << "...  completed\n";
		}
	}
	
	wordsList1 = _UnicueList(wordsList1);
	wordsList1.remove(pattern);
	
	//cout << "shifted size: " << wordsList1.size() << '\n';
	
	TWordNCount pair;
	int count;
	
	for(str = wordsList1.begin(); str != wordsList1.end(); str++){
		if(*str == "$"){
			continue;
		}
		count = dict->Search(*str);
		/*
		if(*str == "april"){
			cout << "april exist: " << count << '\n';
		}else
		if(*str == "apeil"){
			cout << "apeil exist: " << count << '\n';
		}
		else
		if(*str == "aprwl"){
			cout << "aprwl exist: " << count << '\n';
		}
		*/
		if(count){
			pair.word = *str;
			pair.count = count;
			similarList.push_back(pair);
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
	
	string line, str;
	
	TTrie *Trie = new TTrie();
	string pattern = string(argv[2]);
	
	int nxtDelim, delim, num, sum = 0;
	
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
		
		num = GetInt(line.substr(delim + 1));
		sum += num;
		if(MAXINT - num < sum){
			cout << "overflow!\n";
		}
		
		Trie->Include(str, num);
	}
	// НУЖНА ОТДЕЛЬНАЯ ОБРАБОТКА СЛУЧАЯ 1ой БУКВЫ
	list<TWordNCount> List = SimilarTo(pattern,Trie,2);
	
	cout.setf(ios::fixed);
	
	if(List.size() > 0){
		string maxProbStr;
		double maxProb = 0;
		
		for(list<TWordNCount>::iterator it = List.begin(); it != List.end(); it++){
			if((*it).word == "$"){
				continue;
			}
			if(maxProb < ((double)((*it).count)) / sum){
				maxProb = ((double)((*it).count)) / sum;
				maxProbStr = (*it).word;
			}
		}
		cout << maxProbStr << ' ' << maxProb << '\n';
	}else{
		cout << "Something not work - nothing is finded... help me, pls\n";
	}
	
	cout << "\nSUM: " << sum << '\n';
	
	fdict.close();
	return 0;
}