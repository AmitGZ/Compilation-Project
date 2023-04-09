#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "MyStructs.h"
#include "Hash.h"

/**
 * @brief Ask the user for input
 * @param node write input value to node
 */
void MipsIn(const Node* node);

/**
 * @brief Write math operation to mips file
 * @param mulOp Mul operation to perform (DIV/MUL)
 * @param reg0 First register
 * @param reg1 Second register
 * @return Reg result register
 */
Reg MipsMathOp(MathOp mulOp, Reg reg0, Reg reg1);

/**
 * @brief Write Relational operation to mips file
 * @param relOp Relational operation to perform (EQ/NEQ/LT/GT/LE/GE)
 * @param reg0 First register
 * @param reg1 Second register
 * @return Reg result register
 */
Reg MipsRelOp(RelOp relOp, Reg reg0, Reg reg1);

/**
 * @brief Write Logical operation to mips file
 * @param logOp Logical operation to perform (AND/OR/NOR)
 * @param reg0 First register
 * @param reg1 Second register
 * @return Reg result register
 */
Reg MipsLogOp(LogOp logOp, Reg reg0, Reg reg1);

/**
 * @brief Write the declaration of a variable 
 * @param t Variable's type
 * @param id Variable's name
 * @param sval Variable's initial value
 */
void MipsDecl(Type t, const char* id, const char* sval);

/**
 * @brief Writes to mips file the exit instruction
 */
void MipsExit();

/**
 * @brief Writes the assignment of a value to a variable 
 * @param node node to assign the value to
 * @param reg The assigner reg
 */
void MipsAssign(const Node* node, Reg reg);

/**
 * @brief Writes the printing instruction to mips file
 * @param reg register to print out
 */
void MipsOut(const Reg* reg);

/**
 * @brief Initializes the data section start of file
 */
void MipsData();

/**
 * @brief Loads a variable to a temporary register
 * @param node Variable to load
 * @return Reg the register that the variable was loaded into
 */
Reg MipsLoadVar(const Node* node);

/**
 * @brief Loads an immediate value to a temporary register
 * @param val value to load
 * @return Reg the register that the value was loaded into
 */
Reg MipsLoadImmediate(const Val* val);

/**
 * @brief Cast a register to a different type
 * @param reg Register to cast
 * @param t Type to cast to
 * @return Reg Casted register
 */
Reg MipsCast(Reg reg, Type t);

/**
 * @brief Initializes main section label in mips file
 */
void MipsMain();

/**
 * @brief Register manager, allocates a temporary register to be used
 * @param t Register's type to be allocated
 * @return const char* Register's value (Ex. "$t5")
 */
const char* GetReg(Type t);

/**
 * @brief Register manager, frees a temporary register that is no longer used
 * @param t Register's type to be freed
 */
void FreeReg(Type t);

/**
 * @brief Register manager, frees all temporary registers
 */
void FreeAllRegs();

/**
 * @brief Writes While loop to mips file
 * @param reg Boolean register condition for loop to continue
 * @param part Four parts: 
 *             1. while label
 *             2. condition check
 *             3. jump back to start
 *             4. end while label
 */
void MipsWhile(const Reg* reg, uint32_t part);

/**
 * @brief Writes if statement to mips file
 * @param reg boolean register condition
 * @param part Three parts:
 *             1. Check condition, skip to else if false
 *             2. Jump to end if true, and else label 
 *             3. end label
 */
void MipsIf(const Reg* reg, uint32_t part);

/**
 * @brief Writes ForEach statement to mips file, MipsWhile is also used to save code
 * @param part Three parts to be used:
 *             1. before the step
 *             2. before the statement
 *             3. at the end of ForEach
 */
void MipsForEach(uint32_t part);

/**
 * @brief Writes switch statement to mips file
 * @param node 
 * @param start 
 */
void MipsSwitch(const Node* node, bool start);

/**
 * @brief Wrties case label to mips file
 * @param val 
 * @param start 
 */
void MipsCase(Val* val, bool start);
