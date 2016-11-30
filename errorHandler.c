/* 
 * File:            errorHandler.h
 * Project:         Computer Simulation - Phase 4
 * Submission Date: December 3, 2016
 * Author:          Jorge V. Vargas Jr. (0400283)
 * Lab UserId:	    jorge.vargas01
 *
 */

/*
 * The following function holds all possible errors.
 * 
 * This function will be called for any error in the program with the function 
 * type for which the error occurred and the error type (error id).
 */
#include <stdio.h>

void errorHndlg(char* comm, int errortype) {
    
    printf("\n");
    if (comm != "") printf("There was an error when attempting to run "
            "function: %s.\n", comm);
    
    switch (errortype){
        
        case -99:
            printf("Error 0x-99: Blank input. Type 'help for a list of "
                    "possible commands. \n");
            break;
        case 99:
            printf("Error 0x99: Command not found. Type 'help for a list of "
                    "possible commands.\n");
            break;
        case -1:
            printf("Error 0x-1: Insufficient number of arguments. Type 'help' "
                    "for a list of possible commands.\n");
            break;
        case 1:
            printf("Error 0x1: Excessive number of arguments. Type 'help' for a "
                    "list of possible commands.\n");
            break;
        case 2:
            printf("Error 0x2: At least one of the passed parameters are not in "
                    "hexadecimal format.\n");
            break;
        case 3:
            printf("Error 0x3: Maximum number of characters reached. Type 'help' "
                    "for a list of possible commands.\n");
            break;
        case 4:
            printf("Error 0x4: Incorrect file extension!\n"
                    "Assemble instruction handles .asm files.\n"
                    "Load instruction handles .ob files.\n");
            break;
        case 5:
            printf("Error 0x5: File not found!\n");
            break;
        case 6:
            printf("Error 0x7 Start address is larger than end address!\n");
            break;
        case 7:
            printf("Error 0x6: End address does not fall within range: 0x0 - 0x7FFF!\n");
            break;
        case 8:
            printf("Error 0x8: Start address cannot be less than 0!\n");
            break;
        case 9:
            printf("Error 0x9: Head record missing in program!\n");
            break;
        case 10:
            printf("Error 0xA: No program has been loaded or has loaded incorrectly!\n");
            break;
        case 11:
            printf("Error 0xB: End record not found in program!\n");
            break;
    }
    
}
void warningHndlg(int warnType){
    
    switch (warnType){
        
        case 0:
            printf("Warning: No PROGRAM NAME/STARTING ADDRESS found! \n");
            break;
            
    }
}
/******************************************************************************
  Prints the current found errors and its corresponding address.
 ******************************************************************************/
void setErrors(FILE* fp, char *psw){
    if(psw[8]=='1') fputs("***Symbol in operand undefined!\n",fp);
    if(psw[8]=='1') fputs("***Program too long!\n",fp);
    if(psw[7]=='1') fputs("***Too many symbol in source program!\n",fp);
    if(psw[6]=='1') fputs("***Missing or illegal operand on END directive!\n",fp);
    if(psw[5]=='1') fputs("***Missing or illegal operand on START directive!\n",fp);
    if(psw[4]=='1') fputs("***Missing or illegal operand on data "
            "storage directive!\n",fp);
    if(psw[3]=='1') fputs("***Illegal Instruction!\n",fp);
    if(psw[2]=='1') fputs("***Illegal Label!\n",fp);
    if(psw[1]=='1') fputs("***Duplicate Label!\n",fp);
    if(psw[0]=='1') fputs("***END directive not found!\n",fp);
}