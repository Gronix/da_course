#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>

#define ALPHABET 26 // |Alphabet|
#define MAXINT 2147483647
#define MAXPATLEN 100
#define TYPOSDEPTH 10  // typos generation depth
#define TOPSIZE 20

using namespace std;

typedef struct {
	string word;
	int count;
} TWordNCount;

int strLen(const char *str){
	int len = -1;
	while(str[++len]);
	return len;
}


int abs(int x){
	if(x < 0){
		x *= -1;
	}
	return x;
}


bool ProbMoreThan(string candidate, int changeType, int popularity, TWordNCount pairExists){
	if(abs(pairExists.count) < ((changeType == 1)? 2 : 1) * popularity){
		return true;
	}
	return false;
}


void CheckTopOf(list<TWordNCount> *List, string newbie, int changeType, int popularity){
	list<TWordNCount>::iterator str1 = List->begin();
	
	for(; str1 != List->end(); str1++){
		if(ProbMoreThan(newbie, changeType, popularity, *str1)){
			TWordNCount pair;
			
			pair.word = newbie;
			pair.count = ((changeType == 1)? -2 : 1) * popularity;
			List->insert(str1, pair);
			break;
		}
	}
	if(List->size() > TOPSIZE){
		List->pop_back();
	}
}


class TTrie{
public:
	char c;
	int count;
	int typoDepth;
	TTrie *parent;
	TTrie *child[26]; 
	
	
	TTrie(void){
		c = '_';
		count = 0;
		parent = NULL;
		for(int i = 0; i < 26; i++){
			child[i] = NULL;
		}
	};
	
	
	TTrie(char ch){
		c = ch;
		count = 0;
		parent = NULL;
		for(int i = 0; i < 26; i++){
			child[i] = NULL;
		}
	};
	
	
	TTrie(char ch, TTrie *p){
		c = ch;
		count = 0;
		parent = p;
		for(int i = 0; i < 26; i++){
			child[i] = NULL;
		}
	};
	
	
	void Include(string str, int countNum = 0, int genTypos = 0){
		if(child[_CharId(str[0])]){
			child[_CharId(str[0])]->_IncludeRecursive(0, str, countNum, genTypos);
		}else{
			child[_CharId(str[0])] = _CreateBranch(0, str, countNum, genTypos);
		}
	};


