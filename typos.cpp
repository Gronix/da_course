#include "typos.h"

double FAILEDSEARCH = -2.0;

// confusion matrices:
int mDel[27][26];
int mAdd[27][26];
int mSub[26][26];
int mRev[26][26];
int chrs[27][26];
int  chr[26];


inline double ComplexProbability(int confCount, int chCount, double coeff, double prevProb){
	if(!chCount){
		prevProb = 0.;
	}else{
		prevProb = ((double)confCount / chCount) * prevProb;
	}

	return prevProb;
}


TTypoTrie::TTypoTrie(void){
	c = '_';
	prob = 0;
	parent = NULL;
	for(int i = 0; i < ALPHABETALL; i++){
		child[i] = NULL;
	}
}
	
	
TTypoTrie::TTypoTrie(char ch){
	c = ch;
	prob = 0;
	parent = NULL;
	for(int i = 0; i < ALPHABETALL; i++){
		child[i] = NULL;
	}
}
	
	
TTypoTrie::TTypoTrie(char ch, TTypoTrie *p){
	c = ch;
	prob = 0;
	parent = p;
	for(int i = 0; i < ALPHABETALL; i++){
		child[i] = NULL;
	}
}

	
bool TTypoTrie::Include(string str, double probOfWord, int genTypos){
	if(child[_CharId(str[0])]){
		return child[_CharId(str[0])]->_IncludeRecursive(0, str, probOfWord, genTypos);
	}else{
		child[_CharId(str[0])] = _CreateBranch(0, str, probOfWord, genTypos);
		return true;
	}
}


bool TTypoTrie::Include(char *str, double probOfWord, int genTypos){
	if(child[_CharId(str[0])]){
		return child[_CharId(str[0])]->_IncludeRecursive(0, str, probOfWord, genTypos);
	}else{
		child[_CharId(str[0])] = _CreateBranch(0, str, probOfWord, genTypos);
		return true;
	}
}


// Search and Print funcitons:

double TTypoTrie::Search(char *str){
	int i, size = strLen(str);
	double answer = FAILEDSEARCH;
	TTypoTrie *node = this;
	
	for(i = 0; i < size - 1; i++){
		node = node->child[_CharId(str[i])];
		if(node == NULL){
			break;
		}
	}
	if(node != NULL && node->child[_CharId(str[i])] != NULL){
		node = node->child[_CharId(str[i])];
		
		if((i == size - 1) and node->prob){
			answer = node->prob;
		}
	}
	return answer;
}


double TTypoTrie::Search(string str){
	int i;
	double answer = FAILEDSEARCH;
	TTypoTrie *node = this;
	
	for(i = 0; i < str.size() - 1; i++){
		node = node->child[_CharId(str[i])];
		if(node == NULL){
			break;
		}
	}
	if(node != NULL && node->child[_CharId(str[i])] != NULL){
		node = node->child[_CharId(str[i])];
		
		if((i == str.size() - 1) and node->prob){
			answer = node->prob;
		}
	}
	return answer;
}

TTypoTrie* TTypoTrie::SearchTypos(string str){
	int i;
	TTypoTrie *node = this;
	
	for(i = 0; i < str.size() - 1; i++){
		node = node->child[_CharId(str[i])];
		if(node == NULL){
			break;
		}
	}
	if(node != NULL && node->child[_CharId(str[i])] != NULL){
		node = node->child[_CharId(str[i])];
		
		if(not ((i == str.size() - 1) and node->prob) ){
			node = NULL;
		}
	}
	return node;
}


void TTypoTrie::xPrint(ofstream& stream, int lvl){
	for(int j = 0; j < lvl; j++){
		stream << "_";
	}
	stream << c;
	if(prob){
		stream << " + " << prob;
	}
	stream << "\n";
	
	for(int i = 0; i < ALPHABETALL; i++){
		if(child[i]){
			child[i]->xPrint(stream, lvl + 1);
		}
	}
}


void TTypoTrie::Print(int lvl){
	for(int j = 0; j < lvl; j++){
		cout << "_";
	}
	cout << c;
	if(prob){
		cout << " - " << prob;
	}
	cout << "\n";
	
	for(int i = 0; i < ALPHABETALL; i++){
		if(child[i]){
			child[i]->Print(lvl + 1);
		}
	}
}


