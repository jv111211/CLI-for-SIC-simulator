/* 
 * File:            InputHandler.h
 * Project:         Computer Simulation - Phase 4
 * Submission Date: December 3, 2016
 * Author:          Jorge V. Vargas Jr. (0400283)
 * Lab UserId:	    jorge.vargas01
 *
 */

#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

/*
 * For the following struct:
 * 
 * The largest possible command contains 9 characters therefore 10 character 
 * should suffice to include the '\0' character
 * 
 * Maximum size possible for a text file name is 260, 261 characters should 
 * suffice to include the '\0' character
 *  
 * 29 + 1 for '\0' should suffice for most hex values input
 *
 */

#include "sic.h"

struct Instruction{
    char command[10];
    char firstArg[261];
    char secondArg[30];
    int wordCnt;
    int id;
};

//Global variables for loading and executing
char progName[10];
ADDRESS startAddr;

typedef struct Instruction Instruction;
void doHelp();
void nextInstruction(Instruction* c);
int nextChar(char* line, int from);
int myScanner(int start, char* line, int* args, char* part);
int argCountChk(int expected, int args);
int getId(char* comm);
int validateHex(char *hex);
#endif	// 