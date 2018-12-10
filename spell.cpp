/*
Модель у меня 3 словная и 19 вариантная (20-е - само слово, моделирование варианта, когда словарное слово на самом не ошибочно).
Используется алгоритм Noisy Channel. В качестве модели ошибки берутся все слова с расстоянием Дамерау-Левенштейна = 2,
языковая же модель это Knesser-Ney Smoothing, натренированный на 200 метровом сборнике статей из англоязычных блогов на ЖЖ.
*/

#include "spell.h"


TSpecString::TSpecString(char *fn){
	int i;
	for(i = 0; i < MAXWORDS; i++){
		word[i] = new char[MAXWORDLEN];
	}
	word[i] = new char[1];
	word[i][0] = 0;
	fill_n(good_word_seq, MAXWORDS, -1);
}

inline double ComplexProbability(double KN_prob, double channel_prob, double coeff_lm, double coeff_ch){
	return abs(coeff_lm * log(KN_prob) + coeff_ch * log(channel_prob));
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
		return true;
	}
	return true;
}

bool TSpecString::LoadLine(){
	bool markedw = false, haveChars = false, have_bad_chars = true;
	int suspid, wid, chid, mir_chid;
	char chr = '\0';
	char *mirror_variant = new char[MAXWORDLEN], *temp;
	suspid = -1;
	good_id_count = -1;

	// clear old results:
	for(wid = 0; wid < MAXWORDS + 1; wid++){
		word[wid][0] = 0;
	}

	wid = chid = mir_chid = 0;
	cin.unsetf(ios::skipws);
	while(cin >> chr && chr != '\n'){
		if(wid >= MAXWORDS){
			cerr << "OVERFLOW IN STRING!! [num of word >= MAXWORDS const]" << endl;
			cerr << word[0] << " " << word[1] << " " << word[2] << " ..." << endl;
			return false;
		}else
		if(chid >= MAXWORDLEN){
			cerr << "OVERFLOW IN one WORD!!" << endl;
			word[wid][chid] = 0;
			cerr << word[wid] << endl;
			cerr << "last char is: " << chr << endl;
			continue;
		}

		if(chr == ' ' || chr == '\t'){
			if(chid){
				word[wid][chid] = 0;
				mirror_variant[mir_chid] = '\0';
				if(suspid >= 0){
					markedw = CheckWords(wid, suspid);
					if(not markedw and word[wid][suspid+1] == '\0'){
						word[wid][suspid] = '\0';
					}
					suspid = -1;
				}
				if(!markedw && chid < GOODWORDLEN && haveChars){
					good_word_seq[++good_id_count] = wid;
				}else
				if(have_bad_chars or suspid != -1){
					temp = word[wid];
					word[wid] = mirror_variant;
					mirror_variant = temp;
				}
				wid++;
				chid = mir_chid = 0;
				suspid = -1;
				markedw = have_bad_chars = haveChars = false;
			}else
			if(mir_chid){
				mirror_variant[mir_chid] = '\0';
				temp = word[wid];
				word[wid] = mirror_variant;
				mirror_variant = temp;

				mir_chid = 0;
				wid++;
			}
			continue;
		}else
		if(BadChr(chr)){
			markedw = have_bad_chars = true;
		}else
		if(SuspChr(chr)){
			if(suspid != -1){
				markedw = true;
				suspid = -2;
			}
			else{
				suspid = chid;
			}
		}else
		if(WasteChr(chr, haveChars)){
			if(32 <= chr and chr <= 126){
				mirror_variant[mir_chid++] = chr;
			}
			continue;
		}

		chr = toLow(chr);
		word[wid][chid++] = chr;
		mirror_variant[mir_chid++] = chr;
	}

	if(chid){
		word[wid][chid] = '\0';
		mirror_variant[mir_chid] = '\0';
		if(suspid >= 0){
			markedw = CheckWords(wid, suspid);
			if(not markedw and word[wid][suspid+1] == '\0'){
				word[wid][suspid] = '\0';
			}
			suspid = -1;
		}
		if(!markedw && chid < GOODWORDLEN && haveChars){
			good_word_seq[++good_id_count] = wid;
		}else
		if(have_bad_chars or suspid != -1){
			temp = word[wid];
			word[wid] = mirror_variant;
			mirror_variant = temp;
		}
	}else
	if(mir_chid){
		mirror_variant[mir_chid] = '\0';
		temp = word[wid];
		word[wid] = mirror_variant;
		mirror_variant = temp;
	}
	good_word_seq[++good_id_count] = -1;
	word[++wid][0] = 0;

	delete[] mirror_variant;

	if(chr == '\0'){
		return false;
	}
	return true;
}

