#include "BigInt.h"

TStrTemp InputStr;
bool __strNotCreated = true;

inline bool OverflowExpCheck(TBigInt *bi){
    if(bi->len * BLOCK >= MAX_DECIMAL_DIGITS){
        if((bi->len - 1) * BLOCK < MAX_DECIMAL_DIGITS){
            // count leading zeros
            int cur = RADIX / 10, x = bi->num[bi->len - 1], i = 0;
            while(x / cur == 0){
                cur /= 10;
                ++i;
            }
            if(bi->len * BLOCK - i >= MAX_DECIMAL_DIGITS){
                return true;
            }
        }else{
            return true;
        }
    }
    return false;
}

inline void __shiftBI(TBigInt *bi, int count, bool RightDirection){
    if(count < 1){
        return;
    }

    int i, j, start, end, add;

    if(RightDirection){
        start = 0;
        end = bi->len;
        add = 1;
    }else{
        start = bi->len - 1;
        end = -1;
        add = -1;
    }

    if(bi->len > 1){
        for(j = 0; j < count; ++j){
            for(i = start; i != end - add; i += add){
                bi->num[i] = bi->num[i + add];
            }
            bi->num[i] = 0;
        }
    }else{
        bi->num[start] = 0;
    }
}

inline void LeftShiftBigInt(TBigInt *bi, int count = 1){
    __shiftBI(bi, count, 0);
}

inline void RightShiftBigInt(TBigInt *bi, int count = 1){
    __shiftBI(bi, count, 1);
}

inline void ResizeBigInt(TBigInt *bi, int shift){
    int *tmp = bi->num, i;
    bi->len += shift;
    bi->num = new int[bi->len];
    shift = ((shift < 1)? 0 : shift);
    for(i = 0; i < bi->len - shift; ++i){
        bi->num[i] = tmp[i];
    }
    for(; i < bi->len; ++i){
        bi->num[i] = 0;
    }
    delete[] tmp;
}

inline int CountZrBlcsOfBigInt(TBigInt *bi){
    //Count Zero Blocks from highest block to penult.
    int i = bi->len - 1;

    for(; i > 0 && !(bi->num[i]); --i); //!
    i = bi->len - i - 1;
    return i;
}

TBigInt* IntToBigInt(int number){
	TBigInt *bi = new TBigInt();
	//bi->len = 1;
	//bi->num = new int[1];
	//bi->num[0] = number;
	
	// how many digits in 'number':
	
	for()
	
	int len = i;
	bi->len = ((len % BLOCK)? ((len / BLOCK) + 1) : (len / BLOCK));
	bi->num = new int[bi->len];

	for(; i > len % BLOCK; i -= BLOCK){
		tmp = 0;
		for(j = BLOCK; j > 0; --j){
			tmp = tmp*10 + InputStr.str[i - j] - 48;
		}
		bi->num[++curID] = tmp;
	}
	
	if(i > 0){
		tmp = 0;
		j = -1;
		while(j < i - 1){
			tmp = tmp*10 + InputStr.str[++j] - 48;
		}
		bi->num[++curID] = tmp;
	}
	
	return bi;
}

inline TBigInt* CopyBigInt(TBigInt *bi){
    int i;
    TBigInt *Result = new TBigInt();
    Result->len = bi->len;
    Result->num = new int[Result->len];

    for(i = 0; i < Result->len; ++i){
        Result->num[i] = bi->num[i];
    }

    return Result;
}

void ProlongStr(TStrTemp &str){
    char *temp = new char[str.size];
    int i;
    
    for(i = 0; i < str.size; ++i){
        temp[i] = str.str[i];
    }
    
    delete[] str.str;
    str.str = new char[str.size * MULTIPLIER];
    
    for(i = 0; i < str.size; ++i){
        str.str[i] = temp[i];
    }
    
    str.size *= MULTIPLIER;
    delete[] temp;
}