// Save & Load functions:

bool TTypoTrie::_LoadRecursive(ifstream &input){
	char mask[ALPHABETALL];
	int i;
	
	input.read(&c, sizeof c);
	input.read(reinterpret_cast<char*>(&prob), sizeof prob);
	input.read(reinterpret_cast<char*>(&typoDepth), sizeof typoDepth);
	input.read(mask, ALPHABETALL);
	
	for(i = 0; i < ALPHABETALL; i++){
		if(mask[i]){
			TTypoTrie *node = new TTypoTrie();
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


bool TTypoTrie::Load(char *fname){
	ifstream input(fname, ios::binary);
	
	if(!input.is_open()){
		return DataConstructor();
	}
	
	if(_LoadRecursive(input)){
		input.close();

		cout << " # dictionary loaded succesfully!" << endl;
		return true;
	}else{
		input.close();
		return false;
	}
}


bool TTypoTrie::_SaveRecursive(ofstream &out){
	char mask[ALPHABETALL] = {0};
	int i;
	for(i = 0; i < ALPHABETALL; i++){
		if(child[i]){
			mask[i] = 1;
		}
	}
	
	out.write(&c, sizeof c);
	out.write(reinterpret_cast<const char*>(&prob), sizeof prob);
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


bool TTypoTrie::Save(char *fname){
	ofstream outFile(fname, ios::binary);
	
	if(!outFile.is_open()){
		cerr << "Can't open a dict file\n";
		return false;
	}
	
	if(_SaveRecursive(outFile)){
		outFile.close();

		cout << " # dictionary saved succesfully!" << endl;
		return true;
	}else{
		outFile.close();
		return false;
	}
	
	
}

//Recursive functions - include and create branch:

bool TTypoTrie::_IncludeRecursive(int pos, char *str, double probOfWord, int genTypos){
	if(pos == strLen(str) - 1){
		if(prob){
			if(genTypos < 0){
				// cerr << "DUPLICATE1: " << str << " " << str[pos] << " " << pos << "\n";
				return false;
			}else{
				if(prob < probOfWord){
					typoDepth = genTypos;
					prob = probOfWord;
				}
			}
		}else{
			typoDepth = genTypos;
			if(probOfWord >= 0.){
				prob = probOfWord; // dirty trick... be careful
			}else{
				prob = -1.;
			}
		}

		return true;
	}else{
		if(child[_CharId(str[pos + 1])]){
			return child[_CharId(str[pos + 1])]->_IncludeRecursive(pos + 1, str, probOfWord, genTypos);
		}else{
			child[_CharId(str[pos + 1])] = _CreateBranch(pos + 1, str, probOfWord, genTypos);
			return true;
		}
	}

}


TTypoTrie* TTypoTrie::_CreateBranch(int pos, char* str, double probOfWord, int genTypos){
	TTypoTrie *root = new TTypoTrie(str[pos], this);
	
	if(pos == strLen(str) - 1){
		root->typoDepth = genTypos;
		root->parent = this;
		if(probOfWord >= 0.){
			root->prob = probOfWord; // dirty trick... be careful
		}else{
			root->prob = -1.;
		}
	}else{
		root->prob = 0.;
		root->child[_CharId(str[pos + 1])] = root->_CreateBranch(pos + 1, str, probOfWord, genTypos);
	}
	
	return root;
}


bool TTypoTrie::_IncludeRecursive(int pos, string str, double probOfWord, int genTypos){
	if(pos == str.size() - 1){
		if(prob){
			if(genTypos < 0){
				// cerr << "DUPLICATE2: " << str << " " << str[pos] << " " << pos << "\n";
				return false;
			}else{
				if(prob < probOfWord){
					typoDepth = genTypos;
					prob = probOfWord;
				}
			}
		}else{
			typoDepth = genTypos;
			if(probOfWord >= 0.){
				prob = probOfWord; // dirty trick... be careful
			}else{
				prob = -1.;
			}
		}

		return true;
	}else{
		if(child[_CharId(str[pos + 1])]){
			return child[_CharId(str[pos + 1])]->_IncludeRecursive(pos + 1, str, probOfWord, genTypos);
		}else{
			child[_CharId(str[pos + 1])] = _CreateBranch(pos + 1, str, probOfWord, genTypos);
			return true;
		}
	}
}


TTypoTrie* TTypoTrie::_CreateBranch(int pos, string str, double probOfWord, int genTypos){
	TTypoTrie *root = new TTypoTrie(str[pos], this);
	
	if(pos == str.size() - 1){
		root->typoDepth = genTypos;
		if(probOfWord >= 0.){
			root->prob = probOfWord; // dirty trick... be careful
		}else{
			root->prob = -1.;
		}
	}else{
		root->prob = 0.;
		root->child[_CharId(str[pos + 1])] = root->_CreateBranch(pos + 1, str, probOfWord, genTypos);
	}
	
	return root;
}

bool TTypoTrie::DataConstructor(){
	// char dictName[] = "fdict_bin";
	
	ifstream fKnowedWs("know_ws.txt");
	ifstream fUnigrams1("wcounts.txt");
	ifstream fUnigrams2("wcounts2.txt");
	ifstream fBigrams("w2_.txt");
	ifstream fTrigrams("w3_.txt");
	 
	if(!fKnowedWs.is_open() || !fUnigrams1.is_open() || !fUnigrams2.is_open() || !fBigrams.is_open() || !fTrigrams.is_open()){
		cerr << "Error Bro: can't open some files, help me, pls\n";
		return false;
	}
	
	string line, str;

	//extract knowed words... we delete unknowed words use this!
	TTypoTrie *knowed = new TTypoTrie();
	int _c = 0;
	while(getline(fKnowedWs, line)){
		knowed->Include(line);
		_c++;
	}

	int nxtDelim, delim;
	double uniSum, biSum, triSum, num;
	uniSum = biSum = triSum = 0.;

	// extract unigram words and counts:
	while(getline(fUnigrams1, line)){
		delim = line.find(9);
		str = line.substr(0,delim);

		num = GetDouble(line.substr(delim + 1, line.length() - str.length() - 1));

		if(knowed->Search(str) == FAILEDSEARCH){
			num *= pow(REDUCE_COEF);
		}

		uniSum += num;

		Include(str, num, -1);
	}
	

	while(getline(fUnigrams2, line)){
		delim = line.find(9);
		str = line.substr(0,delim);

		num = GetDouble(line.substr(delim + 1, line.length() - str.length() - 1));

		// when str including we returning answer 
		// in Include method - if this word exists then 
		// we don't add its count
		if(Include(str, num, -1)){
			uniSum += num;
		}
	}
	
	//extract bigrams:
	while(getline(fBigrams, line)){
		delim = line.find('\t');
		num = GetDouble(line.substr(0, delim));
		biSum += num;
		
		delim++;
		nxtDelim = line.find('\t', delim);
		str = line.substr(delim, nxtDelim - delim);
		
		nxtDelim++;
		str += ' ';
		str += line.substr(nxtDelim);
		
		Include(str, num, -2);
	}
	
	//extract trigrams:
	while(getline(fTrigrams, line)){
		delim = line.find('\t');
		num = GetDouble(line.substr(0, delim));
		triSum += num;
		
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
		
		Include(str, num, -3);
	}
	
	fKnowedWs.close();
	fUnigrams1.close();
	fUnigrams2.close();
	fBigrams.close();
	fTrigrams.close();


	// correct counts
	char temp_str[MAXPATLEN];
	double lvl_counts[] = {uniSum, biSum, triSum};
	cout << "	$umz: " << uniSum << " " << biSum << " " << triSum << endl;

	TTypoTrie *node;
	for(int i = 0; i < sizeof(lvl_counts) / sizeof(double); i++){
		node = this;
		while((node = Traverse(node, temp_str, -(i + 1)))){
			node->prob /= lvl_counts[i];
		}
	}
	
	cout << " # dictionary constructed!" << endl;

	return true;
}


	// Construct top of variants:

inline double ProbMoreThan(double errChanelProb, int typoDist, double freq, TWordNProb pairExists){
	freq = errChanelProb * freq;
	if(mabs(pairExists.prob) >= freq){
		freq = 0.;
	}

	return freq;
}


void CheckTopOf(list<TWordNProb> *List, char *newbie, double errChanelProb, int typoDist, double freq){
	list<TWordNProb>::iterator str1 = List->begin();
	double temp;
	for(; str1 != List->end(); str1++){
		temp = ProbMoreThan(errChanelProb, typoDist, freq, *str1);
		if(temp){
			TWordNProb pair;
			
			pair.word = string(newbie);
			pair.prob = temp;
			List->insert(str1, pair);
			break;
		}
	}
	if(List->size() > NUM_OF_VERS){
		List->pop_back();
	}
}

void _TopExistsRecursive(TTypoTrie *rootTypos, int depth, char *str, list<TWordNProb> *res, TTypoTrie *db){
	str[depth] = rootTypos->c;
	
	for(int i = 0; i < ALPHABETALL; i++){
		if(rootTypos->child[i]){
			_TopExistsRecursive(rootTypos->child[i], depth + 1, str, res, db);
		}
	}
	
	if(rootTypos->prob){
		str[depth + 1] = 0;
		double frequency = db->Search(str);
		
		if(frequency > 0.){
			CheckTopOf(res, str, rootTypos->prob, rootTypos->typoDepth, frequency);
		}else
		if(rootTypos->typoDepth == PARTITION_CODE){
			frequency = pow(REDUCE_COEF, 6);
			CheckTopOf(res, str, rootTypos->prob, rootTypos->typoDepth, frequency);
		}

		rootTypos->prob = 0.;
	}
}
	
list<TWordNProb> GetTopExistances(string patt, TTypoTrie *sample, TTypoTrie *db){
	list<TWordNProb> res;
	char str[MAXPATLEN];
	int depth = 0;
	
	str[MAXPATLEN] = 0;
	
	TWordNProb dummy;
	dummy.word = string(1, ' ');
	dummy.prob = 0.;
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



	// Traverse:

TTypoTrie* _TraverseRec(TTypoTrie *root, char *pattern, int lvl, int pos){
	TTypoTrie *resNode;
	
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
	if(root->prob > 0 && root->typoDepth == lvl){
		pattern[pos + 1] = 0;
		resNode = root;
	}else{
		resNode = NULL;
	}
	
	return resNode;
}


TTypoTrie* Traverse(TTypoTrie *startNode, char *pattern, int lvl){
	int newChId;
	TTypoTrie *resNode = NULL;
	
	if(lvl > 0){
		lvl--;
	}
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
			if(startNode->prob > 0 && startNode->typoDepth == lvl){
				pattern[pos] = 0;
				resNode = startNode;
				break;
			}
		}while(startNode->c != '_');
	}else{
		for(int i = 0; i < ALPHABETALL; i++){
			if(startNode->child[i]){
				resNode = _TraverseRec(startNode->child[i], pattern, lvl, 0);
				if(resNode){
					break;
				}
			}
		}
	}
	
	return resNode;
}

	// Similar generate:
	// 		РАССМАТРИВАЙ pattern, КАК СЛОВО НАПИСАННОЕ ЗАВЕДОМО С ОШИБКОЙ. 
	// 		ТУТ МЫ ГЕНЕРИРУЕМ ВСЕ ВОЗМОЖНЫЕ ВЕРНЫЕ ИСХОДНЫЕ ВАРИАНТЫ СЛОВА.

// IF WORD be length 1 its so bad.
TTypoTrie* __SimilarGenerate(char *pattern, int typosLvl, TTypoTrie *db, double prevProb, double coeff){
	int i, pos, k, plen = strLen(pattern), id1, id2;
	char *str = new char[plen + 2];
	char *spStr[(plen - 1) * 2]; // splitted string without delimiter char: Cnk*2 - 2 = N!/(N-K)!*K!
	double prob = 0.;
	
	
	// Transpositions
	{
		// number of times when xy was typed as yx
		char c;
		
		str = strCpyFrom(pattern, str);
		for(i = 0; i < plen - 1; i++){
			id1 = _CharId(str[i]);
			id2 = _CharId(str[i+1]);

			c = str[i+1];
			str[i+1] = str[i];
			str[i] = c;
			
			//db->Include(str, 1, typosLvl);
			prob = ComplexProbability(mRev[id1][id2], chrs[id1][id2], coeff, prevProb);
			db->Include(str, prob, typosLvl);
			
			c = str[i+1];
			str[i+1] = str[i];
			str[i] = c;
		}
	}
	
	// Deletions || IN MANUSCRIPT THIS NAMED Add[x,y] => i'm renamed it || ALL MATRICES WAS REPLACED CORRECTRLY.
	{
		pos = -1;
		spStr[++pos] = strCpyCreate(pattern, 1);
		// количество раз, когда х было написано ошибочно, как ху
		id2 = _CharId(pattern[0]);
		prob = ComplexProbability(mDel[26][id2], chr[26], coeff, prevProb);
		db->Include(spStr[pos], prob, typosLvl); 
		
		for(i = 1; i < plen - 1; i++){
			spStr[++pos] = strCpyCreate(pattern, 0, i);
			spStr[++pos] = strCpyCreate(pattern, i + 1, plen - strLen(spStr[pos - 1]) - 1);
			
			str = strCpyFrom(spStr[pos - 1], str);
			str = strCpyBackFrom(spStr[pos], str);
			
			id1 = id2;
			id2 = _CharId(pattern[i]);
			prob = ComplexProbability(mDel[id1][id2], chr[id1], coeff, prevProb);
			db->Include(str, prob, typosLvl);
		}
		
		spStr[++pos] = strCpyCreate(pattern, 0, plen - 1);

		id1 = _CharId(pattern[plen-2]);
		id2 = _CharId(pattern[plen-1]);
		prob = ComplexProbability(mDel[id1][id2], chr[id1], coeff, prevProb);
		db->Include(spStr[pos], prob, typosLvl);
	}
	
	// Substitutions
	{		
		str[0] = 'a';
		str[1] = 0;
		str = strCpyBackFrom(spStr[0], str);
		id1 = _CharId(pattern[0]);
		prob = ComplexProbability(mSub[0][id2], chr[id2], coeff, prevProb);
		db->Include(str, prob, typosLvl);
		for(i = 1; i < ALPHABET; i++){
			str[0] = (char)(97 + i);
			prob = ComplexProbability(mSub[i][id2], chr[id2], coeff, prevProb);
			db->Include(str, prob, typosLvl);
		}

		pos = 1;
		for(int j = 1; j < plen - 1; j++){ // first and last positions processed separately
			k = strLen(spStr[pos]);
			str = strCpyFrom(spStr[pos], str);
			str[k] = 'a';
			str[k + 1] = 0;
			str = strCpyBackFrom(spStr[pos + 1], str);
			
			id2 = _CharId(pattern[k]); // current CharId From Pattern
			
			prob = ComplexProbability(mSub[0][id2], chr[id2], coeff, prevProb);
			db->Include(str, prob, typosLvl);
			for(i = 1; i < ALPHABET; i++){
				str[k] = (char)(i + 97);
				prob = ComplexProbability(mSub[i][id2], chr[id2], coeff, prevProb);
				db->Include(str, prob, typosLvl);
			}
			pos += 2;
		}
		// pos должен быть равен 2n - 1
		
		
		str = strCpyFrom(spStr[pos], str);
		k = strLen(spStr[pos]);
		str[k + 1] = 0;
		
		id2 = _CharId(pattern[k]); // current CharId From Pattern
		
		for(i = 0; i < ALPHABET; i++){
			str[k] = (char)(97 + i);	
			prob = ComplexProbability(mSub[i][id2], chr[id2], coeff, prevProb);
			db->Include(str, prob, typosLvl);
		}
	}
	
	// Insertions || IN MANUSCRIPT IT NAMED Del[x,y] => i'm renamed it. || ALL MATRICES WAS REPLACED CORRECTRLY.
	{ 
		str[0] = 'a';
		str[1] = 0;
		str = strCpyBackFrom(pattern, str);
		prob = ComplexProbability(mAdd[26][0], chrs[26][0], coeff, prevProb);
		db->Include(str, prob, typosLvl);
		for(i = 1; i < ALPHABET; i++){
			str[0] = (char)(97 + i);
			prob = ComplexProbability(mAdd[26][i], chrs[26][i], coeff, prevProb);
			db->Include(str, prob, typosLvl);
		}
		
		if(plen > 1){
			id1 = _CharId(pattern[0]);	

			str[0] = pattern[0];
			str[1] = 'a';
			str[2] = 0;
			str = strCpyBackFrom(spStr[0], str);
			prob = ComplexProbability(mAdd[id1][0], chrs[id1][0], coeff, prevProb);
			db->Include(str, prob, typosLvl);
			for(i = 1; i < ALPHABET; i++){
				str[1] = (char)(97 + i);
				prob = ComplexProbability(mAdd[id1][i], chrs[id1][i], coeff, prevProb);
				db->Include(str, prob, typosLvl);
			}
			
			if(plen > 2){
				pos = 3;
				for(int j = 2; j < plen - 1; j++){ // first and last positions processed separately
					k = strLen(spStr[pos]);
					id1 = _CharId(pattern[k - 1]);	// current CharId From Pattern
					
					str = strCpyFrom(spStr[pos],str);
					str[k] = 'a';
					str[k + 1] = pattern[j];
					str[k + 2] = 0;
					str = strCpyBackFrom(spStr[pos + 1], str);
					prob = ComplexProbability(mAdd[id1][0], chr[id1], coeff, prevProb);
					db->Include(str, prob, typosLvl);
					for(i = 1; i < ALPHABET; i++){
						str[k] = (char)(i + 97);
						prob = ComplexProbability(mAdd[id1][i], chr[id1], coeff, prevProb);
						db->Include(str, prob, typosLvl);
					}
					pos += 2;
				}
				
				k = strLen(spStr[pos]);
				id1 = _CharId(pattern[k - 1]);
				
				str = strCpyFrom(spStr[pos], str);
				str[k] = 'a';
				str[k + 1] = 0;
				str = strCpyBackFrom(spStr[pos - 1], str);
				prob = ComplexProbability(mAdd[id1][0], chr[id1], coeff, prevProb);
				db->Include(str, prob, typosLvl);
				for(i = 1; i < ALPHABET; i++){
					str[k] = (char)(97 + i);
					prob = ComplexProbability(mAdd[id1][i], chr[id1], coeff, prevProb);
					db->Include(str, prob, typosLvl);
				}
			}
		}
		
		id1 = _CharId(str[plen - 1]);	// current CharId From Pattern
		str = strCpyFrom(pattern, str);
		str[plen + 1] = 0;
		for(i = 0; i < ALPHABET; i++){
			str[plen] = (char)(97 + i);
			prob = ComplexProbability(mAdd[id1][i], chr[id1], coeff, prevProb);
			db->Include(str, prob, typosLvl);
		}
		
	}
	
	// free all busy memory

	delete[] str;
	for(i = 0; i < plen * 2 - 2; i++){
		delete[] spStr[i];
	}
	return db;	//	RETURN db ITSELF, JUST TO NOTE THAT CHANGES WAS MADE
}


list<TWordNProb> __SimilarTo(string pattern, TTypoTrie *typos, TTypoTrie *dict, int diff){
	list<TWordNProb> similarList;
	
	if(pattern.size() == 0){
		return similarList;
	}
	// cout << "k = 1... ";	
	char *ss = strCpyCreate(pattern.c_str());
	typos = __SimilarGenerate(ss, 1, typos);

	// cout << "completed\n";
	TTypoTrie *node = typos;
	
	if(diff > 1){
		char typoPat[MAXPATLEN];
		
		for(int j = 2; j < diff + 1; j++){
			// cout << "k = " << j << "... ";
			while((node = Traverse(node, typoPat, j))){
				typos = __SimilarGenerate(typoPat, j, typos, node->prob, REDUCE_COEF);
			}
			// cout << "completed\n";
		}
	}
	
	// Generate patritions:
	{
		if(pattern.size() > 1){
			int i, plen = pattern.size();
			double prob1 = 0., prob2 = 0.;
			char *str = new char[plen + 2];
			for( i = 1; i < plen - 1; i++){
				str = strCpyFrom(ss, str, 0, i);
				str[i] = 0;
				prob1 = dict->Search(str);
				if(prob1 != FAILEDSEARCH){
					str[i] = ' ';
					str[i + 1] = 0;
					str = strCpyBackFrom(ss, str, i);

					prob2 = dict->Search(str + i + 1);
					if(prob2 != FAILEDSEARCH){
						prob1 *= prob2;
						typos->Include(str, prob1, PARTITION_CODE);
					}
				}
			}
			delete[] str;
		}
	}
	
	node = typos->SearchTypos(pattern);
	if(node){
		node->prob = 0.;
	}
	
	similarList = GetTopExistances(pattern, typos, dict);
	delete[] ss;
	
	return similarList;
}