void TSpecString::PrintLine(){
	bool mask[MAXWORDS];
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
// исключением и двумя словами... но нет
bool TSpecString::CheckWords(int wid, int chid){
	if(chid != 0 and word[wid][chid+1] == '\0'){
		return false;
	}
	return true;
}

void TSpecString::Delete(){
	for(int i = 0; i < MAXWORDS+1; i++){
		delete[] word[i];
	}
}

void put_str_in_pool(TCStrPool &cstr_pool, char *str){
	if(cstr_pool.len < MAXWORDS){
		strCpyFrom(str, cstr_pool.w[cstr_pool.len++]);
	}
	else{
		cerr << "ERROR:  SO MUCH YOU WANT TO LOAD INTO cstr_pool" << endl;
	}
}

double GetKNProb(char *word, TTypoTrie *dict){
	return dict->Search(word);
}

double GetKNProb(deque<char*> &ngram, TTypoTrie *dict){
	double res_prob = dict->Search(ngram);
	if(res_prob == FAILEDSEARCH){
		double temp;
		int size = ngram.size();
		res_prob = 1;
		for(int i = 1; i < size; i++){
			// т.е. как бы до предпоследнего символа: 'abc' -> y(ab) * cont(bc)
			// следующей строкой ищем 'y' коэффициент понижения, если префикса нет - 
			//		тогда ставим эпсилон. 
			temp = dict->Search(ngram, i - 1, size - 1, 3); // 'y' типа вес этого невходящего в корпус варианта
															//		относительно вероятностной массы найденных (в корпусе)
			if(temp == FAILEDSEARCH){
				temp = EPSILON;
			}
			res_prob *= temp;
			temp = dict->Search(ngram, i, size, 2); // 'prob cont'
			if(temp != FAILEDSEARCH){
				res_prob *= temp;		
				break;
			}else
			if(i == size - 1){ 	// т.е. если это последняя итерация и мы уже спустились до unigrams
				//	поясню: если мы здесь, значит такого юниграм слова в корпусе 
				//		не было (либо оно встречалось настолько редко, что не внушало доверия, 
				//		и ты его выпилил ещё на стадии предобработки [i.e. preprocessing in python])
				//	значит - либо сделать вероятность такого словосочетания совсем маленькой и вернуть чоесть,
				//		или обнулить всё к херам и сказать, что такого варианта епт, быть скорее всего не может.
				res_prob *= EPSILON;
				//	т.е. я решил (следуя рассуждениям что выше) - уменьшить... ну малоли чо! сколько многобразий в языке!
			}
		}
	}
	return res_prob;
}

int fill_results_with_orig_word(TSpecString *spec_str, 
								char *spellchecked_word,
								deque<char*> *res_dstr, 
								int *word_roadmap,  
								int orig_wid, 
								bool &sec_part_not_included)
{
	if(sec_part_not_included){
		put_str_in_pool(cstr_pool, spellchecked_word);
		res_dstr->push_back(cstr_pool.w[cstr_pool.len-1]);
		sec_part_not_included = false;
	}
	else{
		bool work_in_progress = true;
		while(work_in_progress){
			switch (word_roadmap[orig_wid]){
				case 0:
					res_dstr->push_back(spec_str->word[orig_wid]);
					break;
				case 2:
					sec_part_not_included = true;
				case 1:
					put_str_in_pool(cstr_pool, spellchecked_word);
					res_dstr->push_back(cstr_pool.w[cstr_pool.len-1]);
					work_in_progress = false;
					break;
				default:
					cerr << "some weird in your \'word_roadmap\'" << endl;
					break;
			}
			orig_wid++;
		}
	}
	return orig_wid;
}

bool split_lists(list<TWordNProb*> *variants[], int &list_id){
	int space_place;
	bool result_answer = true;
	if(variants[list_id]->front()->prob == PARTITION_CODE){
		char *word = variants[list_id]->front()->word;
		list<TWordNProb*> *new_one = new list<TWordNProb*>;
		TWordNProb *temp_pair_1, *temp_pair_2;

		temp_pair_1 = new TWordNProb;
		temp_pair_2 = new TWordNProb;
		space_place = strFind(word, ' ');

		temp_pair_1->word = strCpyCreate(word, 0, space_place);
		temp_pair_1->prob = 1.;

		temp_pair_2->word = strCpyCreate(word, space_place + 1);
		temp_pair_2->prob = 1.;

		delete[] word;
		variants[list_id]->pop_front();
		variants[list_id]->push_front(temp_pair_1);
		new_one->push_back(temp_pair_2);

		variants[++list_id] = new_one;
	}
	else{
		result_answer = false;
	}

	return result_answer;
}

char* GET_DSTR_ID_VALUE(deque<char*> &dstr, int id){
	return dstr[id];
}
						
bool Spell(TSpecString *spec_str, deque<char*> *res_dstr, TTypoTrie *dict){
	// если "белых" слов нет - выходим
	int i;
	if(spec_str->good_id_count == 0){
		i = 0;
		while(spec_str->word[i][0] != '\0'){
			res_dstr->push_back(spec_str->word[i++]);
		}
		if(i != 0){
			return true;	
		}
		return false;
	}

	int j, k, i_max, word_count, list_count, list_id;
	int word_roadmap[MAXWORDS+MAXWORDS+1], origin_word_id, good_id;
	bool worked_old_lists = false, sec_part_not_included = false;
	// string temp_str;
	double probs[NUM_OF_VERS][NUM_OF_VERS][NUM_OF_VERS], max_prob; // РАЗМЕРНОСТЬ = LANGMODEL, 
						  								 		   //   реализовать её динамически - муторно, решил напрямую

	list<TWordNProb*> *variants[LANGMODEL + LANGMODEL];
	for(i = 0; i < LANGMODEL + LANGMODEL; i++){
		variants[i] = NULL;
	}
	TTypoTrie *typos;
	word_count = list_id = list_count = origin_word_id = 0;
	// стадия первая - набираем до 3 слов в стартовый набор
	while(word_count < LANGMODEL && 
			list_count < LANGMODEL + LANGMODEL - 1 && 
			word_count < spec_str->good_id_count)
	{
		typos = new TTypoTrie();
		good_id = spec_str->good_word_seq[word_count];

		while(origin_word_id != good_id){
			word_roadmap[origin_word_id++] = 0;
		}

		variants[list_count] = __SimilarTo(spec_str->word[good_id], new list<TWordNProb*>, typos, dict, 2);
		if(variants[list_count]->size() == 1 and split_lists(variants, list_count)){
			word_roadmap[origin_word_id] = 2;
		}
		else{
			word_roadmap[origin_word_id] = 1;
		}

		typos->Delete();
		list_count++;
		word_count++;
		origin_word_id++;
	}

	if(word_count == list_count or list_count < 4){
		worked_old_lists = true;
	}
	else{
		worked_old_lists = false;
	}

	list<TWordNProb*>::iterator it1;
	max_prob = 1000000000;
	if(list_count > 1){
		if(list_count > 2){
			for(i = 0; i < NUM_OF_VERS; i++){
				for(j = 0; j < NUM_OF_VERS; j++){
					for(k = 0; k < NUM_OF_VERS; k++){
						probs[i][j][k] = 1;
					}
				}
			}
			list_id = 2;
		}
		else{
			for(i = 0; i < NUM_OF_VERS; i++){
				for(j = 0; j < NUM_OF_VERS; j++){
					probs[i][j][0] = 1;
				}
			}
			list_id = 1;
		}	

		// выясняем, какой же вариант самый вероятный

		// ЗДЕСЬ РЕКУРСИВНАЯ ФУНКЦИЯ НА ВЫЧИСЛЕНИЕ МАКСИМАЛЬНОЙ КОМБИНАЦИИ
		//		ПЕРЕБОР ОСУЩЕСТВЛЯЕТСЯ ПО ВСЕМ ПАРАМЕТРАМ
		//   ... должна была быть, но создание динамично-размерного массива слишком геморно, 
		//             так что просто закодим как есть

		// ВЫЧИСЛЯЕМ ВЕРОЯТНОСТЬ ВСЕХ ВАРИАНТОВ попутно считая самый вероятный

		double kgram_fst_prob, kgram_sec_prob;
		int j_max, k_max;
		list<TWordNProb*>::iterator it2, it3;
		char *s_;
		s_ = strCpyCreate("#");
		deque<char*> dstr;

		dstr.push_back(s_);
		dstr.push_back(s_);
		for(i = 0, it1 = variants[0]->begin(); it1 != variants[0]->end(); it1++, i++){
			dstr.push_back((*it1)->word);
			kgram_fst_prob = ComplexProbability(GetKNProb(dstr, dict), (*it1)->prob);
			dstr.pop_front();
			for(j = 0, it2 = variants[1]->begin(); it2 != variants[1]->end(); it2++, j++){
				dstr.push_back((*it2)->word);
				kgram_sec_prob = ComplexProbability(GetKNProb(dstr, dict), (*it2)->prob);

				if(list_count > 2){
					dstr.pop_front();
					for(k = 0, it3 = variants[2]->begin(); it3 != variants[2]->end(); it3++, k++){
						// по сути эта матрица нахер не нужна... но я пока оставлю, чисто ради первичного дебага, 
						//    чтобы лучше было видно, как будут менятся вероятности от допиливания кода и игр с весами

						dstr.push_back((*it3)->word);
						probs[i][j][k] *= ComplexProbability(GetKNProb(dstr, dict), (*it3)->prob) * kgram_fst_prob * kgram_sec_prob;
						if(probs[i][j][k] < max_prob){ // там как бы внутри логарифмы, поэтому после инверсии мы взяли и теперь ищем наименьшее
							max_prob = probs[i][j][k];
							i_max = i;
							j_max = j;
							k_max = k;
						}

						dstr.pop_back();
					}
					dstr.push_front(s_);
				}
				// если предложенька всего из 2 слов
				else{
					//тут уже название pair_prob теряет свой смысл и мы просто по тому значение которое там уже есть
					probs[i][j][0] *= kgram_fst_prob * kgram_sec_prob;
					if(probs[i][j][0] < max_prob){ // там как бы внутри логарифмы, поэтому после инверсии мы взяли и теперь ищем наименьшее
						max_prob = probs[i][j][0];
						i_max = i;
						j_max = j;
						k_max = 0;
					}
				}
				dstr.pop_back();
			}
			dstr.pop_back();
			dstr.push_front(s_);
		}

		// ВАЖНО! для того же дебага пока оставлю вставку первого слова в тройку
		//    но вообще оно тоже нахер не нужно, потому что я его сразу же выкидываю
		//    для работы с остатками предложения.
		// ПОТОМ можно исправить (убрать добавление первого слова в очередь)
		dstr.clear();
		int max_ids[] = {i_max, j_max, -1};
		switch(list_id){
			case 2:
				max_ids[2] = k_max;	
			case 1:
				break;
			default:
				cerr << "нето что-то с 'list_id'" << endl;
				break;
		}

		sec_part_not_included = false;

		origin_word_id = 0;
		for(i = 0; i <= list_id; i++){
			for(j = 0, it1 = variants[i]->begin(); j < max_ids[i]; j++, it1++);

			origin_word_id = fill_results_with_orig_word(spec_str, (*it1)->word, res_dstr, word_roadmap, origin_word_id, sec_part_not_included);

			dstr.push_back(res_dstr->back());
			#if _TEST_ 	
				cout << dstr.back() << " ";
			#endif
		}
		#if _TEST_ 	
			cout << "/ " << max_prob << endl;

			cout << "\t# TEST # fst res: " ;
			for(i = 0; i < res_dstr->size(); i++){
				cout << (*res_dstr)[i] << " ";
			}
			cout << "/ " << max_prob << endl;
		#endif

		// стадия вторая - если ещё остались слова - по одному 
		// 		добавляем к набору и выбираем тот, что более вероятен
		//	ДАЛЕЕ variants[] - не нужен, используем один из его слотов для хранения списков кандидатов
		while(word_count < spec_str->good_id_count or not worked_old_lists){
			if(worked_old_lists){

				origin_word_id = spec_str->good_word_seq[word_count-1] + 1;
				good_id = spec_str->good_word_seq[word_count];

				while(origin_word_id != good_id){
					word_roadmap[origin_word_id++] = 0;
				}

				// освобождение памяти под указателями в этом списке:
				if(variants[list_id] == NULL){
					variants[list_id] = new list<TWordNProb*>;
				}
				else{
					it1 = variants[list_id]->begin();
					while(it1 != variants[list_id]->end()){
						delete[] (*it1)->word;
						delete *it1;
						it1 = variants[list_id]->erase(it1);
					}
				}

				typos = new TTypoTrie();
				variants[list_id] = __SimilarTo(spec_str->word[good_id], variants[list_id], typos, dict, 2);
				typos->Delete();

				word_roadmap[origin_word_id] = 1;
				if(variants[list_id]->size() == 1){
					if(split_lists(variants, list_id)){
						worked_old_lists = false;
						word_roadmap[origin_word_id] = 2;
					}
				}

				word_count++;
			}

			//выявление из новых кандидатов наивероятнейшего
			dstr.pop_front();
			max_prob = 1000000000;
			for(i = 0, it1 = variants[list_id]->begin(); it1 != variants[list_id]->end(); it1++, i++){
				dstr.push_back((*it1)->word);
				//по традиции и для тестирования - используем последнюю степень свободы первого элемента массива (но этот массив 
				//   вообще можно выпилить и просто сделать отдельную переменную)
				//я не использую вероятность первых двух слов потому что это сути не поменяет. Я выбираю по относительному превосходству
				//   вероятности, а не по абсолютному. (эти два слова есть во всех вариантах - считай что их вычли из общей суммы)
				probs[0][0][i] = ComplexProbability(GetKNProb(dstr, dict), (*it1)->prob);
				if(probs[0][0][i] < max_prob){ // там как бы внутри логарифмы, поэтому после инверсии мы взяли и теперь ищем наименьшее
					max_prob = probs[0][0][i];
					i_max = i;
				}

				dstr.pop_back();
			}

			for(i = 0, it1 = variants[list_id]->begin(); i < i_max; i++, it1++);

			origin_word_id = spec_str->good_word_seq[word_count-2] + 1;
			fill_results_with_orig_word(spec_str, (*it1)->word, res_dstr, word_roadmap, origin_word_id, sec_part_not_included);

			dstr.push_back(res_dstr->back());
			#if _TEST_ 	
				cout << dstr.back() << " ";
			#endif

			if(not worked_old_lists){
				list_id++;
				if(list_id >= list_count){
					worked_old_lists = true;
					list_id = list_count - 2;
				}
			}
			
			#if _TEST_ 	
				cout << "\t# next: | " ;
				cout << dstr[0] << " " << dstr[1] << " " << dstr[2] << " / " << max_prob << endl;
			#endif
		}

		delete s_;
	}
	else{ 
		for(i = 0, it1 = variants[0]->begin(); it1 != variants[0]->end(); it1++, i++){
			probs[0][0][i] = ComplexProbability(GetKNProb((*it1)->word, dict), (*it1)->prob);
			if(probs[0][0][i] < max_prob){
				max_prob = probs[0][0][i];
				i_max = i;
			}
		}
		for(i = 0, it1 = variants[0]->begin(); i < i_max; i++, it1++);
		origin_word_id = 0;
		sec_part_not_included = false;
		fill_results_with_orig_word(spec_str, (*it1)->word, res_dstr, word_roadmap, origin_word_id, sec_part_not_included);
	}
 
	// ВЫВОД РЕЗУЛЬТАТА

	origin_word_id = spec_str->good_word_seq[word_count-1]; // -1 - потому что сейчас word_count указывает на следующий индекс 
															// 			за последним словом (которого собсно и нет)
	while(spec_str->word[++origin_word_id][0] != 0){
		if(origin_word_id > MAXWORDS){ // массив word имеет 'MAXWORDS + 1' элементов и '=>' индекс последнего '=' именно MAXWORDS.
			cerr << "ХЕРНЯ КАРЛ! - выход за MAXWORDS у спец_строки!!1" << endl;
			return false;
		}

		res_dstr->push_back(spec_str->word[origin_word_id]);
	}

	#if _TEST_ 	
		cout << "#TEST# finally! res: ";
		for(i = 0; i < res_dstr->size(); i++){
			cout << (*res_dstr)[i] << " ";
		}
		cout << endl << "===========" << endl;
	#endif

	// очищение памяти
	for(i = 0; i < LANGMODEL + LANGMODEL && i < list_count; i++){
		it1 = variants[i]->begin();
		while(it1 != variants[i]->end()){
			delete[] (*it1)->word;
			delete *it1;
			it1 = variants[i]->erase(it1);
		}

		delete variants[i];
	}

	return true;
}

int main(int argc, char *argv[]){

	char dictName[] = "fdict_bin";
	char fname[] = __DOMAIN_Z_FPATH__;

	TSpecString *s_string = new TSpecString(fname);
	TTypoTrie *dict = new TTypoTrie();
	
	if(!dict->Load(dictName)){
		if(not dict->DataConstructor()){
			return 1;
		}
		dict->Save(dictName);
	}

	deque<char*> *res = new deque<char*>;
	deque<char*>::iterator it;
	bool is_good = false;

	//приготавливаем пул строчек - очишаем так сказать.

	#if _TEST_ 	
		cout << "Loading lines..." << endl;
	#endif
	while(s_string->LoadLine()){
		for(int i = 0; i < MAXWORDS; i++){
			cstr_pool.w[i][0] = 0;
		}
		cstr_pool.len = 0;
		is_good = Spell(s_string, res, dict);
		if(is_good == false){
			#if _TEST_ 	
				cerr << " # nothing to return!" << endl;
			#endif
			continue;
		}
		for(it = res->begin(); it < res->end(); it++){
			cout << *it << " ";
		}
		cout << endl;
		res->clear();
		#if _TEST_ 	
			cout << endl << endl << "======-+x+-======" << endl << endl;
		#endif

	}

	knowed_ws->Delete();
	dict->Delete();
	s_string->Delete();

	delete res;
	delete s_string;
	delete dict;
	delete knowed_ws;

	#if _TEST_ 	
		cout << "Done!" << endl;
	#endif
}