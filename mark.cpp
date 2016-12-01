#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <vector>

#define MAX_NGRAMS_LVL 5
#define NUMBER_OF_BANNED_SYMBOLS 1
//char bannedSymbols[] = "'\0";
char bannedSymbols[] = "'";
char **domainZones3len;
char **domainZones2len;
//char bannedSymbols[] = {'\'', '\0'};

using namespace std;


int strLen(const char *str){
	int len = -1;
	while(str[++len]);
	return len;
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


//bool _OneOfThem(char ch, int &idx, char *list, int listLen){
inline
bool _CharOneOfThem(char ch, char *list){
	while(*list){
		if(ch == *list++){
			return true;
		}
	}
	return false;
}

inline
bool _WordOneOfThem(char *word, char **list){
	char *curStr;
	int pos;
	while(*list){
		curStr = *list++;
		pos = 0;
		while(*curStr++ == word[pos++] and *curStr);
		if(*curStr == word[pos]){
			return true;
		}
	}
	return false;
}

inline
//int* MarkStr(char *str, int *markInfo){
void MarkStr(char *str, int *markInfo){
	char c;
	int pos = 0, id = 0, pointExist = 0;
	bool markedWord = false;
	markInfo[1] = 0;  // start position of the first word
	while(str[pos]){
		if(str[pos] == '.'){
			//if(pos > 3){
			if(pos > 2){
				if(str[pos - 3] == 'w' and str[pos - 2] == 'w' and str[pos - 1] == 'w'){
					markedWord = true;
				}
			}
			pointExist = pos;
		}
		if(str[pos] == ' '){
			if(markedWord){
				markInfo[id] = 0;
			}else{
				if(pointExist){
					// in difference of positions of string included hidden one integer (i.e. 1)
					if(pos - pointExist - 1 >= 3){
						c = str[pointExist + 3 + 1];
						str[pointExist + 3 + 1] = 0;
						if(_WordOneOfThem(str + pointExist + 1, domainZones3len)){
							str[pointExist + 3 + 1] = c;
							pointExist = pos;  // this need for not execute checking with 'domainZones2len'
							markInfo[id] = 0;
						}else{
							str[pointExist + 3 + 1] = c;
						}
					}
					if(pos - pointExist - 1 >= 2){
						c = str[pointExist + 2 + 1];
						str[pointExist + 2 + 1] = 0;
						if(_WordOneOfThem(str + pointExist + 1, domainZones2len)){
							markInfo[id] = 0;
						}else{
							markInfo[id] = 1;  // in this way: no more variants avaible - word is valid
						}
						str[pointExist + 2 + 1] = c;
					}
				}else{
					markInfo[id] = 1;  // in this way: no more variants avaible - word is valid
				}
			}
			markedWord = false;
			pointExist = 0;
			markInfo[id + 3] = pos + 1; // becouse start of the next word is position of ' ' plus one
			id += 2;
		}
		pos++;
	}

	// process last word that shouldn't be proccesed:
	if(markedWord){
		markInfo[id] = 0;
	}else{
		if(pointExist){
			// in difference of positions of string included hidden one integer (i.e. 1)
			if(pos - pointExist - 1 >= 3){
				c = str[pointExist + 3 + 1];
				str[pointExist + 3 + 1] = 0;
				if(_WordOneOfThem(str + pointExist + 1, domainZones3len)){
					str[pointExist + 3 + 1] = c;
					pointExist = pos;  // this need for not execute checking with 'domainZones2len'
					markInfo[id] = 0;
				}else{
					str[pointExist + 3 + 1] = c;
				}
			}
			if(pos - pointExist - 1 >= 2){
				c = str[pointExist + 2 + 1];
				str[pointExist + 2 + 1] = 0;
				if(_WordOneOfThem(str + pointExist + 1, domainZones2len)){
					markInfo[id] = 0;
				}else{
					markInfo[id] = 1;  // in this way: no more variants avaible - word is valid
				}
				str[pointExist + 2 + 1] = c;
			}
		}else{
			markInfo[id] = 1;  // in this way: no more variants avaible - word is valid
		}
	}
	// position of last word was set
	id += 2;
	
	markInfo[id] = 0;
	markInfo[id + 1] = -1;
	
	//return markInfo;
}

inline
char* PrepareStr(char *origStr, char *resStr, int *markInfo, 
					bool &fstDifference, 
					int &idx1, int &idx2, int &shift
																	){
	fstDifference = true;
	shift = 0;
	for(idx1 = 0;; idx1 += 2){
		if(markInfo[idx1]){
			// in 'markInfo' coded structure: {flag, position of str}
			idx2 = markInfo[idx1 + 1] - 1;
			do{
				idx2++;
				if(fstDifference){
					for(; _CharOneOfThem(origStr[idx2], bannedSymbols)
						; idx2++, shift++)
					{
						if(fstDifference){
							resStr = strCpyCreate(origStr);
							fstDifference = false;
						}
					}
				}else{
					while(_CharOneOfThem(origStr[idx2], bannedSymbols)){
						idx2++;
						shift++;
					}
				}
				//curCh = *tmpChP;
				if(shift){
					resStr[idx2 - shift] = origStr[idx2];
				}
				//curCh = *tmpChP
			}while(origStr[idx2] != ' ' and origStr[idx2] != 0);
		}else{
			if(markInfo[idx1 + 1] == -1){
				if(not shift){ // becouse in general cycle we are already set '\0', if shift was exist
					resStr = origStr;
					//resStr[idx2 - shift] = 0;
				}
				break;
			}
			idx2 = markInfo[idx1 + 1];
			if(shift){
				//idx2++;
				while(origStr[idx2] != ' ' and origStr[idx2] != 0){
					resStr[idx2 - shift] = origStr[idx2];
					idx2++;
				}
			}
			if(origStr[idx2] == 0){
				resStr[idx2 - shift] = 0;
				break;
			}
			/* 
			if(fstDifference){
				resStr = strCpyCreate(origStr);
				fstDifference = false;
			}
			
			// see position of next string: if 'pos' == -1 => this is indicator of end of string
			if(markInfo[idx1 + 3] == -1){ 
				resStr[idx2 - shift] = 0;
				break;
			}
			*/
		}
	}
	
	return resStr;
}

 
bool CorrectStr(char *originStr, int &frameLen, int *markInfo, char *resultStr){
	return false;
}

 
int main(int argc, char *argv[]){
	
	if(argc < 1){
		cerr << "Sorry bro - i need a input string" << endl;
		return 1;
	}
	
	char domain[4];
	char dzFileName[] = "domain zones.txt";
	
	ifstream dzFile;
	dzFile.open(dzFileName);
	if(!dzFile.is_open()){
		cerr << "can't open domain zone file!" << endl;
		return 2;
	}
	
	// READ DOMAIN ZONES:
	
	// count len of 'domain zones' file:
	
	int id = 0;
	while(dzFile.getline(domain, 5)){
		if(domain[0] == '-'){
			break;
		}
		id++;
	}
	domainZones3len = new char* [id + 1];
	domainZones3len[id] = NULL;
	id = 0;
	while(dzFile.getline(domain, 5)){
		id++;
	}
	domainZones2len = new char* [id + 1];
	domainZones2len[id] = NULL;
	
	// end read of lenght. Now start read domain zones:
	dzFile.clear();
	dzFile.seekg(0);
	id = -1;
	while(dzFile.getline(domain, 4)){
		if(domain[0] == '-'){
			break;
		}
		domainZones3len[++id] = strCpyCreate(domain);
	}
	id = -1;
	while(dzFile.getline(domain, 3)){
		domainZones2len[++id] = strCpyCreate(domain);
	}
	dzFile.close();
	// END OF READ DOMAIN ZONES.
	
	
	char correctStr[1000] = {0};
	char *str, *tempStrPointer;
	// shiftInTSP = shift in tempStrPointer from origin string
	int frameLen, shiftInTSP, markInfo[2 * 100] = {0};
	
	int tmpIdx1, tmpIdx2;
	bool tmpFlag;
	
	//markInfo = MarkStr(argv[1], markInfo);
	MarkStr(argv[1], markInfo);
	str = PrepareStr(argv[1], str, markInfo, tmpFlag, tmpIdx1, tmpIdx2, shiftInTSP);
	
	frameLen = 0;
	tempStrPointer = str;
	
	for(int i = 0;; i += 2){
		if(markInfo[i] == 0){
			if(frameLen){
				if(markInfo[i + 1] != -1){
					tempStrPointer[markInfo[i+1] - 1 - shiftInTSP] = 0;
					
					if(CorrectStr(tempStrPointer, frameLen, markInfo, correctStr)){
						cout << correctStr;
					}else{
						cout << tempStrPointer;
					}
					cout << ' ';
					tempStrPointer[markInfo[i+1] - 1 - shiftInTSP] = ' ';
					
					tempStrPointer = str + markInfo[i+1] - shiftInTSP;
				}else{
					if(CorrectStr(tempStrPointer, frameLen, markInfo, correctStr)){
						cout << correctStr;
					}else{
						cout << tempStrPointer;
					}
					cout << '\n';
				}
			}
			if(markInfo[i + 1] == -1){
				break;
			}else
			if(markInfo[i + 3] == -1){
				cout << tempStrPointer << '\n';
				break;
			}else{
				tempStrPointer[markInfo[i+3] - 1 - shiftInTSP] = 0;
				cout << tempStrPointer;
				tempStrPointer[markInfo[i+3] - 1 - shiftInTSP] = ' ';
				tempStrPointer = str + markInfo[i+3] - shiftInTSP;
			}
		}else{
			frameLen++;
		}
	}
	
	return 0;
}

