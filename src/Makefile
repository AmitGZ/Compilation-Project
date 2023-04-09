all: cpm

cpm.tab.c cpm.tab.h:	cpm.y
	bison -t -d cpm.y

lex.yy.c: cpm.l cpm.tab.h
	flex cpm.l

cpm: lex.yy.c cpm.tab.c cpm.tab.h
	gcc -o ../bin/cpm cpm.tab.c lex.yy.c MipsWriter.c Hash.c

clean:
	rm ../bin/cpm cpm.tab.c lex.yy.c cpm.tab.h ../bin/*.s ../bin/*.lst

compile:
	../bin/cpm "../bin/$(file)"