TBigInt* ReadBigInt(){
    char digit;
    digit = std::cin.get();
    if(digit < 1){
        return NULL;
    }

    bool zero = true;
    int len, i;
    
    /* Read like normal string */
    
    if(__strNotCreated){
        InputStr.str = new char[STARTLEN];
        InputStr.size = STARTLEN;
        __strNotCreated = false;
    }

    len = InputStr.size;
    i = 0;
    while(digit > 47){
        if(zero){
            if(digit != '0'){
                zero = false;
            }
        }
        InputStr.str[i] = digit;
        if(++i == len){
            ProlongStr(InputStr);
            len = InputStr.size;
        }
        std::cin.get(digit);
    }
    InputStr.str[i] = 0;
    if(std::cin.eof()){
        return NULL;
    }
    while(digit != 10){ // Windows added \13 - char to end of line
        digit = std::cin.get();
        if(std::cin.eof()){
            return NULL;
        }
    }

    /* Translate into TBigInt */

    TBigInt *bi = new TBigInt();
    if(zero){
        bi->len = 1;
        bi->num = new int[1];
        bi->num[0] = 0;
    }else{
        int j, tmp, curID = -1;
        len = i;
        bi->len = ((len % BLOCK)? ((len / BLOCK) + 1) : (len / BLOCK));
        bi->num = new int[bi->len];

        for(; i > len % BLOCK; i -= BLOCK){
            tmp = 0;
            for(j = BLOCK; j > 0; --j){
                tmp = tmp*10 + InputStr.str[i - j] - 48;
            }
            bi->num[++curID] = tmp;
        }
        
        if(i > 0){
            tmp = 0;
            j = -1;
            while(j < i - 1){
                tmp = tmp*10 + InputStr.str[++j] - 48;
            }
            bi->num[++curID] = tmp;
        }
    }
    return bi;
}

void PrintBigInt(TBigInt *bi){
    if(!bi){
        std::cout << "Error\n";
        return;
    }
    
    int i;
    /*
    std::cout << "internal representation:\n";
    for(i = 0; i < bi->len; ++i){
        std::cout << std::setfill('0') << std::setw(4) << bi->num[i] << ".";
    }
    std::cout << "\ninteger:\n" << bi->num[bi->len -1];
    */
    std::cout << bi->num[bi->len -1];
    for(i = bi->len - 2; i > -1; --i){
        std::cout <<  std::setfill('0') << std::setw(4) << bi->num[i];
    }
    std::cout << "\n";
}

int __compareBigInt(TBigInt *First, TBigInt *Second){
    int answer;

    if(First->len > Second->len){
        answer = 1;
    }else
    if(First->len < Second->len){
        answer = -1;
    }else{
        TBigInt *dif = Minus(First,Second);
        if(dif){
            if(dif->len == 1 && dif->num[0] == 0){
                answer = 0;
            }else{
                answer = 1;
            }
            DeleteBigInt(dif);
        }else{
            answer = -1;
        }
    }

    return answer;
}

bool isEqual(TBigInt *First, TBigInt *Second){
    if(__compareBigInt(First,Second)){
        return false;
    }
    return true;
}

bool isLarger(TBigInt *First, TBigInt *Second){
    if(__compareBigInt(First,Second) == 1){
        return true;
    }
    return false;
}
bool isSmaller(TBigInt *First, TBigInt *Second){
    if(__compareBigInt(First,Second) == -1){
        return true;
    }
    return false;
}

bool isMoreOrEqual(TBigInt *First, TBigInt *Second){
    int answer = __compareBigInt(First,Second);
    if(answer == 1 || answer == 0){
        return true;
    }
    return false;
}

TBigInt* Plus(TBigInt *First, TBigInt *Second){
    int curr, excess, i, smallLen;
    
    TBigInt *Result = new TBigInt(), *larger;
    larger = ((First->len > Second->len) ? First : Second);
    smallLen = ((larger == First)? Second->len : First->len);

    Result->len = larger->len;
    Result->num = new int[Result->len];
    excess = 0;
    for(i = 0; i < smallLen; ++i){
        curr = First->num[i] + Second->num[i] + excess;
        Result->num[i] = curr % RADIX;
        excess = curr / RADIX;
    }

    for(; i < larger->len; ++i){
        curr = larger->num[i] + excess;
        Result->num[i] = curr % RADIX;
        excess = curr / RADIX;
    }

    if(excess){
        ResizeBigInt(Result,1);
        Result->num[Result->len - 1] = excess;
    }
    if(OverflowExpCheck(Result)){
        DeleteBigInt(Result);
        Result = NULL;
    }
    
    return Result;
}

