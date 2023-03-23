bison Bison.y -d
flex Analyzer.l
g++ Bison.tab.c lex.yy.c -o grammar
