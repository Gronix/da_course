#ifndef __SPELLCH_H__
#define __SPELLCH_H__

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "tools.h"
#include "typos.h"

#define EPSILON 0.00000001 // = 10^-8 чтобы не обнулять все меньшие вероятности
#define MAXWORDLEN 50
#define GOODWORDLEN 25
#define MAXWORDS 20
#define LANGMODEL 3

#define COEF_4_CHANNEL_PROB  0.4
#define COEF_4_LANG_MODEL_PROB  1.

using namespace std;

ofstream logfile;

typedef struct {
	char w[MAXWORDS][MAXWORDLEN];
	int len;
} TCStrPool;
TCStrPool cstr_pool;

class TSpecString{
public:
	char *word[MAXWORDS+1];
	int good_word_seq[MAXWORDS+1];
	int good_id_count; // НАЧИНАЕТСЯ С _0_ 

	TSpecString(char*);

	bool LoadLine();
	void PrintLine();

	void Delete();
private:
	bool BadChr(char chr);
	bool SuspChr(char chr);
	bool WasteChr(char chr, bool &haveChars);

	bool CheckWords(int wid, int chid);
};

void put_str_in_pool(TCStrPool cstr_pool, string *str);
int fill_results_with_orig_word(TSpecString *s_str, char *word, deque<char*> *r_dstr, int *roadmap,  int o_wid, bool &sec_part_not_included);

inline double ComplexProbability(double KN_prob, double channel_prob, double coeff_lm = COEF_4_LANG_MODEL_PROB, double coeff_ch = COEF_4_CHANNEL_PROB);

double GetKNProb(char *word, TTypoTrie *dict);
double GetKNProb(deque<char*> &ngram, TTypoTrie *dict);

bool split_lists(list<TWordNProb*> *variants, int &list_id);
bool Spell(TSpecString *spec_str, deque<char*> *res_dstr, TTypoTrie *dict);

#endif //__SPELLCH_H__