TBigInt* Minus(TBigInt *First, TBigInt *Second){
    if(First->len < Second->len){
        return NULL;
    }
    
    TBigInt *Result = new TBigInt();
    Result->len = First->len;
    Result->num = new int[Result->len];
    int curr, i;
    bool debt = false;
    for(i = 0; i < Second->len; ++i){
        if(debt){
            curr = -1;
        }else{
            curr = 0;
        }
        
        curr += First->num[i] - Second->num[i];
        
        if(curr < 0){
            debt = true;
            curr += RADIX;
        }else{
            debt = false;
        }
        
        Result->num[i] = curr;
    }
    for(; i < First->len; ++i){
        if(debt){
            curr = -1;
        }else{
            curr = 0;
        }
        
        curr += First->num[i];
        
        if(curr < 0){
            debt = true;
            curr += RADIX;
        }else{
            debt = false;
        }
        
        Result->num[i] = curr;
    }

    if(debt){
        // First lower than Second -> error
        DeleteBigInt(Result);
        Result = NULL;
    }else{
        if(Result->len > 1 && !(Result->num[First->len - 1])){
            // if older digit == 0 -> resize array
            if(Result->len > 2){
                i = CountZrBlcsOfBigInt(Result);
                i *= -1;
            }else{
                i = -1;
            }
            ResizeBigInt(Result,i);
        }
    }
    
    return Result;
}

TBigInt* SimpleMulty(TBigInt *Complex, int Simple){
    int i, excess, curr;
    TBigInt *Result = new TBigInt();
    Result->len = Complex->len + 1;
    Result->num = new int[Result->len];

    excess = 0;
    for(i = 0; i < Complex->len; ++i){
        curr = Complex->num[i] * Simple + excess;
        excess = curr / RADIX;
        if(excess){
            curr %= RADIX;
        }
        Result->num[i] = curr;
    }
    if(excess){
        Result->num[i] = excess;
    }else{
        if(Simple == 0){
            i = CountZrBlcsOfBigInt(Result);    
            i *= -1;
        }else{
            i = -1;
        }
        ResizeBigInt(Result, i);
    }

    if(OverflowExpCheck(Result)){
        DeleteBigInt(Result);
        Result = NULL;
    }

    return Result;
}

TBigInt* Multy(TBigInt *First, TBigInt *Second){
    int excPlus = 0, excMult = 0, i, j, curr;
    TBigInt *Result = new TBigInt();
    Result->len = First->len + Second->len;
    Result->num = new int[Result->len];

    for(i = 0; i < Result->len; ++i){
        Result->num[i] = 0;
    }

    for(i = 0; i < First->len; ++i){
        for(j = 0; j < Second->len; ++j){
            curr = First->num[i] * Second->num[j] + excMult;
            excMult = curr / RADIX;
            if(excMult){
                curr %= RADIX;
            }
            Result->num[i+j] += curr + excPlus;
            excPlus = Result->num[i+j] / RADIX;
            if(excPlus){
                Result->num[i+j] %= RADIX;
            }
        }
        if(excPlus + excMult){
            Result->num[i+j] += excPlus + excMult;
            excPlus = Result->num[i+j] / RADIX;
            if(excPlus){
                ++(Result->num[i+j+1]);
            }
        }
        excPlus = excMult = 0;
    }

    i = CountZrBlcsOfBigInt(Result);
    if(i){
        i *= -1;
        ResizeBigInt(Result, i);
    }
    if(OverflowExpCheck(Result)){
        DeleteBigInt(Result);
        Result = NULL;
    }

    return Result;
}

TBigInt* __smplDivide(TBigInt *Complex, int smpl){
    TBigInt *Result = new TBigInt();
    int i, cur = 0, debt = 0;

    Result->len = Complex->len;
    Result->num = new int[Result->len];

    for(i = Complex->len - 1; i > -1; --i){
        if(debt){
            cur = debt * RADIX;
        }

        cur += Complex->num[i];

        if(cur < smpl){
            debt = cur;
            Result->num[i] = 0;
        }else{
            debt = cur % smpl;
            Result->num[i] = cur / smpl;
        }
        cur = 0;
    }

    i = CountZrBlcsOfBigInt(Result);
    if(i){
        i *= -1;
        ResizeBigInt(Result, i);
    }

    return Result;
}

TBigInt* SimpleDivide(TBigInt *Complex, int Simple){
    if(!Simple){ // == 0
        return NULL;
    }
    return __smplDivide(Complex, Simple);
}

TBigInt* SimpleDivide(TBigInt *Complex, TBigInt *Simple){
    if(Simple->len != 1 || Simple->num[0] == 0){
        return NULL;
    }

    int smpl = Simple->num[0];
    return __smplDivide(Complex,smpl);
}

