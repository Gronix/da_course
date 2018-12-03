.PHONY: clean del

cc: spell.o tools.o typos.o
	g++ -o s obj/spell.o obj/tools.o obj/typos.o

tools.o: tools.cpp
	g++ -c -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -g -o obj/tools.o tools.cpp

typos.o: typos.cpp
	g++ -c  -g -o obj/typos.o typos.cpp

spell.o: spell.cpp
	g++ -c -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -g -o obj/spell.o spell.cpp
	
clean: 
	rm obj/*.o

del:
	rm s obj/*.o