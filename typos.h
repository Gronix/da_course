#ifndef __TYPOS_H__
#define __TYPOS_H__

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "tools.h"


#define ADDITINALCHARS 1 	// 4x : \space, -, ', . 
#define ALPHABET 26  		// |Alphabet| 
#define ALPHABETALL ALPHABET + ADDITINALCHARS
#define MAXPATLEN 100
#define NUM_OF_VERS 5
#define PARTITION_CODE 100
#define REDUCE_COEF 0.1

// #define FAILEDSEARCH -2.0
// #define MAXINT 2147483647
// #define TYPOSDEPTH 10  // typos generation depth
// #define RELIABLELEN 6 // shifted by start index of cicle = 1
// #define EPS 0.00000000001
// #define REALWORDPROB = 0.8
// #define INFINITY = 1. / 0.
// #define NAN = 0. / 0.

extern double FAILEDSEARCH;


using namespace std;

typedef struct {
	string word;
	double prob;
} TWordNProb;


inline double ComplexProbability(int confCount, int chCount, double coeff, double prevProb);


class TTypoTrie{
public:
	char c;
	double prob;
	int typoDepth;
	TTypoTrie *parent;
	TTypoTrie *child[ALPHABETALL]; 
	
	
	TTypoTrie(void);
	TTypoTrie(char ch);
	TTypoTrie(char ch, TTypoTrie *p);

	bool Include(string str, double probOfWord = -1., int genTypos = 0);
	bool Include(char *str, double probOfWord = -1., int genTypos = 0);

	// Search and Print funcitons:
	double Search(char *str);
	double Search(string str);
	TTypoTrie* SearchTypos(string str);	

	void xPrint(ofstream& stream, int lvl = 0);
	void Print(int lvl = 0);
	
	
	// Save & Load functions:
	bool _LoadRecursive(ifstream &input);
	bool Load(char *fname);
	bool _SaveRecursive(ofstream &out);
	bool Save(char *fname);
	
	
/* 	 " "  = 26 // space and tab
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

	//Recursive functions - include and create branch:

	bool _IncludeRecursive(int pos, char *str, double probOfWord, int genTypos);
	TTypoTrie* _CreateBranch(int pos, char* str, double probOfWord, int genTypos);
	bool _IncludeRecursive(int pos, string str, double probOfWord, int genTypos);
	TTypoTrie* _CreateBranch(int pos, string str, double probOfWord, int genTypos);

	bool DataConstructor();

};


	// Construct top of variants:

inline double ProbMoreThan(double errChanelProb, int typoDist, double freq, TWordNProb pairExists);
void CheckTopOf(list<TWordNProb> *List, char *newbie, double errChanelProb, int typoDist, double freq);
void _TopExistsRecursive(TTypoTrie *rootTypos, int depth, char *str, list<TWordNProb> *res, TTypoTrie *db);	
list<TWordNProb> GetTopExistances(string patt, TTypoTrie *sample, TTypoTrie *db);

	// Traverse:

TTypoTrie* _TraverseRec(TTypoTrie *root, char *pattern, int lvl, int pos);
TTypoTrie* Traverse(TTypoTrie *startNode, char *pattern, int lvl = 1);

	// Similar generate:
	// 		РАССМАТРИВАЙ pattern, КАК СЛОВО НАПИСАННОЕ ЗАВЕДОМО С ОШИБКОЙ. 
	// 		ТУТ МЫ ГЕНЕРИРУЕМ ВСЕ ВОЗМОЖНЫЕ ВЕРНЫЕ ИСХОДНЫЕ ВАРИАНТЫ СЛОВА.
TTypoTrie* __SimilarGenerate(char *pattern, int typosLvl, TTypoTrie *db, double prevProb = 1., double coeff = 1.);
list<TWordNProb> __SimilarTo(string pattern, TTypoTrie *typos, TTypoTrie *dict, int diff = 1);

#endif //__TYPOS_H__