TBigInt* Divide(TBigInt *Divident, TBigInt *Divisor){
    if(Divisor->len == 1 && Divisor->num[0] == 0){
        return NULL;
    }

    TBigInt *Result = new TBigInt();

    if(isSmaller(Divident,Divisor)){
        Result->len = 1;
        Result->num = new int[1];
        Result->num[0] = 0;
        return Result;
    }

    TBigInt *divid, *tmp, *rest;
    int i, j, left, right, middle, curResID;
    bool exit = false;
    
    divid = new TBigInt();
    divid->len = Divisor->len;
    divid->num = new int[divid->len];
    for(i = Divident->len - 1, j = divid->len - 1; (i > Divident->len - Divisor->len - 1) && (j > -1) && (i > -1); --i, --j){
        divid->num[j] = Divident->num[i];
    }

    Result->len = Divident->len;
    Result->num = new int[Result->len];

    curResID = Result->len - 1;
    Result->num[curResID] = 0;
    tmp = NULL;
    rest = NULL;

    i = Divident->len - divid->len - 1;
    do{
        j = 0;
        while(isSmaller(divid,Divisor)){
            if(i < 0){
                exit = true;
                break;
            }
            if(++j > 1){
                Result->num[curResID] = 0;
                --curResID;
            }
            if(divid->num[0] != 0 || divid->len != 1){
                ResizeBigInt(divid,1);
                LeftShiftBigInt(divid);
            }
            divid->num[0] = Divident->num[i];
            --i;
        }
        if(exit){
            // в divid лежит остаток
            break;
        }
        //ДИХОТОМИЯ
        left = 0;
        right = MAXDIGIT;
        while(left < right){
            middle = (left + right) >> 1;
            tmp = SimpleMulty(Divisor,middle);
            if(isMoreOrEqual(divid,tmp)){
                left = middle + 1;
            }else{
                right = middle - 1;
            }
            DeleteBigInt(tmp);
        }
        middle = (left + right) >> 1;
        tmp = SimpleMulty(Divisor,middle);
        if(!tmp){
            DeleteBigInt(divid);
            DeleteBigInt(Result);
            return NULL;
        }
        while(isLarger(tmp,divid)){
            j = 0;
            DeleteBigInt(tmp);
            tmp = SimpleMulty(Divisor, --middle);
            if(!tmp){
                DeleteBigInt(divid);
                DeleteBigInt(Result);
                return NULL;
            }
        }
        rest = Minus(divid,tmp);
        DeleteBigInt(divid);
        divid = rest;
        DeleteBigInt(tmp);

        Result->num[curResID] = middle;
        --curResID;
    } while(i > -1);

    if(curResID > -1){   
        ++curResID;
        RightShiftBigInt(Result,curResID);
        curResID *= -1;
        ResizeBigInt(Result,curResID);
    }

    DeleteBigInt(divid);

    return Result;
}

TBigInt* Pow(TBigInt *Number, TBigInt *N){
    if(Number->len == 1 && Number->num[0] == 0 && N->len == 1 && N->num[0] == 0){
        return NULL;
    }

    TBigInt *n = CopyBigInt(N), *Num = CopyBigInt(Number);
    TBigInt *tmp = NULL, *Result = new TBigInt(), *zero = new TBigInt();
    zero->len = Result->len = 1;
    zero->num = new int[1];
    zero->num[0] = 0;
    Result->num = new int[1];
    Result->num[0] = 1;

    while(n->len > 1 || n->num[0] != 0){
        if(n->num[0] & 1){ // число нечётное
            tmp = Multy(Num,Result);
            if(!tmp){
                DeleteBigInt(Result);
                DeleteBigInt(zero);
                DeleteBigInt(n);
                DeleteBigInt(Num);
                return NULL;
            }
            DeleteBigInt(Result);
            Result = tmp;
        }
        tmp = Multy(Num, Num);
        if(!tmp){
            DeleteBigInt(Result);
            DeleteBigInt(zero);
            DeleteBigInt(n);
            DeleteBigInt(Num);
            return NULL;
        }
        DeleteBigInt(Num);
        Num = tmp;
        tmp = SimpleDivide(n,2);
        DeleteBigInt(n);
        n = tmp;
    }

    DeleteBigInt(zero);
    DeleteBigInt(n);
    DeleteBigInt(Num);
    if(OverflowExpCheck(Result)){
        DeleteBigInt(Result);
        Result = NULL;
    }

    return Result;
}

void EndOfGame(){// delete InputStr
    delete[] InputStr.str;
}