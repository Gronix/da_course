/*
Дальше надо проработать систему версий словосочетаний: модель у меня 3 словная и 5 вариантная.
Продумай как считать новые слова, которые уже идут после первой выбранной тройки. Как реализовать?
В общем надо это мочкануть. давай. =)
*/

#include "spell.h"

//  MAXWORDLEN 50
//  GOODWORDLEN 25
//  MAXWORDSINLINE 20
//  LANGMODEL 3
//  NUM_OF_VERS 5
//  ALPHA 0.8

TSpecString::TSpecString(char *fn){
	fill_n(good_word_seq, MAXWORDS, -1);

	ifstream fdoms(fn);

	if(!fdoms.is_open()){
		cerr << "CAN'T OPEN DOMS FILE!!" << endl;
	}

	string temp;

	markers = new TTypoTrie();

	while(getline(fdoms, temp)){
		markers->Include(temp);
	}
	temp = string("http");
	markers->Include(temp);
	temp = string("ftp");
	markers->Include(temp);
	temp = string("www");
	markers->Include(temp);

	cout << "TRIE TEST" << endl;
	markers->Print();
	cout << "========" << endl << endl;

	fdoms.close();
}

inline bool TSpecString::BadChr(char chr){
	if(chr == '@' || chr == '&' || chr == '/' || chr == '\\' || chr == '|')
		return true;
	return false;
}

inline bool TSpecString::SuspChr(char chr){
	if(chr == '.' || chr == ':')
		return true;
	return false;
}
// такой порядок спецом забил - заглавные встречаются реже прописных и цифр.
inline bool TSpecString::WasteChr(char chr, bool &haveChars){
	if( ('a' <= chr && chr <= 'z') ||
		('A' <= chr && chr <= 'Z'))
	{
		haveChars = true;
		return false;
	}else
	if('0' <= chr && chr <= '9')
	{
		return false;
	}
	return true;
}

// void TSpecString::LoadLine(char *input_str){
bool TSpecString::LoadLine(){
	bool markedw = false, haveChars = false;
	int suspid, wid, chid;
	wid = chid = 0;
	char chr = '\0';
	suspid = -1;
	good_id_count = -1;

	cin.unsetf(ios::skipws);
	while(cin >> chr && chr != '\n'){
		if(wid >= MAXWORDS){
			cerr << "OVERFLOW IN STRING!!" << endl;
			return false;
		}else
		if(chid >= MAXWORDLEN){
			cerr << "OVERFLOW IN one WORD!!" << endl;
			word[wid][chid] = 0;
			cerr << word[wid][chid] << endl;
			cerr << "last char is: " << chr << endl;
			continue;
		}

		if(chr == ' ' || chr == '\t'){
			if(chid){
				word[wid][chid] = 0;
				if(suspid >= 0){
					/*
					if(CheckWords(wid, suspid)){
						markedw = true;
					}
					*/
					markedw = CheckWords(wid, suspid);
					suspid = -1;
				}
				if(!markedw && chid < GOODWORDLEN && haveChars){
					good_word_seq[++good_id_count] = wid;
				}
				wid++;
				chid = 0;
				markedw = false;
				haveChars = false;
			}
			continue;
		}else
		if(BadChr(chr)){
			markedw = true;
		}else
		if(SuspChr(chr)){
			suspid = chid;
		}else
		if(WasteChr(chr, haveChars)){
			continue;
		}

		word[wid][chid] = toLow(chr);
		chid++;
	}

	if(chid){
		word[wid][chid] = 0;
		if(suspid >= 0){
			/*
			if(CheckWords(wid, suspid)){
				markedw = true;
			}
			*/
			markedw = CheckWords(wid, suspid);
			suspid = -1;
		}
		if(!markedw && chid < GOODWORDLEN && haveChars){
			good_word_seq[++good_id_count] = wid;
		}
	}
	good_word_seq[++good_id_count] = -1;
	word[++wid][0] = 0;
	if(chr == '\0'){
		return false;
	}
	return true;
}

