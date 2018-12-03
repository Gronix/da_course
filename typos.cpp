#include "typos.h"

//double FAILEDSEARCH = -2.0;
double FAILEDSEARCH = 0.;
TTypoTrie *knowed_ws;
// double uniSum, biSum, triSum;
double def_prob;

// confusion matrices:
int mDel[27][26];
int mAdd[27][26];
int mSub[26][26];
int mRev[26][26];
int chrs[27][26];
int  chr[27];

int t_all, t;

inline int _CharId(char c){
	if(65 <= c and c < 91){
		c -= 65;
	}else
	if(97 <= c and c < 123){
		c -= 97;
	}else
	if(c == 32 or c == '\t'){
		c = 26;
	}else
	if(c == 35){ // '#'  начало строки
		c = 27;
	}else
	if(c == 36){ // '$'  начало строки
		c = 28;
	}else{
		cerr << "WRONG CHAR! : " << (int)c << " " << c << "\n";
	}
	return c;
};

inline double ChannelProb(int confCount, int chCount, double coeff, double prevProb){
	if(!chCount){
		prevProb = 0.;
	}else{
		prevProb = ((double)confCount / chCount) * prevProb;
	}

	#if _TEST_
		if(prevProb == 1){
			cout << "wat?" << endl;
		}
	#endif

	return prevProb;
}


TTypoTrie::TTypoTrie(void){
	c = '_';
	prob = 0.;
	prob_cont = 0.;
	y = 0.;
	parent = NULL;
	for(int i = 0; i < ALPHABETALL; i++){
		child[i] = NULL;
	}
}
	
	
TTypoTrie::TTypoTrie(char ch){
	c = ch;
	prob = 0.;
	prob_cont = 0.;
	y = 0.;
	parent = NULL;
	for(int i = 0; i < ALPHABETALL; i++){
		child[i] = NULL;
	}
}
	
	
TTypoTrie::TTypoTrie(char ch, TTypoTrie *p){
	c = ch;
	prob = 0.;
	prob_cont = 0.;
	y = 0.;
	parent = p;
	for(int i = 0; i < ALPHABETALL; i++){
		child[i] = NULL;
	}
}

	
bool TTypoTrie::Include(string &str, double probOfWord, int genTypos, double probOfWord_cont, double y_const){
	// if(probOfWord == 0.){
	// 	//cout << "wtf bro!?" << endl;
	// 	t++;
	// }
	// t_all++;
	if(child[_CharId(str[0])]){
		return child[_CharId(str[0])]->_IncludeRecursive(0, str, probOfWord, genTypos, probOfWord_cont, y_const);
	}else{
		child[_CharId(str[0])] = _CreateBranch(0, str, probOfWord, genTypos, probOfWord_cont, y_const);
		return true;
	}
}


bool TTypoTrie::Include(char *str, double probOfWord, int genTypos, double probOfWord_cont, double y_const){
	// if(probOfWord == 0.){
	// 	//cout << "wtf bro!?" << endl;
	// 	t++;
	// }
	// t_all++;
	if(child[_CharId(str[0])]){
		return child[_CharId(str[0])]->_IncludeRecursive(0, str, probOfWord, genTypos, probOfWord_cont, y_const);
	}else{
		child[_CharId(str[0])] = _CreateBranch(0, str, probOfWord, genTypos, probOfWord_cont, y_const);
		return true;
	}
}


// Search and Print funcitons:

double TTypoTrie::Search(char *str, int what_constant_needed){
	int i, size = strLen(str);
	double answer = FAILEDSEARCH;
	TTypoTrie *node = this;
	
	for(i = 0; i < size - 1; i++){
		node = node->child[_CharId(str[i])];
		if(node == NULL){
			break;
		}
	}
	if(what_constant_needed == 1){
		if(node != NULL && node->child[_CharId(str[i])] != NULL){
			node = node->child[_CharId(str[i])];
			
			if((i == size - 1) and node->prob != 0.){
				answer = node->prob;
			}
		}
	}else
	if(what_constant_needed == 2){
		if(node != NULL && node->child[_CharId(str[i])] != NULL){
			node = node->child[_CharId(str[i])];
			
			if((i == size - 1) and node->prob_cont != 0.){
				answer = node->prob_cont;
			}
		}
	}
	else{
		if(node != NULL && node->child[_CharId(str[i])] != NULL){
			node = node->child[_CharId(str[i])];
			
			if((i == size - 1) and node->y != 0.){
				answer = node->y;
			}
		}
	}
	return answer;
}


double TTypoTrie::Search(string &str, int what_constant_needed){
	int i, size = str.size();
	double answer = FAILEDSEARCH;
	TTypoTrie *node = this;
	
	for(i = 0; i < size - 1; i++){
		node = node->child[_CharId(str[i])];
		if(node == NULL){
			break;
		}
	}
	if(what_constant_needed == 1){
		if(node != NULL && node->child[_CharId(str[i])] != NULL){
			node = node->child[_CharId(str[i])];
			
			if((i == size - 1) and node->prob != 0.){
				answer = node->prob;
			}
		}
	}else
	if(what_constant_needed == 2){
		if(node != NULL && node->child[_CharId(str[i])] != NULL){
			node = node->child[_CharId(str[i])];
			
			if((i == size - 1) and node->prob_cont != 0.){
				answer = node->prob_cont;
			}
		}
	}
	else{
		if(node != NULL && node->child[_CharId(str[i])] != NULL){
			node = node->child[_CharId(str[i])];
			
			if((i == size - 1) and node->y != 0.){
				answer = node->y;
			}
		}
	}
	return answer;
}


