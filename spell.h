#ifndef __SPELLCH_H__
#define __SPELLCH_H__

#include <string>
#include <deque>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "tools.h"
#include "typos.h"

#define MAXWORDLEN 50
#define GOODWORDLEN 25
#define MAXWORDS 20
#define LANGMODEL 3
//#define NUM_OF_VERS 5 - т.к. есть уже в typos.h
#define ALPHA 0.8


using namespace std;

class TSpecString{
public:
	char word[MAXWORDS+1][MAXWORDLEN];
	int good_word_seq[MAXWORDS+1];
	int good_id_count;

	TSpecString(char*);

	// void LoadLine(char *input_str);
	bool LoadLine();
	void PrintLine();
private:
	TTypoTrie *markers;

	bool BadChr(char chr);
	bool SuspChr(char chr);
	bool WasteChr(char chr, bool &haveChars);

	bool CheckWords(int wid, int chid);
};

class TStringVersions{
public:
	char version[LANGMODEL][GOODWORDLEN];
	double prob[NUM_OF_VERS][NUM_OF_VERS][NUM_OF_VERS];
	int order[LANGMODEL];

	void Spell(TSpecString& spec_str);
};

// class TVersion{
// public:
// 	TVersion(TStringVersions);
// 	char Next();
// 	char Start();
// private:
// 	TStringVersions *str;
// 	int id1, id2, id3, iword, ichr;
// };

#endif //__SPELLCH_H__