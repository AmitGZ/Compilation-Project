# Compilation-Project
The CPL (Compiler Project Language) is a compiler that translates source code written in the CPL language into executable machine code in the form of an assembly language file. The CPL compiler is implemented using the lexical analyzer Flex and parser generator Bison. The program's symbol table implementation uses a hash table of size 500 to store variables. The CPL compiler generates two output files: a <filename>.lst file containing the listing of the source code with error messages and a <filename>.s file containing the compiled assembly code.

## System Requirements
•	A C compiler that supports the C18 standard
•	Flex and Bison lexical analyzer and parser generator tools


## Installation

### using makefile

```bash
$ cd src
$ make all
$ make compile file=input1.txt
```

### manually

```bash
flex cpm.l
bison -t -d cpm.y
gcc -o ../bin/cpm cpm.tab.c lex.yy.c MipsWriter.c Hash.c
cd ../bin
./cpm “<filename>.cpl”
```