void TSpecString::PrintLine(){
	bool mask[MAXWORDS];
	//char temp[MAXWORDS * MAXWORDLEN + 1];
	char *temp;
	int i = 0;
	temp = new char[MAXWORDS * MAXWORDLEN + MAXWORDS + 1]; // ещё одно + MAXWORDS для пробелов между словами
	fill_n(mask, MAXWORDS, false);
	fill_n(temp, MAXWORDS * MAXWORDLEN + MAXWORDS + 1, 0);

	cout << "good_word_seq: ";
	while(good_word_seq[i] != -1){
		cout << good_word_seq[i] << ' ';
		mask[good_word_seq[i]] = true;
		i++;
	}
	cout << endl << endl << "============" << endl;

	i = 0;
	while(word[i][0] != 0){
		if(i >= MAXWORDS){
			cout << "FATALITY - YOU MISS EOF IN word ARRAY" << endl;
			return;
		}
		cout << mask[i] << " | " << word[i] << endl;
		if(mask[i]) {
			temp = strCpyBackFrom(word[i], temp);
			temp[strLen(temp)] = 32;
		}
		i++;
	}

	cout << "Results:" << endl << temp << endl << "+++========+++" << endl;
}

// введём упрощение - если подозрительное слово (т.е. содержит ":" или "." - то исключаем его полностью)
// всё равно наш анализатор не сможет подобрать им замену. тут был бы вариант с 
// исключением и двумя словами... но наф
bool TSpecString::CheckWords(int wid, int chid){
	return true;
}
/*
bool TSpecString::CheckWords(int wid, int chid){
	char temp[10] = {0};
	int curid;
	if(word[wid][chid] == ':'){
		curid = 9;
		while(curid >= 6 && chid >= 1){
			if(9 - curid == 3 && markers->Search(temp + curid) == FAILEDSEARCH){
				return false;
			}
			temp[--curid] = word[wid][--chid];
		}
		if(curid != 9){
			if(markers->Search(temp + curid) == FAILEDSEARCH){
				return false;
			}
		}
	}else
	if(word[wid][chid] == '.'){
		curid = -1;
		while(word[wid][++chid] && curid < 2){
			temp[++curid] = word[wid][chid];
		}
		if(curid > 0){
			if(markers->Search(temp) == FAILEDSEARCH){
				return false;
			}
		}
	}

	return true;
}
*/
/*
void TStringVersions::Spell(TSpecString& spec_str){
	int i, j;
	for(i = 0, j = 0; spec_str->good_word_seq[i] <= spec_str->good_id_count; i += 3, j = 0){

		while(spec_str->good_word_seq[i] != -1){
			version[order[j]]
		}
	}
}
*/

void TStringVersions::Spell(TSpecString *spec_str, TTrie *dict){
	// если "белых" слов нет - выходим
	if(spec_str->good_id_count < 0){
		return;
	}

	int i, j, word_count;
	double maxProb;
	list<TWordNProb> *variants[LANGMODEL];
	TTrie *typos;
	word_count = 0;
	// стадия первая - набираем до 3 слов в стартовый набор
	while(word_count < LANGMODEL && word_count < spec_str->good_id_count){
		typos = new TTrie();
		//__SimilarTo(pattern, typos, trie, 2);
		variants[word_count] = __SimilarTo(pattern, typos, trie, 2);
		delete typos;
		word_count++;
	}
	// выясняем, какой же вариант самый вероятный
	maxProb = -1;
	i = 0;
	j = 0;
	while(i < )
	// стадия вторая - если ещё остались слова - по одному 
	// 		добавляем к набору и выбираем тот, что более вероятен



	while(i <= spec_str->good_id_count){
		
	}
}

// TVersion::TVersion(TStringVersions){
// }

// char TVersion::Next(){
// }

// char TVersion::Start(){
// }


int main(int argc, char *argv[]){
	// scientific;
	 
	// ifstream fdoms();
	char dictName[] = "fdict_bin";
	char fname[] = "domain_z.txt";

	TSpecString *s_string = new TSpecString(fname);
	TTrie *trie = new TTrie();
	
	if(!trie->Load(dictName)){
		trie->DataConstructor();
		trie->Save(dictName);
	}

	// fdoms.close();

	cout << "Loading lines..." << endl;
	while(s_string->LoadLine()){
		
		s_string->PrintLine();
		cout << endl << "==============" << endl << endl;

	}
	cout << "Done!" << endl;
}