// проверь все обращения к деку
double TTypoTrie::Search(deque<char*> &dstr, int start, int end, int what_constant_needed){
	int i, j, n;//, wlen;
	double answer = FAILEDSEARCH;
	TTypoTrie *node = this;
	//wlen = 
	//считаем длину дека-слова=предложения   
	//     !!! ПРОВЕРЬ ТУТ ВСЕ ОБРАЩЕНИЯ К МЕТОДАМ БЛЕАТЬ!!22121!!!! х))
/*	wlen = -1; // чтобы нивелировать последний лишний плюс к длине за пробелы, что в первом (внешнем) цикле
	for(i = 0; i < dstr.size; i++){
		for(j = 0; j < dstr[i].len; j++){
			wlen++;
		}
		wlen++
	}
	
	for(i = 0; i < wlen - 1; i++){
		node = node->child[_CharId(str[i])];
		if(node == NULL){
			break;
		}
	}*/

	if(end == -1){
		end = dstr.size();
	}
	for(i = start; i < end - 1; i++){
		n = strLen(dstr[i]);
		for(j = 0; j < n; j++){
			node = node->child[_CharId(dstr[i][j])];
			if(node == NULL){
				break;
			}
		}
		if(node == NULL){
			break;
		}
		//node = node->child[_CharId(' ')];
		node = node->child[26]; // это символ пробела в дереве совпадает по смыслу с верхней строкой
		if(node == NULL){
			break;
		}
	}
	if(node != NULL){
		n = strLen(dstr[i]);
		for(j = 0; j < n - 1; j++){
			node = node->child[_CharId(dstr[i][j])];
			if(node == NULL){
				break;
			}
		}

		if(node != NULL and \
			j == n - 1 and \
			node->child[_CharId(dstr[i][j])] != NULL)
		{
			node = node->child[_CharId(dstr[i][j])];
			if(what_constant_needed == 1  and  node->prob != 0.){
				answer = node->prob;
			}else
			if(what_constant_needed == 2  and  node->prob_cont != 0.){
				answer = node->prob_cont;
			}else
			if(node->y != 0.){
				answer = node->y;
			}
		}
	}
	return answer;
}

TTypoTrie* TTypoTrie::SearchNode(char *str){
	int i, size = strLen(str);
	TTypoTrie *node = this;
	
	for(i = 0; i < size - 1; i++){
		node = node->child[_CharId(str[i])];
		if(node == NULL){
			break;
		}
	}
	if(node != NULL && node->child[_CharId(str[i])] != NULL){
		node = node->child[_CharId(str[i])];
		
		if(not ((i == size - 1) and node->prob != 0.) ){
			node = NULL;
		}
	}
	return node;
}

