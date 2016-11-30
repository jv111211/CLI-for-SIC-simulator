/* 
 * File:            sicLoader.c
 * Project:         Computer Simulation - Phase 4
 * Submission Date: December 3, 2016
 * Author:          Jorge V. Vargas Jr. (0400283)
 * Lab UserId:	    jorge.vargas01
 */

#include <stdio.h>
#include <stdlib.h>

#include "errorHandler.h"
#include "sic.h"
#include "InputHandler.h"
#include "sicLoader.h"

/******************************************************************************
  Handles the loading of an object file @param filename into the SICEngine.
 ******************************************************************************/
int LoadProgram(char* filename){
    
    FILE* fp = fopen(filename,"r");
    int endFound=0;
    if(fp==NULL) {errorHndlg("Load",5);return;}
    
    char line[75];
    fgets(line,sizeof(line),fp);
    
    if(line[0]=='H'){
        char progAddr[10], progLen[10];
        
        sscanf(line, "%*1s%6s%6s%6s%*s", progName, progAddr, progLen);
        startAddr = strtol(progAddr, NULL, 16);
        fillSpaces(progName,6);
        
        //Load Text Records
        while(fgets(line,sizeof(line),fp)!=NULL){
            if(line[0]=='E') {endFound=1;break;}
            TextRecord* currentRecord=malloc(sizeof(TextRecord));
            setRecord(currentRecord,line);
            loadRecord(currentRecord);
        }
        
        if(endFound==0) {errorHndlg("Load",11);return 0;}
        else{
            sscanf(line, "%*1s%6s",progAddr);
            startAddr = strtol(progAddr,NULL,16);
            fillSpaces(progName,6);
            return 1;
        }
    }
    else errorHndlg("Load",9);
    
    return 0;
    
}
/******************************************************************************
 Used to break the record currently being read into the Struct's corresponding
 locations.
 ******************************************************************************/
void setRecord(TextRecord* currRecord,char* line){
    char firstAddress[7],recLength[7],byteInstructions[61],curByte[3];
    int i,j=0,tmp=0;
    sscanf(line,"%*1s%6s%2s%60s",firstAddress,recLength,byteInstructions);
    currRecord->startAddress=strtol(firstAddress,NULL,16);
    currRecord->recordLength=strtol(recLength,NULL,16);
    for(i=0;i<(currRecord->recordLength*2);i++){
        curByte[0]=byteInstructions[i];
        curByte[1]=byteInstructions[++i];
        curByte[2]='\0';
        tmp = strtol(curByte,NULL,16);
        currRecord->byteInstructions[i]=tmp;
        //printf("current loading byte: |%s|%u|\n",curByte,currRecord->byteInstructions[i]);
    }
}
/******************************************************************************
 Loads each given record @param currRecord into each byte's corresponding 
 address.
 ******************************************************************************/
void loadRecord(TextRecord* currRecord){
    int i;
    BYTE curByte;
    ADDRESS curLocation=currRecord->startAddress;
    for(i=0;i<(currRecord->startAddress*2);i++){
        curByte=currRecord->byteInstructions[++i];
        PutMem(curLocation,&curByte,0);
        curLocation++;
    }
}