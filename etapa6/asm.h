#ifndef ASM_HEADER
#define ASM_HEADER

#include "tac.h"

void generateASM(TAC *first);
void makeBeginFun(FILE* fout, TAC* tac);
void makeEndFun(FILE* fout, TAC* tac);
void makePrint(FILE* fout, TAC* tac);

#endif
