/* 
 * File:            main.c
 * Project:         Computer Simulation - Phase 4
 * Submission Date: December 3, 2016
 * Author:          Jorge V. Vargas Jr. (0400283)
 * Lab UserId:	    jorge.vargas01
 *
 * This program simulates some functions that a computer can execute in this 
 * particular phase, the following are some characteristics of this program.
 * 
 * 1) It can excecute multiple commands such as Directory to show in the 
 * console the items in the current directory. To view a complete listing of 
 * the possible commands and their respecitve functions type 'Help' upon 
 * running this program.
 * 2) The functions that this program is capable of excecuting are not case 
 * sensitive, therefore, writing 'HeLp' and 'help' would return the same result.
 * Furthermore, this program is able to excecute abreviated commands of at least
 * 2 characters in length, which means that 'He' will complete the same task as 
 * 'Help'.
 * 3) This program is intended to perform assembly of source code, meant to be 
 * ran in an SIC architecture engine, currently the program can complete the 
 * functions of a 2-Pass assembler. By assembling an asm file the program will 
 * excecute and create a Listing File and an Object File of the input program.
 * (Note: Any errors and warnings encountered in the assembly will be displayed 
 * in the listing file, below the original line, the object file will not be 
 * created if errors are encountered.)
 * 4) The intermediate file contains the following:
 *    Copy of the original line
 *    Current address
 *    Label (Note: If no Label is found '%NOLABEL' will be written instead)
 *    OpCode respective to the current line's instruction
 *    Operand (Note: If no Operand is found '%NOOPERAND' will be written instead)
 *    Errors: 10-bit flag format ([9][8][7][6][5][4][3][2][1][0]) w/o brackets
 *      The error's are defined as follows:
 *      9: Symbol in operand not found.
 *      8: END directive not found.
 *      7: Duplicate Label.
 *      6: Illegal Label.
 *      5: Illegal Instruction.
 *      4: Missing or Illegal operand on data storage directive.
 *      3: Missing or Illegal operand on START directive.
 *      2: Missing or Illegal operand on END dirctive.
 *      1: Too many symbols in source program.
 *      0: Program too long.
 * 5) The assembler implements the use of hash tales for fast writing and
 * retrieval of both Labels and Instruction OpCodes. The hashtable implemented
 * is an opensource library by the University of Cambridge. All the details 
 * for this library can be found in the files strmap.h and strmap.c.
 */

#include <stdlib.h>
#include <stdio.h>

#include "InputHandler.h"
#include "sic.h"



int main() {
    
    printf("Welcome to Jorge's Interpreter!\n");
    printf("This is Phase 4: \n\n");
    SICInit();
    doHelp();
    printf("\n");
    
    while (1) {
        
        printf(">> ");
        struct Instruction curCommand;
        
        nextInstruction(&curCommand);
        printf("\n");

        if (curCommand.id == 7) break; //Handles exit function
        
    }
    
    return (EXIT_SUCCESS);
    
}