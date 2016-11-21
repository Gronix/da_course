#include <iostream>
#include <fstream>
#include <string>
#include <list>

#define ADDITINALCHARS 4 	// 4x : \space, -, ', . 
#define ALPHABET 26  		// |Alphabet| 
#define ALPHABETALL ALPHABET + ADDITINALCHARS
#define MAXINT 2147483647
#define MAXPATLEN 100
#define TYPOSDEPTH 10  // typos generation depth
#define TOPSIZE 20
#define RELIABLELEN 6 // shifted by start index of cicle = 1

class TTrie;
TTrie *trie;


// confusion matrices:
int mDel[27][26];
int mAdd[27][26];
int mSub[26][26];
int mRev[26][26];

double REDUCE_COEF = 0.1;

using namespace std;


typedef struct {
	string word;
	int count;
} TWordNCount;


	// Secondary functions:

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


double pow(double x, int n = 2){
	double res = 1.0;
	for(int i = 0; i < n; i++){
		res *= x;
	}
	
	return res;
}


bool ProbMoreThan(string candidate, int errChanelProb, int typoDist, int popularity, TWordNCount pairExists){
	if(abs(pairExists.count) < errChanelProb * popularity * ((typoDist == 1)? 1 : pow(REDUCE_COEF, typoDist - 1))){
		return true;
	}
	return false;
}


void CheckTopOf(list<TWordNCount> *List, string newbie, int errChanelProb, int typoDist, int popularity){
	list<TWordNCount>::iterator str1 = List->begin();
	
	for(; str1 != List->end(); str1++){
		if(ProbMoreThan(newbie, errChanelProb, typoDist, popularity, *str1)){
			TWordNCount pair;
			
			pair.word = newbie;
			pair.count = errChanelProb * popularity * ((typoDist == 1)? 1 : pow(REDUCE_COEF, typoDist - 1));
			List->insert(str1, pair);
			break;
		}
	}
	if(List->size() > TOPSIZE){
		List->pop_back();
	}
}


inline int ComplexProbability(int curCount, double coeff, int prevProb){
	return prevProb + curCount + 1;
}


int _CharId(char c){
	if(65 <= c and c < 91){
		c -= 65;
	}else
	if(97 <= c and c < 123){
		c -= 97;
	}else
	if(c == ' ' or c == '\t'){
		c = 26;
	}else
	if(c == '-'){
		c = 27;
	}else
	if(c == '\''){
		c = 28;
	}else
	if(c == '.'){
		c = 29;
	}
	else{
		cerr << "WRONG CHAR! : " << (int)c << " " << c << "\n";
	}
	return c;
}

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



