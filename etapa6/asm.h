#ifndef ASM_HEADER
#define ASM_HEADER

#include "tac.h"

void generateASM(TAC *first);
void printASM(FILE* fout, TAC* first);
void makeBeginFun(FILE* fout, TAC* tac);
void makeEndFun(FILE* fout, TAC* tac);
void makePrint(FILE* fout, TAC* tac);
void makeCopy(FILE* fout, TAC* tac);
void makeArg(FILE* fout, TAC* tac);
void makeAdd(FILE* fout, TAC* tac);
void makeSub(FILE* fout, TAC* tac);
void makeMul(FILE* fout, TAC* tac);
void makeDiv(FILE* fout, TAC* tac);
void makeEq(FILE* fout, TAC* tac);
void makeDif(FILE* fout, TAC* tac);
void makeGrt(FILE* fout, TAC* tac);
void makeGeq(FILE* fout, TAC* tac);
void makeLes(FILE* fout, TAC* tac);
void makeLeq(FILE* fout, TAC* tac);
void makeAnd(FILE* fout, TAC* tac);
void makeOr(FILE* fout, TAC* tac);
void makeJz(FILE* fout, TAC* tac);
void makeJmp(FILE* fout, TAC* tac);
void makeLab(FILE* fout, TAC* tac);
void makeReturn(FILE* fout, TAC* tac);
void makeArrayAcc(FILE* fout, TAC* tac);
void makeFunctionCall(FILE* fout, TAC* tac);

#endif
