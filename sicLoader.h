/* 
 * File:            sicLoader.c
 * Project:         Computer Simulation - Phase 4
 * Submission Date: December 3, 2016
 * Author:          Jorge V. Vargas Jr. (0400283)
 * Lab UserId:	    jorge.vargas01
 *
 */

#ifndef SICLOADER_H
#define SICLOADER_H

struct TextRecord{
    ADDRESS startAddress;
    BYTE recordLength;
    BYTE byteInstructions[30];
};
typedef struct TextRecord TextRecord;
int LoadProgram(char* filename);
void setRecord(TextRecord* currRecord,char* line);
void loadRecord(TextRecord* currRecord);

#endif /* SICLOADER_H */