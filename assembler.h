/* 
 * File:            assembler.h
 * Project:         Computer Simulation - Phase 4
 * Submission Date: December 3, 2016
 * Author:          Jorge V. Vargas Jr. (0400283)
 * Lab UserId:	    jorge.vargas01
 *
 */

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "strmap.h"

struct assyLine{
    char Label[10];
    char Operation[10];
    char Operand[15];
    char Comment[100];
    long PSW;
    char psw[15];
    char Opcode[10];
    char LOCCTR[10];
    char ObCode[10];
    char line[200];
};

typedef struct assyLine assyLine;

//Pass 1 Functions
void Pass_Juan(char* filename);
void initOpTable(StrMap *hashtable);
void setOutLine(char* outLn, assyLine* commin, char *errors);
void setLine(char* line,assyLine* commin);
int opSize(char *Operand);
int isAlpha(char *operand);
void toLow(char *instruction);
long toBinary(int n);
int toHex(char* buffer, char* op);

//Pass 2 Functions
void Pass_Chuy(StrMap *symTab, char* intermediateFile, int errors, int start);
void setArtifacts(FILE* fp, assyLine* commin, int head);
void placeHeadEndRecord(FILE* fp, assyLine* commin,int op,char* s0,char* s1);
void TextRecToObFile(char* record, FILE* fp, char* address,int err);
void addTextObcode(char* record,char* obcode);
void setTextLength(char* record);
void initTextRecord(char* record, char* address);
void setObcode(assyLine* commin, StrMap* symTab);
void putListingLine(FILE* fp, assyLine* commin);
void fillZeros(char* cur);
void fillSpaces(char* col, int sizeReq);

//Functions used by both passes
void replaceExtension(char* f0, char *f1, char* f2, int pass);
void clearStruct(assyLine* commin);

#endif	// ASSEMBLER_H