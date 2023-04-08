#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "my_structs.h"
#include "Hash.h"

/**
 * @brief Ask the user for input
 * @param node write input value to node
 */
void MipsIn(const Node* node);

Reg MipsMathOp(MathOp mulOp, Reg reg0, Reg reg1);

Reg MipsRelOp(RelOp relOp, Reg reg0, Reg reg1);

Reg MipsLogOp(LogOp logOp, Reg reg0, Reg reg1);

void MipsDecl(Type t, const char* id, const char* sval);

void MipsExit();

/**
 * @brief 
 * 
 * @param node 
 * @param name
 */
void MipsAssign(const Node* node, Reg reg);

/**
 * @brief 
 * 
 * @param node 
 */
void MipsOut(const Reg* reg);

/**
 * @brief 
 */
void MipsData();

Reg MipsLoadVar(const Node* node);

Reg MipsLoadImmediate(const Val* val);

Reg MipsCast(const Reg* reg, Type t);

void MipsMain();

const char* GetReg(Type t);

void FreeReg(Type t);

void FreeAllRegs();

void MipsWhile(const Reg* reg, uint32_t part);

void MipsIf(const Reg* reg, uint32_t part);

void MipsForEach(uint32_t part);

void MipsSwitch(const Node* node, bool start);

void MipsCase(Val* val, bool start);
