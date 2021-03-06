#ifndef __TYPOS_H__
#define __TYPOS_H__

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <deque>
#include <cmath>
#include "tools.h"

#define _TEST_ false

	#if _TEST_
#define __KNOWED_WS_FPATH__ "x_workdicts/know_ws.txt"
#define __LANG_MODEL_FPATH__ "x_workdicts/kn_model.txt"
#define __DOMAIN_Z_FPATH__ "x_workdicts/domain_z.txt"
	#else
#define __KNOWED_WS_FPATH__ "x_workdicts/know_ws.txt"
#define __LANG_MODEL_FPATH__ "x_workdicts/kn_model.txt"
#define __DOMAIN_Z_FPATH__ "x_workdicts/domain_z.txt"
// #define __KNOWED_WS_FPATH__ "../x_workdicts/know_ws.txt"
// #define __LANG_MODEL_FPATH__ "../x_workdicts/kn_model.txt"
// #define __DOMAIN_Z_FPATH__ "../x_workdicts/domain_z.txt"
	#endif


#define ADDITINALCHARS 3 	// 3x : \space, \<s> = # = start string, \</s> = $ = end string
#define ALPHABET 26  		// |Alphabet| 
#define ALPHABETALL ALPHABET + ADDITINALCHARS
#define MAXPATLEN 100
#define NUM_OF_VERS 20
#define PARTITION_CODE 100
#define REDUCE_COEF 6
#define REDUCE_BASE 0.1
#define PROB_CONT_DEF 0.

#define ALPHA_MODEL 0.30 // это ещё надо подкручивать. но это - вероятность того, что слово не было написано с ошибкой
			// т.е. 30 процентов что слово словарное на самом деле ошибка и соотв. 70 то что правда.

extern double FAILEDSEARCH;

using namespace std;

extern ofstream logfile;

typedef struct {
	char *word;
	double prob;
} TWordNProb;


inline double ChannelProb(int confCount, int chCount, double coeff, double prevProb);
inline int _CharId(char c);


class TTypoTrie{
public:
	char c; // символ для печати особый, нахер не нужен, только для функции xPrint  и Print
	double prob;
	double prob_cont;
	double y;
	int typoDepth;
	TTypoTrie *parent;
	TTypoTrie *child[ALPHABETALL]; 
	
	
	TTypoTrie(void);
	TTypoTrie(char ch);
	TTypoTrie(char ch, TTypoTrie *p);

	bool Include(string &str, double probOfWord = -1., int genTypos = 0, double probOfWord_cont = -1., double y_const = -1.);
	bool Include(char *str, double probOfWord = -1., int genTypos = 0, double probOfWord_cont = -1., double y_const = -1.);

	// Search and Print funcitons:
	double Search(char *str, int what_const_needed = 1);
	double Search(string &str, int what_const_needed = 1);
	double Search(deque<char*> &dstr, int start = 0, int end = -1, int what_const_needed = 1);
	TTypoTrie* SearchNode(string &str);
	TTypoTrie* SearchNode(char *str);

	void xPrint(ofstream& stream, int lvl = 0);
	void Print(int lvl = 0);
	
	
	// Save & Load functions:

	bool Load(char *fname);
	bool Save(char *fname);
	bool DataConstructor();

	void Delete();
	
private:

	//Recursive functions - include and create branch:

	bool _IncludeRecursive(int pos, char *str, double probOfWord, int genTypos, double probOfWord_cont, double y_const);
	TTypoTrie* _CreateBranch(int pos, char* str, double probOfWord, int genTypos, double probOfWord_cont, double y_const);
	bool _IncludeRecursive(int pos, string &str, double probOfWord, int genTypos, double probOfWord_cont, double y_const);
	TTypoTrie* _CreateBranch(int pos, string &str, double probOfWord, int genTypos, double probOfWord_cont, double y_const);

	bool _LoadRecursive(ifstream &input);
	bool _SaveRecursive(ofstream &out);
	
	void _DeleteRecursive();

};

extern TTypoTrie *knowed_ws;
	// Construct top of variants:

inline double ProbMoreThan(double errChanelProb, int typoDist, double freq, TWordNProb pairExists, double exists_coef);
void CheckTopOf(list<TWordNProb*> *List, char *newbie, double errChanelProb, int typoDist, double freq);
void _TopExistsRecursive(TTypoTrie *rootTypos, int depth, char *str, list<TWordNProb*> *res, TTypoTrie *db);	
list<TWordNProb*>* GetTopExistances(TTypoTrie *sample, TTypoTrie *db, list<TWordNProb*> *res);

	// Traverse:

TTypoTrie* _TraverseRec(TTypoTrie *root, char *pattern, int lvl, int pos);
TTypoTrie* Traverse(TTypoTrie *startNode, char *pattern, int lvl = 1);

	// Similar generate:
	// 		РАССМАТРИВАЙ pattern, КАК СЛОВО НАПИСАННОЕ ЗАВЕДОМО С ОШИБКОЙ. 
	// 		ТУТ МЫ ГЕНЕРИРУЕМ ВСЕ ВОЗМОЖНЫЕ ВЕРНЫЕ ИСХОДНЫЕ ВАРИАНТЫ СЛОВА.
TTypoTrie* __SimilarGenerate_4_ONE_CHR(char *pattern, int typosLvl, TTypoTrie *db, double prevProb = 1., double coeff = 1.);
TTypoTrie* __SimilarGenerate(char *pattern, int typosLvl, TTypoTrie *db, double prevProb = 1., double coeff = 1.);
list<TWordNProb*>* __SimilarTo(char *pattern, list<TWordNProb*> *similarList, TTypoTrie *typos, TTypoTrie *dict, int diff = 1);

#endif //__TYPOS_H__