class TTrie{
public:
	char c;
	int count;
	int typoDepth;
	TTrie *parent;
	TTrie *child[ALPHABETALL]; 
	
	
	TTrie(void){
		c = '_';
		count = 0;
		parent = NULL;
		for(int i = 0; i < ALPHABETALL; i++){
			child[i] = NULL;
		}
	};
	
	
	TTrie(char ch){
		c = ch;
		count = 0;
		parent = NULL;
		for(int i = 0; i < ALPHABETALL; i++){
			child[i] = NULL;
		}
	};
	
	
	TTrie(char ch, TTrie *p){
		c = ch;
		count = 0;
		parent = p;
		for(int i = 0; i < ALPHABETALL; i++){
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

	
	// Search and Print funcitons:

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
		
		for(int i = 0; i < ALPHABETALL; i++){
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
		
		for(int i = 0; i < ALPHABETALL; i++){
			if(child[i]){
				child[i]->Print(lvl + 1);
			}
		}
	}
	
	
	// Save & Load functions:

	bool _LoadRecursive(ifstream &input){
		char mask[ALPHABETALL];
		int i;
		
		input.read(&c, sizeof c);
		input.read(reinterpret_cast<char*>(&count), sizeof count);
		input.read(reinterpret_cast<char*>(&typoDepth), sizeof typoDepth);
		input.read(mask, ALPHABETALL);
		
		for(i = 0; i < ALPHABETALL; i++){
			if(mask[i]){
				TTrie *node = new TTrie();
				child[i] = node;
				node->parent = this;
				node->_LoadRecursive(input);
			}
		}
		
		if(i != ALPHABETALL){
			cerr << "some error with loading..." << endl;
			return false;
		}
		return true;
	}
	
	
	bool Load(char *fname){
		ifstream input(fname, ios::binary);
		
		if(!input.is_open()){
			cerr << "Can't open input file" << endl;
			return false;
		}
		
		if(_LoadRecursive(input)){
			input.close();
			return true;
		}else{
			input.close();
			return false;
		}
	}
	
	
	bool _SaveRecursive(ofstream &out){
		char mask[ALPHABETALL] = {0};
		int i;
		for(i = 0; i < ALPHABETALL; i++){
			if(child[i]){
				mask[i] = 1;
			}
		}
		
		out.write(&c, sizeof c);
		out.write(reinterpret_cast<const char*>(&count), sizeof count);
		out.write(reinterpret_cast<const char*>(&typoDepth), sizeof typoDepth);
		out.write(mask, sizeof mask);

		for(i = 0; i < ALPHABETALL; i++){
			if(mask[i]){
				child[i]->_SaveRecursive(out);
			}
		}
		
		if(i != ALPHABETALL){
			cerr << "some err..." << endl;
			return false;
		}
		
		return true;
	}
	

	bool Save(char *fname){
		ofstream outFile(fname, ios::binary);
		
		if(!outFile.is_open()){
			cerr << "Can't open a dict file\n";
			return false;
		}
		
		if(_SaveRecursive(outFile)){
			outFile.close();
			return true;
		}else{
			outFile.close();
			return false;
		}
		
		
	}
	
	
/* 	 " "  = 26 // space and tab
	" - " = 27
	" ' " = 28
	" . " = 29;
 */
	
	int _CharId(char c){
		if(65 <= c and c < 91){
			c -= 65;
		}else
		if(97 <= c and c < 123){
			c -= 97;
		}else
		if(c == 32 or c == '\t'){
			c = 26;
		}else
		if(c == '-'){
			c = 27;
		}else
		if(c == '\''){
			c = 28;
		}else
		if(c == '.'){
			c = 29;
		}
		else{
			cerr << "WRONG CHAR! : " << (int)c << " " << c << "\n";
		}
		return c;
	};
	
private:

	char _ToLow(char c){
		if((65 <= c) and (c <= 90)){
			c += 22;
		}
		return c;
	};


	//Recursive functions - include and create branch:

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

};

	// Construct top of variants:

void _TopExistsRecursive(TTrie *rootTypos, int depth, char *str, list<TWordNCount> *res, TTrie *db){
	str[depth] = rootTypos->c;
	
	for(int i = 0; i < ALPHABETALL; i++){
		if(rootTypos->child[i]){
			_TopExistsRecursive(rootTypos->child[i], depth + 1, str, res, db);
		}
	}
	
	if(rootTypos->count){
		str[depth + 1] = 0;
		int popularity = db->Search(str);
		
		if(popularity > 0){
			CheckTopOf(res, str, rootTypos->count, rootTypos->typoDepth, popularity);
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
		for(int i = 0; i < ALPHABETALL; i++){
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



	// String functions:

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



	// Traverse:

TTrie* _TraverseRec(TTrie *root, char *pattern, int lvl, int pos){
	TTrie *resNode;
	
	pattern[pos] = root->c;
	for(int i = 0; i < ALPHABETALL; i++){
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
			while(newChId < ALPHABETALL){
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
		for(int i = 0; i < ALPHABETALL; i++){
			resNode = _TraverseRec(startNode->child[i], pattern, lvl, 0);
			if(resNode){
				break;
			}
		}
	}
	
	return resNode;
}



	// Similar generate:

// IF WORD be length 1 its so bad.
TTrie* __SimilarGenerate(char *pattern, int typosLvl, TTrie *db, int prevProb = 0, double coeff = 1.0){
	int i, pos, k, curCharIdFromPatt, plen = strLen(pattern);
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
			
			//db->Include(str, 1, typosLvl);
			db->Include(str, ComplexProbability(mRev[ _CharId(str[i]) ][ _CharId(str[i+1]) ], coeff, prevProb), typosLvl);
			
			c = str[i+1];
			str[i+1] = str[i];
			str[i] = c;
		}
	}
	
	// Deletions
	{
		pos = -1;
		spStr[++pos] = strCpyCreate(pattern, 1);
		//db->Include(spStr[pos], 2, typosLvl);
		// количество раз, когда х было написано ошибочно, как ху
		db->Include(spStr[pos], ComplexProbability(mDel[ 26 ][ _CharId(pattern[0]) ], coeff, prevProb), typosLvl); 
		
		for(i = 1; i < plen - 1; i++){
			spStr[++pos] = strCpyCreate(pattern, 0, i);
			spStr[++pos] = strCpyCreate(pattern, i + 1, plen - strLen(spStr[pos - 1]) - 1);
			
			str = strCpyFrom(spStr[pos - 1], str);
			str = strCpyBackFrom(spStr[pos], str);
			
			//db->Include(str, 2, typosLvl);
			db->Include(str, ComplexProbability(mDel[ _CharId(pattern[i-1]) ][ _CharId(pattern[i]) ], coeff, prevProb), typosLvl);
		}
		
		spStr[++pos] = strCpyCreate(pattern, 0, plen - 1);
		//db->Include(spStr[pos], 2, typosLvl);
		db->Include(spStr[pos], ComplexProbability(mDel[ _CharId(pattern[plen-2]) ][ _CharId(pattern[plen-1]) ], coeff, prevProb), typosLvl);
	}
	
	// Substitutions
	{		
		str[0] = 'a';
		str[1] = 0;
		str = strCpyBackFrom(spStr[0], str);
		//db->Include(str, 3, typosLvl);
		curCharIdFromPatt = _CharId(pattern[0]);
		db->Include(str, ComplexProbability(mSub[ 0 ][ curCharIdFromPatt ], coeff, prevProb), typosLvl);
		for(i = 1; i < ALPHABET; i++){
			str[0] = (char)(97 + i);
			//db->Include(str, 3, typosLvl);
			db->Include(str, ComplexProbability(mSub[ i ][ curCharIdFromPatt ], coeff, prevProb), typosLvl);
		}

		pos = 1;
		for(int j = 1; j < plen - 1; j++){ // first and last positions processed separately
			k = strLen(spStr[pos]);
			str = strCpyFrom(spStr[pos], str);
			str[k] = 'a';
			str[k + 1] = 0;
			str = strCpyBackFrom(spStr[pos + 1], str);
			
			curCharIdFromPatt = _CharId(pattern[k]);
			
			//db->Include(str, 3, typosLvl);
			db->Include(str, ComplexProbability(mSub[ 0 ][ curCharIdFromPatt ], coeff, prevProb), typosLvl);
			for(i = 1; i < ALPHABET; i++){
				str[k] = (char)(i + 97);
				//db->Include(str, 3, typosLvl);
				db->Include(str, ComplexProbability(mSub[ i ][ curCharIdFromPatt ], coeff, prevProb), typosLvl);
			}
			pos += 2;
		}
		// pos должен быть равен 2n - 1
		
		
		str = strCpyFrom(spStr[pos], str);
		k = strLen(spStr[pos]);
		str[k + 1] = 0;
		
		curCharIdFromPatt = _CharId(pattern[k]);
		
		for(i = 0; i < ALPHABET; i++){
			str[k] = (char)(97 + i);
			//db->Include(str, 3, typosLvl);
			db->Include(str, ComplexProbability(mSub[ i ][ curCharIdFromPatt ], coeff, prevProb), typosLvl);
		}
	}
	
	// Insertions
	{ 
		str[0] = 'a';
		str[1] = 0;
		str = strCpyBackFrom(pattern, str);
		//db->Include(str, 4, typosLvl);
		db->Include(str, mAdd[ 26 ][ 0 ], typosLvl);
		for(i = 1; i < ALPHABET; i++){
			str[0] = (char)(97 + i);
			//db->Include(str, 4, typosLvl);
			db->Include(str, ComplexProbability(mAdd[ 26 ][ i ], coeff, prevProb), typosLvl);
		}
		
		if(plen > 1){
			curCharIdFromPatt = _CharId(str[0]);
			
			str[0] = pattern[0];
			str[1] = 'a';
			str[2] = 0;
			str = strCpyBackFrom(spStr[0], str);
			//db->Include(str, 4, typosLvl);
			db->Include(str, ComplexProbability(mAdd[ curCharIdFromPatt ][ 0 ], coeff, prevProb), typosLvl);
			for(i = 1; i < ALPHABET; i++){
				str[1] = (char)(97 + i);
				//db->Include(str, 4, typosLvl);
				db->Include(str, ComplexProbability(mAdd[ curCharIdFromPatt ][ i ], coeff, prevProb), typosLvl);
			}
			
			
			if(plen > 2){
				pos = 3;
				for(int j = 2; j < plen - 1; j++){ // first and last positions processed separately
					k = strLen(spStr[pos]);
					curCharIdFromPatt = _CharId(pattern[k - 1]);
					
					str = strCpyFrom(spStr[pos],str);
					str[k] = 'a';
					str[k + 1] = pattern[j];
					str[k + 2] = 0;
					str = strCpyBackFrom(spStr[pos + 1], str);
					//db->Include(str, 4, typosLvl);
					db->Include(str, ComplexProbability(mAdd[ curCharIdFromPatt ][ 0 ], coeff, prevProb), typosLvl);
					for(i = 1; i < ALPHABET; i++){
						str[k] = (char)(i + 97);
						//db->Include(str, 4, typosLvl);
						db->Include(str, ComplexProbability(mAdd[ curCharIdFromPatt ][ i ], coeff, prevProb), typosLvl);
					}
					pos += 2;
				}
				
				k = strLen(spStr[pos]);
				curCharIdFromPatt = _CharId(str[k - 1]);
				
				str = strCpyFrom(spStr[pos], str);
				str[k] = 'a';
				str[k + 1] = 0;
				str = strCpyBackFrom(spStr[pos - 1], str);
				//db->Include(str, 4, typosLvl);
				db->Include(str, ComplexProbability(mAdd[ curCharIdFromPatt ][ 0 ], coeff, prevProb), typosLvl);
				for(i = 1; i < ALPHABET; i++){
					str[k] = (char)(97 + i);
					//db->Include(str, 4, typosLvl);
					db->Include(str, ComplexProbability(mAdd[ curCharIdFromPatt ][ i ], coeff, prevProb), typosLvl);
				}
			}
		}
		
		curCharIdFromPatt = _CharId(str[plen - 1]);
		str = strCpyFrom(pattern, str);
		str[plen + 1] = 0;
		for(i = 0; i < ALPHABET; i++){
			str[plen] = (char)(97 + i);
			//db->Include(str, 4, typosLvl);
			db->Include(str, ComplexProbability(mAdd[ curCharIdFromPatt ][ i ], coeff, prevProb), typosLvl);
		}
		
	}
	
	// free all busy memory

	delete[] str;
	for(i = 0; i < plen * 2 - 2; i++){
		delete[] spStr[i];
	}
	return db;
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

	//cout << "actress: " << typos->Search(string("actress")) << endl;
	
	TTrie *node = typos;
	
	if(diff > 1){
		char typoPat[MAXPATLEN];
		
		for(int j = 2; j < diff + 1; j++){
			cout << "k = " << j;
			while(node = Traverse(node, typoPat, j)){
				typos = __SimilarGenerate(typoPat, j, typos, node->count, REDUCE_COEF);
				//typos = __SimilarGenerate(typoPat, j, typos, node->count);
			}
			cout << "...  completed\n";
		}
	}
	
	// Generate patritions:
	{
		if(pattern.size() > 1){
			int i, prob = 0, plen = pattern.size();
			char *str = new char[plen + 2];
			
			for( i = 1; i < plen - 1; i++){
				str = strCpyFrom(ss, str, 0, i);
				str[i] = 0;
				prob = dict->Search(str) * pow(0.1, RELIABLELEN - i + 1);
				str[i] = ' ';
				str[i + 1] = 0;
				str = strCpyBackFrom(ss, str, i);
				prob += dict->Search(str + i + 1) * pow(0.1, RELIABLELEN - plen + i + 1);
				typos->Include(str, ComplexProbability(prob, 1.0, 0), 1);
			}
			delete[] str;
		}
	}
	
	node = typos->SearchTypos(pattern);
	if(node){
		node->count = 0;
	}
	
	similarList = GetTopExistances(typos, dict);
	delete[] ss;
	
	return similarList;
}



	// Read matricies:
	
void packRow(int *matrix, int row, string line){
	int num, i;
	
	i = -1;
	for(int col = 0; col < 25; col++){
		num = 0;
		while(line[++i] != ' '){
			num = (line[i] - 48) + 10 * num;
		}
		*(matrix + row * 26 + col) = num;
	}
	num = 0;
	while(line[++i] != 0){
		num = (line[i] - 48) + 10 * num;
	}
	*(matrix + row * 26 + 25) = num;

}


void readMatrices(int *matrices[]){
	char sdel[] = "_del_counts.txt";   // количество раз, когда x было ошибочно написано как xy
	char sadd[] = "_add_counts.txt";   // -||-  ху было ошибочно написано как х
	char ssub[] = "_subst_counts.txt"; // -||-  у было написано как х
	char srev[] = "_rev_counts.txt";   // -||-  ху было написано как ух
	
	string line;
	int row;
	
	char *fName[] = {sdel, sadd, ssub, srev};
	
	for(int id = 0; id < 4; id++){
		ifstream file(fName[id]);
		row = 0;
		while(getline(file, line)){
			packRow(matrices[id], row, line);
			row++;
		}
	}
}


	
int main(int argc, char *argv[]){
	/* if(argc < 3){
		cout << "Error Bro: set correct input, pls: prog file pattern\n";
		return 1;
	}
	
	ifstream fUnigrams (argv[1]);
	 */
	 
	bool needToSave = false;
	char dictName[] = "fdict_bin";
	int sum = 0;
	string pattern = string(argv[1]);
	
	//TTrie *trie = new TTrie();
	trie = new TTrie();
	if(!trie->Load(dictName)){
		needToSave = true;
		
		ifstream fUnigrams("wcounts.txt");
		ifstream fBigrams("w2_.txt");
		ifstream fTrigrams("w3_.txt");
		 
		if(!fUnigrams.is_open() || !fBigrams.is_open() || !fTrigrams.is_open()){
			cout << "Error Bro: can't open some files, help me, pls\n";
			return 2;
		}
		
		string line, str;
		
		//string pattern = string(argv[2]);
		
		int nxtDelim, delim, num;
		
		// extract unigram words and counts:
		while(getline(fUnigrams, line)){
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
			
			trie->Include(str, num);
		}
	
		cout << "unigram extracted" << endl;
		
		//extract bigrams:
		while(getline(fBigrams, line)){
			delim = line.find('\t');
			num = GetInt(line.substr(0, delim));
			
			delim++;
			nxtDelim = line.find('\t', delim);
			str = line.substr(delim, nxtDelim - delim);
			
			nxtDelim++;
			str += ' ';
			str += line.substr(nxtDelim);
			
			trie->Include(str, num);
		}
		
		cout << "bigram extracted" << endl;
		
		//extract trigrams:
		while(getline(fTrigrams, line)){
			delim = line.find('\t');
			num = GetInt(line.substr(0, delim));
			
			delim++;
			nxtDelim = line.find('\t', delim);
			str = line.substr(delim, nxtDelim - delim);
			
			nxtDelim++;
			delim = line.find('\t', nxtDelim);
			str += ' ';
			str += line.substr(nxtDelim, delim - nxtDelim);
			
			delim++;
			str += ' ';
			str += line.substr(delim);
			
			trie->Include(str, num);
		}
		
		cout << "trigram extracted" << endl;
		
		fUnigrams.close();
		fBigrams.close();
		fTrigrams.close();
	}
	
	cout << "dictionary constructed" << endl;
	
	if(argc >= 3){
		if(pattern == "-s"){
			pattern = argv[2];
			for(int i = 3; i < argc; i++){
				pattern += ' ';
				pattern += argv[i];
			}
			
			cout << pattern << ' ' << trie->Search(pattern) << endl;
	
			if(needToSave){
				trie->Save(dictName);
			}
	
			return 0;
		}
	}
	
	//  *** READ CONFUSION MATRICES: ***
	
	int *matrices[] = {&mDel[0][0], &mAdd[0][0], &mSub[0][0], &mRev[0][0]};
	readMatrices(matrices);
	
	//  *** END READ ***
	
	
	TTrie *typos = new TTrie();
	
	list<TWordNCount> List = __SimilarTo(pattern, typos, trie, 2);
	
	cout.setf(ios::fixed);
	
	if(List.size() > 1){
		string maxProbStr;
		double maxProb = 0;
		
		if(List.back().word == " "){
			List.pop_back();
		}
		
		for(list<TWordNCount>::iterator it = List.begin(); it != List.end(); it++){
			cout << (*it).word << ' ' << (*it).count << endl;
		}
	}else{
		cout << "Something not work - nothing is finded... help me, pls\n";
	}
	
	cout << "\nSUM: " << sum << '\n';
	
	if(needToSave){
		trie->Save(dictName);
	}
	
	return 0;
}
