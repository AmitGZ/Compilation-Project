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

void MipsMathOp(MathOp mulOp, Reg* res, Reg* reg0, Reg* reg1);

void MipsRelOp(RelOp relOp, Reg* res, Reg* reg0, Reg* reg1);

void MipsLogOp(LogOp logOp, Reg* res, Reg* reg0, Reg* reg1);

void MipsDecl(Type t, const char* id, const char* val);

void MipsExit();

/**
 * @brief 
 * 
 * @param node 
 * @param name
 */
void MipsAssign(const Node* node, Reg* reg);

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

void MipsLoadV(Reg* reg);

void MipsLoadI(Reg* reg);

void MipsCast(Reg* reg, Type t);

void MipsMain();

const char* GetReg(Type t);

void FreeReg(Type t);

void FreeAllRegs();

void MipsWhile(Reg* reg, uint32_t part);

void MipsIf(Reg* reg, uint32_t part);

void MipsForEach(uint32_t part);
