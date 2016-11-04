#ifndef __BigInt__H___
#define __BigInt__H___

#include <iostream>
#include <iomanip>

#define MULTIPLIER 2
#define STARTLEN 1
#define RADIX 10000
#define MAXDIGIT RADIX - 1
#define BLOCK 4
#define MAX_DECIMAL_DIGITS 100000

typedef struct{
    char *str;
    int size;
} TStrTemp;

typedef struct{
    int len;
    int *num;
} TBigInt;

TBigInt* ReadBigInt();
TBigInt* IntToBigInt(int);
void ProlongStr(TStrTemp &str);
void PrintBigInt(TBigInt *bi);

TBigInt* Plus(TBigInt *First, TBigInt *Second);
TBigInt* Minus(TBigInt *First, TBigInt *Second);
TBigInt* Multy(TBigInt *First, TBigInt *Second);
TBigInt* Divide(TBigInt *Divident, TBigInt *Divisor);
TBigInt* Pow(TBigInt *Number, TBigInt *N);

TBigInt* SimpleMulty(TBigInt *Complex, int Simple);
TBigInt* SimpleDivide(TBigInt *Complex, int Simple);

bool isSmaller(TBigInt *First, TBigInt *Second);
bool isLarger(TBigInt *First, TBigInt *Second);
bool isEqual(TBigInt *First, TBigInt *Second);

inline bool OverflowExpCheck(TBigInt *bi);
inline void ResizeBigInt(TBigInt *bi, int shift);
inline void DeleteBigInt(TBigInt *bi){
	if(bi){
        delete[] bi->num;
        delete bi;
    }
}
void EndOfGame();

#endif