TTypoTrie* TTypoTrie::SearchNode(string &str){
	int i, size = str.size();
	TTypoTrie *node = this;
	
	for(i = 0; i < size - 1; i++){
		node = node->child[_CharId(str[i])];
		if(node == NULL){
			break;
		}
	}
	if(node != NULL && node->child[_CharId(str[i])] != NULL){
		node = node->child[_CharId(str[i])];
		
		if(not ((i == size - 1) and node->prob != 0.) ){
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
		stream << " + " << prob << " : " << prob_cont << " : " << y;
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
		cout << " - " << prob << " : " << prob_cont << " : " << y;
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
	input.read(reinterpret_cast<char*>(&prob_cont), sizeof prob_cont);
	input.read(reinterpret_cast<char*>(&y), sizeof y);
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
	ifstream fKnowedWs("x_workdicts/know_ws.txt");
	ifstream input(fname, ios::binary);
	
	if(!fKnowedWs.is_open() || !input.is_open()){
		return false;
	}
	
	// input.read(reinterpret_cast<char*>(&uniSum), sizeof uniSum);
	// input.read(reinterpret_cast<char*>(&biSum), sizeof biSum);
	// input.read(reinterpret_cast<char*>(&triSum), sizeof triSum);
	input.read(reinterpret_cast<char*>(&def_prob), sizeof def_prob);
	if(_LoadRecursive(input)){
		input.close();

		int *matrices[] = {&mDel[0][0], &mAdd[0][0], &mSub[0][0], &mRev[0][0], &chrs[0][0], &chr[0]};
		// int tt[][2] = {{27, 26},{27,26},{26,26},{26,26},{27,26},{27,1}};
		readMatrices(matrices);
		// for(int i = 0; i < 6; i++){
		// 	__printMatrices(matrices[i], tt[i][0], tt[i][1]);
		// }
		string line;
		knowed_ws = new TTypoTrie();
		while(getline(fKnowedWs, line)){
			knowed_ws->Include(line);
		}
		fKnowedWs.close();
		#if _TEST_
			cout << " # dictionary loaded succesfully!" << endl;
		#endif
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
	out.write(reinterpret_cast<const char*>(&prob_cont), sizeof prob_cont);
	out.write(reinterpret_cast<const char*>(&y), sizeof y);
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
	
	// outFile.write(reinterpret_cast<const char*>(&uniSum), sizeof uniSum);
	// outFile.write(reinterpret_cast<const char*>(&biSum), sizeof biSum);
	// outFile.write(reinterpret_cast<const char*>(&triSum), sizeof triSum);
	outFile.write(reinterpret_cast<char*>(&def_prob), sizeof def_prob);
	if(_SaveRecursive(outFile)){
		outFile.close();
		#if _TEST_ 
			cout << " # dictionary saved succesfully!" << endl;
		#endif
		return true;
	}else{
		#if _TEST_ 
			cout << " WTF!?" << endl;
		#endif
		outFile.close();
		return false;
	}
	
	
}

//Recursive functions - include and create branch:

bool TTypoTrie::_IncludeRecursive(int pos, char *str, double probOfWord, int genTypos, double probOfWord_cont, double y_const){
	if(pos == strLen(str) - 1){
		if(prob){
			if(genTypos < 0){
				// cerr << "DUPLICATE1: " << str << " " << str[pos] << " " << pos << "\n";
				return false;
			}else{
				if(prob < probOfWord){
					typoDepth = genTypos;
					prob = probOfWord;
					prob_cont = probOfWord_cont;
					y = y_const;
				}
			}
		}else{
			typoDepth = genTypos;
			if(probOfWord >= 0.){
				prob = probOfWord; // dirty trick... be careful
				prob_cont = probOfWord_cont;
				y = y_const;
			}else{
				prob = -1.;
				prob_cont = -1.;
				y = -1.;
			}
		}

		return true;
	}else{
		if(child[_CharId(str[pos + 1])]){
			return child[_CharId(str[pos + 1])]->_IncludeRecursive(pos + 1, str, probOfWord, genTypos, probOfWord_cont, y_const);
		}else{
			child[_CharId(str[pos + 1])] = _CreateBranch(pos + 1, str, probOfWord, genTypos, probOfWord_cont, y_const);
			return true;
		}
	}
}


TTypoTrie* TTypoTrie::_CreateBranch(int pos, char* str, double probOfWord, int genTypos, double probOfWord_cont, double y_const){
	TTypoTrie *root = new TTypoTrie(str[pos], this);
	
	if(pos == strLen(str) - 1){
		root->typoDepth = genTypos;
		root->parent = this;
		if(probOfWord >= 0.){
			root->prob = probOfWord; // dirty trick... be careful
			root->prob_cont = probOfWord_cont;
			root->y = y_const;
		}else{
			root->prob = -1.;
			root->prob_cont = -1.;
			root->y = -1.;
		}
	}else{
		root->prob = 0.;
		root->prob_cont = 0.;
		root->y = 0.;
		root->child[_CharId(str[pos + 1])] = root->_CreateBranch(pos + 1, str, probOfWord, genTypos, probOfWord_cont, y_const);
	}
	
	return root;
}


bool TTypoTrie::_IncludeRecursive(int pos, string &str, double probOfWord, int genTypos, double probOfWord_cont, double y_const){
	if(pos == str.size() - 1){
		if(prob){
			if(genTypos < 0){
				// cerr << "DUPLICATE2: " << str << " " << str[pos] << " " << pos << "\n";
				return false;
			}else{
				if(prob < probOfWord){
					typoDepth = genTypos;
					prob = probOfWord;
					prob_cont = probOfWord_cont;
					y = y_const;
				}
			}
		}else{
			typoDepth = genTypos;
			if(probOfWord >= 0.){
				prob = probOfWord; // dirty trick... be careful
				prob_cont = probOfWord_cont;
				y = y_const;
			}else{
				prob = -1.;
				prob_cont = -1.;
				y = -1.;
			}
		}

		return true;
	}else{
		if(child[_CharId(str[pos + 1])]){
			return child[_CharId(str[pos + 1])]->_IncludeRecursive(pos + 1, str, probOfWord, genTypos, probOfWord_cont, y_const);
		}else{
			child[_CharId(str[pos + 1])] = _CreateBranch(pos + 1, str, probOfWord, genTypos, probOfWord_cont, y_const);
			return true;
		}
	}
}


TTypoTrie* TTypoTrie::_CreateBranch(int pos, string &str, double probOfWord, int genTypos, double probOfWord_cont, double y_const){
	TTypoTrie *root = new TTypoTrie(str[pos], this);
	
	if(pos == str.size() - 1){
		root->typoDepth = genTypos;
		root->parent = this;
		if(probOfWord >= 0.){
			root->prob = probOfWord; // dirty trick... be careful
			root->prob_cont = probOfWord_cont;
			root->y = y_const;
		}else{
			root->prob = -1.;
			root->prob_cont = -1.;
			root->y = -1.;
		}
	}else{
		root->prob = 0.;
		root->prob_cont = 0.;
		root->y = 0.;
		root->child[_CharId(str[pos + 1])] = root->_CreateBranch(pos + 1, str, probOfWord, genTypos, probOfWord_cont, y_const);
	}
	
	return root;
}


void TTypoTrie::_DeleteRecursive(){
	for(int i = 0; i < ALPHABETALL; i++){
		if(child[i] != NULL){
			child[i]->_DeleteRecursive();
			delete child[i];
		}
	}
}
void TTypoTrie::Delete(){
	_DeleteRecursive();
	delete this;
}


bool TTypoTrie::DataConstructor(){
	// char dictName[] = "fdict_bin";
	ifstream fsource("x_workdicts/kn_model.txt");
	ifstream fKnowedWs("x_workdicts/know_ws.txt");
	 
	if(!fKnowedWs.is_open() || !fsource.is_open()){
		cerr << "Error Bro: can't open some files, help me, pls\n";
		return false;
	}

	// extract все миллион слов что есть в модели
	def_prob = 100000.;
	double prob1, prob2, y;
	int delim, i;
	string str, line;
	// ifstream fsource(argv[1]);
	fsource.clear();
	std::istringstream iss("");
	// knowed_ws = new TTypoTrie();
	knowed_ws = new TTypoTrie();
	while(getline(fKnowedWs, line)){
		knowed_ws->Include(line);
	}
	fKnowedWs.close();
	while(true){
		if(!getline(fsource, str, '\t')){
			break;
		}
		// delim = str.find("<s>");
		// while(delim != string::npos){
		// 	// cout << ".!. " << endl;
		// 	str.replace(delim, 3, "#");
		// 	delim = str.find("<s>");
		// }
		// delim = str.find("</s>");
		// while(delim != string::npos){
		// 	// cout << ".!.  " << endl;
		// 	str.replace(delim, 4, "$");
		// 	delim = str.find("</s>");
		// }
		if(!getline(fsource, line)){
			break;
		}
		iss.str(line); // меняем источник
		iss.clear(); // эта функция сбрасывает все флаги потока позволяя считать его заново
		i = 0;
    	while (iss >> y) {
    		if(++i > 3){
    			cerr << "WTF BRO" << endl;
    		}else
    		if(i == 1){
    			prob1 = y;
    		}else
    		if(i == 2){
    			prob2 = y;
    		}
	    }

		// пока по умолчанию вероятность для слова - это минимальная вероятность
		if(prob1 < def_prob){
			def_prob = prob1;
		}
		Include(str, prob1, -1, prob2, y);
	}
	
	fsource.close();

	// read confusion matrices
	// 	int mDel[27][26];
	// 	int mAdd[27][26];
	// 	int mSub[26][26];
	// 	int mRev[26][26];
	// 	int chrs[27][26];
	// 	int  chr[26];
	int *matrices[] = {&mDel[0][0], &mAdd[0][0], &mSub[0][0], &mRev[0][0], &chrs[0][0], &chr[0]};
	// int tt[][2] = {{27, 26},{27,26},{26,26},{26,26},{27,26},{27,1}};
	readMatrices(matrices);

	#if _TEST_ 	
		cout << " # dictionary constructed!" << endl;
	#endif
		
	return true;
}


	// Construct top of variants:

inline double ProbMoreThan(double errChanelProb, int typoDist, double freq, double pairExists_prob, double exists_coef){
	// freq = errChanelProb * freq;
	freq = errChanelProb * exists_coef; // ПОТОМУ ЧТО ТЕПЕРЬ ТЫ УМНОЖАЕШЬ НА LANG-MODEL Кнессера-Нея в spell.cpp
							//  ТАК ЧТО ТУТ ТОЛЬКО Channel probability 
	// if(mabs(pairExists_prob) >= freq){
	if(pairExists_prob >= freq){
		freq = 0.;
	}

	return freq;
}

void CheckTopOf(list<TWordNProb*> *List, char *newbie, double errChanelProb, int typoDist, double freq){
	list<TWordNProb*>::iterator str1 = List->begin();
	double temp, exist_modif_coef = pow(REDUCE_BASE, REDUCE_COEF);;
	for(; str1 != List->end(); str1++){
		if(knowed_ws->Search(newbie) == FAILEDSEARCH){
			temp = ProbMoreThan(errChanelProb, typoDist, freq, (*str1)->prob, exist_modif_coef * ALPHA_MODEL);
		}else{
			temp = ProbMoreThan(errChanelProb, typoDist, freq, (*str1)->prob, ALPHA_MODEL);
		}
		
		if(temp){
			TWordNProb *pair = new TWordNProb;
			
			pair->word = strCpyCreate(newbie);
			pair->prob = temp;
			List->insert(str1, pair);
			break;
		}
	}
	if(List->size() > NUM_OF_VERS - 1){ // ЭТО ДЛЯ ДОБАВЛЕНИЯ САМОГО СЛОВА ПОТОМ В СПИСОЧЕК!
		delete[] List->back()->word;
		delete List->back();
		List->pop_back();
	}
}

void _TopExistsRecursive(TTypoTrie *rootTypos, int depth, char *str, list<TWordNProb*> *res, TTypoTrie *db){
	str[depth] = rootTypos->c;
	
	for(int i = 0; i < ALPHABETALL; i++){
		if(rootTypos->child[i]){
			_TopExistsRecursive(rootTypos->child[i], depth + 1, str, res, db);
		}
	}
	
	if(rootTypos->prob != 0){
		str[depth + 1] = 0;
		double frequency = db->Search(str);

		if(frequency > 0.){
			CheckTopOf(res, str, rootTypos->prob, rootTypos->typoDepth, frequency);
		}else		
		// if(frequency > 0.){
		// 	CheckTopOf(res, str, rootTypos->prob, rootTypos->typoDepth, frequency);
		// }else
		// if(rootTypos->typoDepth == PARTITION_CODE){
		// 	frequency = pow(REDUCE_BASE, REDUCE_COEF);
		// 	CheckTopOf(res, str, rootTypos->prob, rootTypos->typoDepth, frequency);
		// }

		rootTypos->prob = 0.;
	}
}

list<TWordNProb*>* GetTopExistances(TTypoTrie *sample, TTypoTrie *db, list<TWordNProb*> *res){
	//list<TWordNProb*> res;  // - каждый раз возвращая такой вывод ты вызывал копирование списка - это лютый оверхеад
	char str[MAXPATLEN];
	int depth = 0;
	
	str[MAXPATLEN] = 0;
	
	TWordNProb *dummy = new TWordNProb;
	dummy->word = strCpyCreate(" ");
	dummy->prob = 0.;
	res->push_back(dummy);

	if(sample->c == '_'){
		for(int i = 0; i < ALPHABETALL; i++){
			if(!sample->child[i]){
				continue;
			}
			_TopExistsRecursive(sample->child[i], depth, str, res, db);
		}
	}else{
		_TopExistsRecursive(sample, depth, str, res, db);
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

// не обошлось и без быдло кода. УВЫ, обычный симиляр генерейт - чудо творение моего мозга - сейчас его не перепилить 
//    ещё и под корректную работу с одним символом.... 
//    просто сделаем отдельно обработку его: deletions и subst

TTypoTrie* __SimilarGenerate_4_ONE_CHR(char *pattern, int typosLvl, TTypoTrie *db, double prevProb, double coeff){
	char str1[3], str2[3]; // третий под терминальный символ
	str1[1] = pattern[0]; // на самом деле символ один, но преобразование то сделать надо из строки в один чарик
	str2[0] = pattern[0];
	str1[2] = 0;
	str2[2] = 0;
	int id, i;
	double prob;

	id = _CharId(pattern[0]);
	//Insert : когда к паттерну прибавить надо букву и получить исходное слово
	for(i = 0; i < ALPHABET; i++){
		str1[0] = (char)(97 + i);
		str2[1] = (char)(97 + i);
		prob = ChannelProb(mAdd[26][i], chrs[26][i], coeff, prevProb);
		db->Include(str1, prob, typosLvl);
		prob = ChannelProb(mAdd[id][i], chrs[id][i], coeff, prevProb);
		db->Include(str2, prob, typosLvl);
	}

	//Substit   : исходный символ был по ошибке 
	str1[1] = 0;
	for(i = 0; i < ALPHABET; i++){
		str1[0] = (char)(97 + i);
		prob = ChannelProb(mSub[i][id], chr[id], coeff, prevProb);
		db->Include(str1, prob, typosLvl);
	}

	return db;
}

// IF WORD be length 1 its so bad.
TTypoTrie* __SimilarGenerate(char *pattern, int typosLvl, TTypoTrie *db, double prevProb, double coeff){
	// int *matrices[] = {&mDel[0][0], &mAdd[0][0], &mSub[0][0], &mRev[0][0], &chrs[0][0], &chr[0]};
	// int tt[][2] = {{27, 26},{27,26},{26,26},{26,26},{27,26},{27,1}};
	// readMatrices(matrices);
	// for(int i = 0; i < 6; i++){
	// 	__printMatrices(matrices[i], tt[i][0], tt[i][1]);
	// }


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
			prob = ChannelProb(mRev[id1][id2], chrs[id1][id2], coeff, prevProb);
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
		prob = ChannelProb(mDel[26][id2], chr[26], coeff, prevProb);
		db->Include(spStr[pos], prob, typosLvl); 
		
		for(i = 1; i < plen - 1; i++){
			spStr[++pos] = strCpyCreate(pattern, 0, i);
			// spStr[++pos] = strCpyCreate(pattern, i + 1, plen - strLen(spStr[pos - 1]) - 1);
			spStr[++pos] = strCpyCreate(pattern, i + 1);
			
			str = strCpyFrom(spStr[pos - 1], str);
			str = strCpyBackFrom(spStr[pos], str);
			
			id1 = id2;
			id2 = _CharId(pattern[i]);
			prob = ChannelProb(mDel[id1][id2], chr[id1], coeff, prevProb);
			db->Include(str, prob, typosLvl);
		}
		
		spStr[++pos] = strCpyCreate(pattern, 0, plen - 1);

		id1 = _CharId(pattern[plen-2]);
		id2 = _CharId(pattern[plen-1]);
		prob = ChannelProb(mDel[id1][id2], chr[id1], coeff, prevProb);
		db->Include(spStr[pos], prob, typosLvl);
	}
	
	// Substitutions
	{		
		str[0] = 'a';
		str[1] = 0;
		str = strCpyBackFrom(spStr[0], str);
		id1 = _CharId(pattern[0]);
		prob = ChannelProb(mSub[0][id2], chr[id2], coeff, prevProb); // хз почему, но было использовано id2, вместо логично id1
		//prob = ChannelProb(mSub[0][id1], chr[id1], coeff, prevProb);
		db->Include(str, prob, typosLvl);
		for(i = 1; i < ALPHABET; i++){
			str[0] = (char)(97 + i);
			prob = ChannelProb(mSub[i][id2], chr[id2], coeff, prevProb); // тоже самое
			//prob = ChannelProb(mSub[i][id1], chr[id1], coeff, prevProb);
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
			
			prob = ChannelProb(mSub[0][id2], chr[id2], coeff, prevProb);
			db->Include(str, prob, typosLvl);
			for(i = 1; i < ALPHABET; i++){
				str[k] = (char)(i + 97);
				prob = ChannelProb(mSub[i][id2], chr[id2], coeff, prevProb);
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
			prob = ChannelProb(mSub[i][id2], chr[id2], coeff, prevProb);
			db->Include(str, prob, typosLvl);
		}
	}
	
	// Insertions || IN MANUSCRIPT IT NAMED Del[x,y] => i'm renamed it. || ALL MATRICES WAS REPLACED CORRECTRLY.
	{ 
		str[0] = 'a';
		str[1] = 0;
		str = strCpyBackFrom(pattern, str);
		prob = ChannelProb(mAdd[26][0], chrs[26][0], coeff, prevProb);
		db->Include(str, prob, typosLvl);
		for(i = 1; i < ALPHABET; i++){
			str[0] = (char)(97 + i);
			prob = ChannelProb(mAdd[26][i], chrs[26][i], coeff, prevProb);
			db->Include(str, prob, typosLvl);
		}
		
		if(plen > 1){
			id1 = _CharId(pattern[0]);	

			str[0] = pattern[0];
			str[1] = 'a';
			str[2] = 0;
			str = strCpyBackFrom(spStr[0], str);
			prob = ChannelProb(mAdd[id1][0], chrs[id1][0], coeff, prevProb);
			db->Include(str, prob, typosLvl);
			for(i = 1; i < ALPHABET; i++){
				str[1] = (char)(97 + i);
				prob = ChannelProb(mAdd[id1][i], chrs[id1][i], coeff, prevProb);
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
					prob = ChannelProb(mAdd[id1][0], chr[id1], coeff, prevProb);
					db->Include(str, prob, typosLvl);
					for(i = 1; i < ALPHABET; i++){
						str[k] = (char)(i + 97);
						prob = ChannelProb(mAdd[id1][i], chr[id1], coeff, prevProb);
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
				prob = ChannelProb(mAdd[id1][0], chr[id1], coeff, prevProb);
				db->Include(str, prob, typosLvl);
				for(i = 1; i < ALPHABET; i++){
					str[k] = (char)(97 + i);
					prob = ChannelProb(mAdd[id1][i], chr[id1], coeff, prevProb);
					db->Include(str, prob, typosLvl);
				}
			}
		}
		
		id1 = _CharId(pattern[plen - 1]);	// current CharId From Pattern
		str = strCpyFrom(pattern, str);
		str[plen + 1] = 0;
		for(i = 0; i < ALPHABET; i++){
			str[plen] = (char)(97 + i);
			prob = ChannelProb(mAdd[id1][i], chr[id1], coeff, prevProb);
			db->Include(str, prob, typosLvl);
		}
	}
	
	// free all busy memory

	delete[] str;
	for(i = 0; i < plen * 2 - 2; i++){
		delete[] spStr[i];
	}
	// logfile.open("wtf.txt");
	// db->xPrint(logfile);
	// logfile.close();


	return db;	//	RETURN db ITSELF, JUST TO NOTE THAT CHANGES WAS MADE
}



list<TWordNProb*>* __SimilarTo(char* pattern, list<TWordNProb*> *similarList, TTypoTrie *typos, TTypoTrie *dict, int diff){
//	list<TWordNProb*> similarList; - вывел это во внешнюю, вызывающюю часть

	// int *matrices[] = {&mDel[0][0], &mAdd[0][0], &mSub[0][0], &mRev[0][0], &chrs[0][0], &chr[0]};
	// int tt[][2] = {{27, 26},{27,26},{26,26},{26,26},{27,26},{27,1}};
	// readMatrices(matrices);
	// for(int i = 0; i < 6; i++){
	// 	__printMatrices(matrices[i], tt[i][0], tt[i][1]);
	// }

	int plen = strLen(pattern);
	if(plen == 0){
		return similarList;
	}
	// cout << "k = 1... ";	
	TTypoTrie *node;

	t_all = 0;
	t = 0;

	if(plen == 1){
		typos = __SimilarGenerate_4_ONE_CHR(pattern, 1, typos);

		node = typos;
	}
	else{
		typos = __SimilarGenerate(pattern, 1, typos);

		// cout << "completed\n";
		// char s[] = "one";
		// cout << typos->Search(s) << endl;
		// logfile.open("wtf.txt");
		// typos->xPrint(logfile);
		// logfile.close();

		node = typos;
		
		if(diff > 1 && strLen(pattern) <= 18){
			char typoPat[MAXPATLEN];
			
			for(int j = 2; j < diff + 1; j++){
				// cout << "k = " << j << "... ";
				while((node = Traverse(node, typoPat, j))){
					if(typoPat[1] != 0){ // == /0 (т.е. концу строки)
						typos = __SimilarGenerate(typoPat, j, typos, node->prob, REDUCE_BASE);
					}
				}
				// cout << "completed\n";
			}
		}
		
		// Generate patritions:
		{
			if(plen > 1){
				int i = plen;
				double prob1 = 0., prob2 = 0.;
				char *str = new char[plen + 2];
				for( i = 1; i < plen - 1; i++){
					str = strCpyFrom(pattern, str, 0, i);
					str[i] = 0;
					prob1 = dict->Search(str);
					if(prob1 != FAILEDSEARCH){
						str[i] = ' ';
						str[i + 1] = 0;
						str = strCpyBackFrom(pattern, str, i);

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
	}

	node = typos->SearchNode(pattern);
	if(node != NULL){
		node->prob = 0.; //
	}

	// logfile.open("wtf.txt");
	// typos->xPrint(logfile);
	// logfile.close();

	similarList = GetTopExistances(typos, dict, similarList);

	list<TWordNProb*>::iterator it_str = similarList->begin();
	double temp, word_freq;

	word_freq = dict->Search(pattern);
	if(word_freq != FAILEDSEARCH){
		// word_freq = (DEF_UNI / uniSum) * pow(REDUCE_BASE, REDUCE_COEF);
		// word_freq = def_prob;

		for(; it_str != similarList->end(); it_str++){
			temp = ProbMoreThan(word_freq, 0, word_freq, (*it_str)->prob, 1 - ALPHA_MODEL);
			if(temp){
				TWordNProb *pair = new TWordNProb;
				
				pair->word = strCpyCreate(pattern);
				pair->prob = temp;
				similarList->insert(it_str, pair);
				break;
			}
		}
		if(it_str == similarList->end()){
			TWordNProb *pair = new TWordNProb;
			
			pair->word = strCpyCreate(pattern);
			pair->prob = ProbMoreThan(word_freq, 0, word_freq, 0., 1 - ALPHA_MODEL);

			similarList->insert(it_str, pair);
		}
	}
	
	if(similarList->size() == 1){
		TWordNProb *pair;
		bool have_parted = false;
		if(plen > 1){
			int succ_i, max_succ_i_1 = -1, max_succ_i_2 = -1, max_len_1 = -1, max_len_2 = -1, twice_id = -1;
			int i = plen;
			double prob1 = 0., prob2 = 0.;
			char *str1 = new char[plen + 2],*str2 = new char[plen + 2];
			for( i = 1; i < plen; i++){
				str1 = strCpyFrom(pattern, str1, 0, i);
				str2 = strCpyFrom(pattern, str2, i);
				prob1 = dict->Search(str1);
				prob2 = dict->Search(str2);
				if(prob1 != FAILEDSEARCH and max_len_1 < i){
					max_len_1 = i;
					max_succ_i_1 = i;
				}
				if(prob2 != FAILEDSEARCH and max_len_2 < plen - i){
					max_len_2 = plen - i;
					max_succ_i_2 = i;
					if(prob1 != FAILEDSEARCH){
						twice_id = i;
					}
				}
			}

			delete[] str1;
			delete[] str2;
			if(max_succ_i_1 != -1 or max_succ_i_2 != -1){
				if(twice_id != -1){
					succ_i = twice_id;
				}else
				if(max_len_1 >= max_len_2 and max_len_1 != -1){
					succ_i = max_len_1;
				}else
				if(max_len_2 > max_len_1){
					succ_i = max_len_2;
				}else{
					succ_i = -1;
				}

				if(succ_i != -1){
					pair = new TWordNProb;
					int n;
					char *split_str;
					split_str = new char[plen+2];

					split_str = strCpyFrom(pattern, split_str, 0, succ_i);
					n = strLen(split_str);
					split_str[n++] = ' ';
					split_str[n] = '\0';
					split_str = strCpyBackFrom(pattern, split_str, succ_i);

					pair->word = split_str;
					pair->prob = PARTITION_CODE;

					similarList->push_front(pair);
					have_parted = true;
				}
			}
		}
		if(not have_parted){
			pair = new TWordNProb;
			
			pair->word = strCpyCreate(pattern);
			pair->prob = ProbMoreThan(word_freq, 0, word_freq, 0., 1 - ALPHA_MODEL);

			similarList->insert(similarList->begin(), pair);
		}
	}

	if(similarList->back()->prob == 0){
		delete[] similarList->back()->word;
		delete similarList->back();
		similarList->pop_back();
	}
	
	return similarList;
}






// inline double ProbMoreThan(double errChanelProb, int typoDist, double freq, double pairExists_prob){
// 	freq = errChanelProb * freq;
// 	if(mabs(pairExists_prob) >= freq){
// 		freq = 0.;
// 	}

// 	return freq;
// }


// это старый вариант, когда на вход подаётся string, но мы уже с ними не работаем, теперь только char[]

/*list<TWordNProb*>* __SimilarTo(string pattern, list<TWordNProb*> *similarList, TTypoTrie *typos, TTypoTrie *dict, int diff){
//	list<TWordNProb*> similarList; - вывел это во внешнюю, вызывающюю часть
	
	if(pattern.size() == 0){
		return similarList;
	}
	// cout << "k = 1... ";	
	char *ss = strCpyCreate(pattern.c_str());
	TTypoTrie *node;
	if(pattern.size() == 1){
		typos = __SimilarGenerate_4_ONE_CHR(ss, 1, typos);

		node = typos;
	}
	else{
		typos = __SimilarGenerate(ss, 1, typos);

		// cout << "completed\n";
		node = typos;
		
		if(diff > 1){
			char typoPat[MAXPATLEN];
			
			for(int j = 2; j < diff + 1; j++){
				// cout << "k = " << j << "... ";
				while((node = Traverse(node, typoPat, j))){
					typos = __SimilarGenerate(typoPat, j, typos, node->prob, REDUCE_BASE);
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
	}
	//   ВАЖНЕНЬКО!
	// тут встроим вероятность того что ошибки в слове изначальном не было. 
	//    напомню - что потом node->prob (онж Channel Prob) будет умножаться на частоту самого варианта.
	node = typos->SearchNode(pattern);
	if(node == NULL){
		typos->Include(pattern, ALPHA_MODEL, 1);
	}
	else{
		node->prob = ALPHA_MODEL;
	}
	
	similarList = GetTopExistances(pattern, typos, dict, similarList);
	delete[] ss;
	
	return similarList;
}*/


/*bool TTypoTrie::DataConstructor(){
	// char dictName[] = "fdict_bin";
	
	ifstream fKnowedWs("x_workdicts/know_ws.txt");
	ifstream fUnigrams1("x_workdicts/good_counts.txt"); //  отсеил из старых wcounts.txt те, которых нет в wcounts2 - были просто совсем мисспеловские варианты
	ifstream fUnigrams2("x_workdicts/wcounts2.txt");	// понизил планку с 95к до 38к примерно с этого параметра всякая дичь идёт в wcounts.txt
	ifstream fBigrams("x_workdicts/w2_.txt");
	ifstream fTrigrams("x_workdicts/w3_.txt");
	 
	if(!fKnowedWs.is_open() || !fUnigrams1.is_open() || !fUnigrams2.is_open() || !fBigrams.is_open() || !fTrigrams.is_open()){
		cerr << "Error Bro: can't open some files, help me, pls\n";
		return false;
	}
	
	string line, str;

	double intpart, fractpart; //TEST!

	//extract knowed words... we delete unknowed words use this!
	TTypoTrie *knowed = new TTypoTrie();
	int _c = 0;
	while(getline(fKnowedWs, line)){
		knowed->Include(line);
		_c++;
	}

	int nxtDelim, delim;
	//double uniSum, biSum, triSum, num;
	double num;
	uniSum = biSum = triSum = 0.;

	// extract unigram words and counts:
	while(getline(fUnigrams1, line)){
		delim = line.find(9);
		str = line.substr(0,delim);

		num = GetDouble(line.substr(delim + 1, line.length() - str.length() - 1));

		if(knowed->Search(str) == FAILEDSEARCH){
			num *= pow(REDUCE_BASE);
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
	
	// ofstream ff("info.txt");
	// xPrint(ff);
	// ff.close();

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
	
	delete knowed;

	// read confusion matrices
	// 	int mDel[27][26];
	// 	int mAdd[27][26];
	// 	int mSub[26][26];
	// 	int mRev[26][26];
	// 	int chrs[27][26];
	// 	int  chr[26];
	int *matrices[] = {&mDel[0][0], &mAdd[0][0], &mSub[0][0], &mRev[0][0], &chrs[0][0], &chr[0]};
	int tt[][2] = {{27, 26},{27,26},{26,26},{26,26},{27,26},{27,1}};
	readMatrices(matrices);
	// for(int i = 0; i < 6; i++){
	// 	__printMatrices(matrices[i], tt[i][0], tt[i][1]);
	// }

	cout << " # dictionary constructed!" << endl;

	return true;
}*/