	void Include(char *str, int countNum = 0, int genTypos = 0){
		if(child[_CharId(str[0])]){
			child[_CharId(str[0])]->_IncludeRecursive(0, str, countNum, genTypos);
		}else{
			child[_CharId(str[0])] = _CreateBranch(0, str, countNum, genTypos);
		}
	};

	
	int Search(char *str){
		int i, answer = 0, size = strLen(str);
		TTrie *node = this;
		
		for(i = 0; i < size - 1; i++){
			node = node->child[_CharId(str[i])];
			if(node == NULL){
				break;
			}
		}
		if(node != NULL && node->child[_CharId(str[i])] != NULL){
			node = node->child[_CharId(str[i])];
			
			if((i == size - 1) and node->count){
				answer = node->count;
			}
		}
		return answer;
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
	
	
	void xPrint(ofstream& stream, int lvl = 0){
		for(int j = 0; j < lvl; j++){
			stream << "_";
		}
		stream << c;
		if(count){
			stream << " + " << count;
		}
		stream << "\n";
		
		for(int i = 0; i < 26; i++){
			if(child[i]){
				child[i]->xPrint(stream, lvl + 1);
			}
		}
	}

	
	void Print(int lvl = 0){
		for(int j = 0; j < lvl; j++){
			cout << "_";
		}
		cout << c;
		if(count){
			cout << " - " << count;
		}
		cout << "\n";
		
		for(int i = 0; i < 26; i++){
			if(child[i]){
				child[i]->Print(lvl + 1);
			}
		}
	}
	
	
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
	
private:

	bool _IncludeRecursive(int pos, char *str, int countNum, int genTypos){
		if(pos == strLen(str) - 1){
			if(count){
				if(!genTypos){
					cerr << "DUPLICATE: " << str << " " << str[pos] << " " << pos << "\n";
				}
			}else{
				typoDepth = genTypos;
				if(countNum != 0){
					count = countNum; // dirty trick... be careful
				}else{
					count = -1;
				}
			}
		}else{
			if(child[_CharId(str[pos + 1])]){
				child[_CharId(str[pos + 1])]->_IncludeRecursive(pos + 1, str, countNum, genTypos);
			}else{
				child[_CharId(str[pos + 1])] = _CreateBranch(pos + 1, str, countNum, genTypos);
			}
		}
	};

	
	TTrie* _CreateBranch(int pos, char* str, int countNum, int genTypos){
		TTrie *root = new TTrie(str[pos], this);
		
		if(pos == strLen(str) - 1){
			root->typoDepth = genTypos;
			root->parent = this;
			if(countNum != 0){
				root->count = countNum; // dirty trick... be careful
			}else{
				root->count = -1;
			}
		}else{
			root->count = 0;
			root->child[_CharId(str[pos + 1])] = root->_CreateBranch(pos + 1, str, countNum, genTypos);
		}
		
		return root;
	};
	

	bool _IncludeRecursive(int pos, string str, int countNum, int genTypos){
		if(pos == str.size() - 1){
			if(count){
				if(!genTypos){
					cerr << "DUPLICATE: " << str << " " << str[pos] << " " << pos << "\n";
				}
			}else{
				typoDepth = genTypos;
				if(countNum != 0){
					count = countNum; // dirty trick... be careful
				}else{
					count = -1;
				}
			}
		}else{
			if(child[_CharId(str[pos + 1])]){
				child[_CharId(str[pos + 1])]->_IncludeRecursive(pos + 1, str, countNum, genTypos);
			}else{
				child[_CharId(str[pos + 1])] = _CreateBranch(pos + 1, str, countNum, genTypos);
			}
		}
	};

	
	TTrie* _CreateBranch(int pos, string str, int countNum, int genTypos){
		TTrie *root = new TTrie(str[pos], this);
		
		if(pos == str.size() - 1){
			root->typoDepth = genTypos;
			if(countNum != 0){
				root->count = countNum; // dirty trick... be careful
			}else{
				root->count = -1;
			}
		}else{
			root->count = 0;
			root->child[_CharId(str[pos + 1])] = root->_CreateBranch(pos + 1, str, countNum, genTypos);
		}
		
		return root;
	};
	
	
	char _ToLow(char c){
		if((65 <= c) and (c <= 90)){
			c += 22;
		}
		return c;
	};
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


void _TopExistsRecursive(TTrie *rootTypos, int depth, char *str, list<TWordNCount> *res, TTrie *db){
	str[depth] = rootTypos->c;
	
	for(int i = 0; i < 26; i++){
		if(rootTypos->child[i]){
			_TopExistsRecursive(rootTypos->child[i], depth + 1, str, res, db);
		}
	}
	
	if(rootTypos->count){
		str[depth + 1] = 0;
		int popularity = db->Search(str);
		
		if(popularity > 0){
			CheckTopOf(res, str, rootTypos->count, popularity);
		}
		rootTypos->count = 0;
	}
}

	
list<TWordNCount> GetTopExistances(TTrie *sample, TTrie *db){
	list<TWordNCount> res;
	char str[MAXPATLEN];
	int depth = 0;
	
	str[MAXPATLEN] = 0;
	
	TWordNCount dummy;
	dummy.word = string(1, ' ');
	dummy.count = 0;
	res.push_back(dummy);
	
	if(sample->c == '_'){
		for(int i = 0; i < 26; i++){
			if(!sample->child[i]){
				continue;
			}
			_TopExistsRecursive(sample->child[i], depth, str, &res, db);
		}
	}else{
		_TopExistsRecursive(sample, depth, str, &res, db);
	}
	
	return res;
}


// UnSafe function: dont check str "dest" to memory corruptions!
char* strCpyFrom(char *origin, char *dest, int start = 0, int len = 0){
	if(len == 0){
		origin += start;
		while(*origin){
			dest[len] = *origin;
			len++;
			origin++;
		}
		dest[len] = 0;
	}else{
		dest[len] = 0;
		len--;
		while(len >= start){
			dest[len] = origin[len];
			len--;
		}
	}
	return dest;
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

// UnSafe function: dont check str "dest" to memory corruptions!
char* strCpyBackFrom(char *origin, char *dest, int start = 0, int len = 0){
	if(len == 0){
		len = strLen(dest);
		origin += start;
		while(*origin){
			dest[len] = *origin;
			len++;
			origin++;
		}
		dest[len] = 0;
	}else{
		int olen = start + len - 1;
		len += strLen(dest);
		dest[len] = 0;
		len--;
		while(olen >= start){
			dest[len] = origin[olen];
			len--;
			olen--;
		}
	}
	return dest;
}


// IF WORD be length 1 its so bad.
TTrie* __SimilarGenerate(char *pattern, int typosLvl, TTrie *db){
	int i, pos, k, plen = strLen(pattern);
	char *str = new char[plen + 2];
	char *spStr[(plen - 1) * 2]; // splitted string without delimiter char: Cnk*2 - 2 = N!/(N-K)!*K!

	
	// Transpositions
	{
		char c;
		
		str = strCpyFrom(pattern, str);
		for(i = 0; i < plen - 1; i++){
			c = str[i+1];
			str[i+1] = str[i];
			str[i] = c;
			
			db->Include(str, 1, typosLvl);
			
			c = str[i+1];
			str[i+1] = str[i];
			str[i] = c;
		}
	}
	
	// Deletions
	{
		pos = -1;
		spStr[++pos] = strCpyCreate(pattern, 1);
		db->Include(spStr[pos], 2, typosLvl);
		
		for(i = 1; i < plen - 1; i++){
			spStr[++pos] = strCpyCreate(pattern, 0, i);
			spStr[++pos] = strCpyCreate(pattern, i + 1, plen - strLen(spStr[pos - 1]) - 1);
			
			str = strCpyFrom(spStr[pos - 1], str);
			str = strCpyBackFrom(spStr[pos], str);
			
			db->Include(str, 2, typosLvl);
		}
		
		spStr[++pos] = strCpyCreate(pattern, 0, plen - 1);
		db->Include(spStr[pos], 2, typosLvl);
	}
	
	// Substitutions
	{		
		str[0] = 'a';
		str[1] = 0;
		str = strCpyBackFrom(spStr[0], str);
		db->Include(str, 3, typosLvl);
		for(i = 1; i < ALPHABET; i++){
			str[0] = (char)(97 + i);
			db->Include(str, 3, typosLvl);
		}

		pos = 1;
		for(int j = 1; j < plen - 1; j++){ // first and last positions processed separately
			k = strLen(spStr[pos]);
			str = strCpyFrom(spStr[pos], str);
			str[k] = 'a';
			str[k + 1] = 0;
			str = strCpyBackFrom(spStr[pos + 1], str);
			db->Include(str, 3, typosLvl);
			for(i = 1; i < ALPHABET; i++){
				str[k] = (char)(i + 97);
				db->Include(str, 3, typosLvl);
			}
			pos += 2;
		}
		// pos должен быть равен 2n - 1
		
		str = strCpyFrom(spStr[pos], str);
		k = strLen(spStr[pos]);
		str[k + 1] = 0;
		for(i = 0; i < ALPHABET; i++){
			str[k] = (char)(97 + i);
			db->Include(str, 3, typosLvl);
		}
	}
	
	// Insertions
	{
		str[0] = 'a';
		str[1] = 0;
		str = strCpyBackFrom(pattern, str);
		db->Include(str, 4, typosLvl);
		for(i = 1; i < ALPHABET; i++){
			str[0] = (char)(97 + i);
			db->Include(str, 4, typosLvl);
		}
		
		if(plen > 1){
			str[0] = pattern[0];
			str[1] = 'a';
			str[2] = 0;
			str = strCpyBackFrom(spStr[0], str);
			db->Include(str, 4, typosLvl);
			for(i = 1; i < ALPHABET; i++){
				str[1] = (char)(97 + i);
				db->Include(str, 4, typosLvl);
			}
			
			
			if(plen > 2){
				pos = 1;
				for(int j = 1; j < plen - 1; j++){ // first and last positions processed separately
					k = strLen(spStr[pos]);
					str = strCpyFrom(spStr[pos],str);
					str[k] = 'a';
					str[k + 1] = pattern[j];
					str[k + 2] = 0;
					str = strCpyBackFrom(spStr[pos + 1], str);
					db->Include(str, 4, typosLvl);
					for(i = 1; i < ALPHABET; i++){
						str[k] = (char)(i + 97);
						db->Include(str, 4, typosLvl);
					}
					pos += 2;
				}
				
				k = strLen(spStr[pos]);
				str = strCpyFrom(spStr[pos], str);
				str[k] = 'a';
				str[k + 1] = 0;
				str = strCpyBackFrom(spStr[pos - 1], str);
				db->Include(str, 4, typosLvl);
				for(i = 1; i < ALPHABET; i++){
					str[k] = (char)(97 + i);
					db->Include(str, 4, typosLvl);
				}
			}
		}
		
		str = strCpyFrom(pattern, str);
		str[plen + 1] = 0;
		for(i = 0; i < ALPHABET; i++){
			str[plen] = (char)(97 + i);
			db->Include(str, 4, typosLvl);
		}
		
	}
	
	// free all busy memory

	delete[] str;
	for(i = 0; i < plen * 2 - 2; i++){
		delete[] spStr[i];
	}
	return db;
}


TTrie* _TraverseRec(TTrie *root, char *pattern, int lvl, int pos){
	TTrie *resNode;
	
	pattern[pos] = root->c;
	for(int i = 0; i < 26; i++){
		if(!root->child[i]){
			continue;
		}
		resNode = _TraverseRec(root->child[i], pattern, lvl, pos + 1);
		if(resNode){
			return resNode;
		}
	}
	if(root->count > 0 && root->typoDepth == lvl){
		pattern[pos + 1] = 0;
		resNode = root;
	}else{
		resNode = NULL;
	}
	
	return resNode;
}


TTrie* Traverse(TTrie *startNode, char *pattern, int lvl){
	int newChId;
	TTrie *resNode;
	
	lvl--;
	if(startNode->c != '_'){
		int pos = strLen(pattern);
		char prevCh;
		do{
			prevCh = startNode->c;
			startNode = startNode->parent;
			newChId = _CharId(prevCh) + 1;
			pos--;
			while(newChId < 26){
				if(startNode->child[newChId]){
					resNode = _TraverseRec(startNode->child[newChId], pattern, lvl, pos);
					if(resNode){
						return resNode;
					}
				}
				newChId++;
			}
		}while(startNode->c != '_');
	}else{
		for(int i = 0; i < 26; i++){
			resNode = _TraverseRec(startNode->child[i], pattern, lvl, 0);
			if(resNode){
				break;
			}
		}
	}
	
	return resNode;
}



list<TWordNCount> __SimilarTo(string pattern, TTrie *typos, TTrie *dict, int diff = 1){
	list<TWordNCount> similarList;
	
	if(pattern.size() == 0){
		return similarList;
	}
	
	cout << "k = 1";
	
	char *ss = strCpyCreate(pattern.c_str());
	typos = __SimilarGenerate(ss, 1, typos);
	
	cout << "...  completed.\n";

	
	TTrie *node = typos;
	
	if(diff > 1){
		char typoPat[MAXPATLEN];
		
		for(int j = 2; j < diff + 1; j++){
			cout << "k = " << j;
			while(node = Traverse(node, typoPat, j)){
				typos = __SimilarGenerate(typoPat, j, typos);
			}

			cout << "...  completed\n";
		}
	}
	
	node = typos->SearchTypos(pattern);
	if(node){
		node->count = 0;
	}
	
	similarList = GetTopExistances(typos, dict);
	
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
		delim = line.find(9);
		str = line.substr(0,delim);
		
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
	
	TTrie *typos = new TTrie();
	
	list<TWordNCount> List = __SimilarTo(pattern, typos, Trie, 2);
	
	cout.setf(ios::fixed);
	
	if(List.size() > 1){
		string maxProbStr;
		double maxProb = 0;
		
		if(List.back().word == " "){
			List.pop_back();
		}
		
		for(list<TWordNCount>::iterator it = List.begin(); it != List.end(); it++){
			cout << (*it).word << ' ';
			if((*it).count < 0){
				cout << (*it).count / -2;
			}else{
				cout << (*it).count;
			}
			cout << '\n';
		}
	}else{
		cout << "Something not work - nothing is finded... help me, pls\n";
	}
	
	cout << "\nSUM: " << sum << '\n';
	
	fdict.close();
	return 0;
}
