#include "tools.h"
// #include <std::string>
	// std::string functions:

int strLen(const char *str){
	int len = -1;
	while(str[++len]);
	return len;
}

// UnSafe function: dont check str "dest" to memory corruptions!
char* strCpyFrom(char *origin, char *dest, int start, int len){
	if(len == 0){
		origin += start;
		while(*origin){
			dest[len] = *origin;
			len++;
			origin++;
		}
		dest[len] = 0;
	}else{
		dest[len] = 0;
		len--;
		while(len >= start){
			dest[len] = origin[len];
			len--;
		}
	}
	return dest;
}

	
char* strCpyCreate(const char *origin, int start, int len){
	int plen = strLen(origin);
	char *res;
	if(len == 0){
		res = new char[plen - start + 1];
	}else{
		if(plen - len < 0){
			std::cerr << "Error in strCpy: plen - clen < 0: " << 
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


// UnSafe function: dont check str "dest" to memory corruptions!
char* strCpyBackFrom(char *origin, char *dest, int start, int len){
	if(len == 0){
		len = strLen(dest);
		origin += start;
		while(*origin){
			dest[len] = *origin;
			len++;
			origin++;
		}
		dest[len] = 0;
	}else{
		int olen = start + len - 1;
		len += strLen(dest);
		dest[len] = 0;
		len--;
		while(olen >= start){
			dest[len] = origin[olen];
			len--;
			olen--;
		}
	}
	return dest;
}

char toLow(char chr){
	if('A' <= chr && chr <= 'Z'){
		chr += 32;
	}
	return chr;
}

	// Secondary functions:


int mabs(int x){
	if(x < 0){
		x *= -1;
	}
	return x;
}

double mabs(double x){
	if(x < 0){
		x *= -1;
	}
	return x;
}


double pow(double x, int n){
	double res = 1.0;
	for(int i = 0; i < n; i++){
		res *= x;
	}
	
	return res;
}



int _CharId(char c){
	if(65 <= c and c < 91){
		c -= 65;
	}else
	if(97 <= c and c < 123){
		c -= 97;
	}else
	if(c == ' ' or c == '\t'){
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
		std::cerr << "WRONG CHAR! : " << (int)c << " " << c << "\n";
	}
	return c;
}


double GetDouble(std::string str){
	int i = -1;
	double x = 0.;
	while(str[++i]){
		x = (str[i] - 48) + x*10.;
	}
	return x;
}

double GetDouble(char *str){
	int i = -1;
	double x = 0.;
	while(str[++i]){
		x = (str[i] - 48) + x*10.;
	}
	return x;
}

int GetInt(std::string str){
	int i = -1, x = 0;
	while(str[++i]){
		x = str[i] - 48 + x*10;
	}
	return x;
}

int GetInt(char *str){
	int i = -1, x = 0;
	while(str[++i]){
		x = str[i] - 48 + x*10;
	}
	return x;
}

	// Read matricies:
	
void packRow(int *matrix, int row, std::string line){
	int num, i;
	
	i = -1;
	for(int col = 0; col < 25; col++){
		num = 0;
		while(line[++i] != ' '){
			num = (line[i] - 48) + 10 * num;
		}
		*(matrix + row * 26 + col) = num;
	}
	num = 0;
	while(line[++i] != 0){
		num = (line[i] - 48) + 10 * num;
	}
	*(matrix + row * 26 + 25) = num;
}


void __printMatrices(int *matrix, int n, int m){
	for(int i = 0; i < n; i++){
		std::cout << *(matrix + i * m);
		for(int j = 1; j < m; j++){
			std::cout << " " << *(matrix + i * m + j);
		}
		std::cout << std::endl;
	}
}


void readMatrices(int *matrices[]){
	char sdel[] = "_del_counts.txt";   	// количество раз, когда x было ошибочно написано как xy
	char sadd[] = "_add_counts.txt";   	// -||-  ху было ошибочно написано как х
	char ssub[] = "_subst_counts.txt"; 	// -||-  у было написано как х
	char srev[] = "_rev_counts.txt";   	// -||-  ху было написано как ух
	char chrs[] = "_chars.txt";			// -||-  xy встречается в тестовом корпусе
	char  chr[] = "_char.txt";			// -||-  x встречается в тестовом корпусе
	
	std::string line;
	int row;
	
	char *fName[] = {sdel, sadd, ssub, srev, chrs};
	
	for(int id = 0; id < 5; id++){
		std::ifstream file(fName[id]);
		row = 0;
		while(getline(file, line)){
			packRow(matrices[id], row, line);
			row++;
		}
		file.close();
	}

	// read 'char' matrix
	int i = -1;
	int num = 0;
	row = -1;
	std::ifstream file(chr);
	while(getline(file, line)){
		while(line[++i]){
			num = (line[i] - 48) + 10 * num;
		}
		*(matrices[5] + (++row)) = num;
		num = 0;
		i = -1;
	}
	file.close();

}