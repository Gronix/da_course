.PHONY: clean del

cc: spell.o tools.o typos.o
	g++ -o s spell.o tools.o typos.o

tools.o: tools.cpp
	g++ -c -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -o tools.o tools.cpp

typos.o: typos.cpp
	g++ -c -o typos.o typos.cpp

spell.o: spell.cpp
	g++ -c -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -g -o spell.o spell.cpp
	
clean: 
	rm *.o

del:
	rm s *.o