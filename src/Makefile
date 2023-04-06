all: calc

calc.tab.c calc.tab.h:	calc.y
	bison -t -d calc.y

lex.yy.c: calc.l calc.tab.h
	flex calc.l

calc: lex.yy.c calc.tab.c calc.tab.h
	gcc -o ../bin/calc calc.tab.c lex.yy.c MipsWriter.c Hash.c

clean:
	rm ../bin/calc calc.tab.c lex.yy.c calc.tab.h ../bin/mips.s

compile:
	../bin/calc "../TestFiles/input1.txt" "../bin/output.txt" "../bin/mips.s"