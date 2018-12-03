#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <string>
#include <iostream>
#include <fstream>
#include <sstream> 
#include <iomanip>

	// std::string functions:
int strLen(const char *str);
// UnSafe function: dont check str "dest" to memory corruptions!
char* strCpyFrom(char *origin, char *dest, int start = 0, int len = 0);
char* strCpyCreate(const char *origin, int start = 0, int len = 0);
// UnSafe function: dont check str "dest" to memory corruptions!
char* strCpyBackFrom(char *origin, char *dest, int start = 0, int len = 0);
int strFind(char *str, char c, int start = 0, int len = -1);
char toLow(char);

	// Secondary functions:
inline int plus_part(int x);
inline double plus_part(double x);
int mabs(int x);
double mabs(double x);
double mpow(double x, int n = 2);
// int _CharId(char c);
double GetDouble(std::string str);
double GetDouble(char *str);
int GetInt(std::string str);
int GetInt(char *str);

	// Read matricies:
void packRow(int *matrix, int row, std::string line);
void __printMatrices(int *matrix, int n, int m);
void readMatrices(int *matrices[]);

#endif