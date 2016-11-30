/* 
 * File:            InputHandler.c
 * Project:         Computer Simulation - Phase 4
 * Submission Date: December 3, 2016
 * Author:          Jorge V. Vargas Jr. (0400283)
 * Lab UserId:	    jorge.vargas01
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <ctype.h>


#include "sic.h"
#include "InputHandler.h"
#include "assembler.h"
#include "errorHandler.h"
#include "sicLoader.h"

/*
 * @param filename This is the name of the object file that will be loaded
 * 
 * The following first 7 function are to run each of the possible commands with 
 * the exception of 'exit' due to the fact that the exit functionality is 
 * handled in the main function by terminating the while loop.
 */
void doLoad(char* filename) {

    int success=0;
    progName[0]='\0';
    if(strstr(filename, ".ob")) success=LoadProgram(&filename[0]);
    else errorHndlg("Load", 4);
    if(success==1) printf("\nFile: %s was loaded successfully!\nProgram Name: %s\n",filename,progName);
    else printf("\nFile: %s failed to load!\n",filename);

}

void doExecute() {
    if(strlen(progName)!=0){
        printf("\nProgram name n size: %s!\nStarting address: %lu!\nExecuting...\n",progName,startAddr);
        SICRun(&startAddr, FALSE);
        printf("\nProgram concluded execution!\n");
    }
    else errorHndlg("Execute",10);  
}

void doDebug() {

    // Will perform the Debug function
    printf("\nDebug function not yet implemented.\n");

}
/*
 * @param start This is the initial address that must be dump
 * @param end This is the end address that must be dump
 */
void doDump(char* start, char* end) {
    
    if(validateHex(&start[0])!=0&&validateHex(&end[0])!=0) {
        int from = strtol(start, NULL, 16), to = strtol(end, NULL, 16), 
                i, tmp=0,count=-1;
        if(from>to) {errorHndlg("Dump",6);return;}
        if(to>32767) {errorHndlg("Dump",7);return;}
        if(from<0) {errorHndlg("Dump",8);return;}
        
        printf("\n         +0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F");
        
        for(i=(from-(from%16));i<=(to+(15-(to%16)));i++){
            if(i%16==0) {
                if(count==15) {
                    while(getchar() != '\n');
                    count=-1;
                    printf("\n         +0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F");
                }
                printf("\n%06Xh: ",i);count++;
            }
            if(i<from || i>to) printf("-- ");
            else{
                GetMem(i, &tmp, 0);
                printf("%02X ", tmp);
            }
            
        }
        printf("\n");
    }
    else errorHndlg("Dump",2);
}

void doHelp() {
    
    printf("\nLoad <filename>         --Loads the file called <filename>.\n");
    printf("Execute                 --Uses computer simulation to execute a "
            "program previously loaded.\n");
    printf("Debug                   --Allows you to execute in debug mode.\n");
    printf("Dump <start> <end>      --Calls dump between <start> and <end>. "
            "(Note: <start> and <end> should be hexadecimal values.)\n");
    printf("Help                    --Prints this help prompt.\n");
    printf("Assemble <filename>     --Assembles a file called <filename>.\n");
    printf("Directory               --Lists the contents of the "
            "current directory.\n");
    printf("Exit                    --Terminates program.\n");
    
}

/*
 * @param filename This is the name of the file that will be assembled
 * 
 * This function will take a file and assemble it to output an object file
 */
void doAssemble(char* filename) {
    
    // Will perform the Assemble functionl
    if(strstr(filename, ".asm")) Pass_Juan(filename);
    else errorHndlg("Assemble", 4);
    
}

void doDirectory() {
    
    // Will perform the Directory function
    printf("\nThe following are the current directory contents: \n");
    system("ls");
    
}

/*
 * @param comid This is the command id for ease in m
 * @param args This is the number of arguments received by the user
 * @param firstArg This is the first parameter received by the user
 * @param secondArg This is the second parameter received by the user
 * 
 * The following function runs the specific command by the user and execute 
 * errors when required.
 * case -1 for comid is the result for a blank input in case the user simply 
 * types enter without typing any words prior
 * The default means that the user has entered an invalid command which will 
 * trigger the corresponding error.
 */
void runCommand(int comid, int args, char* firstArg, char* secondArg) {
    
    switch (comid) {

        case 0:
            if (args != 1) errorHndlg("load", argCountChk(1, args));
            else doLoad(firstArg);
            break;
        case 1:
            if (args != 0) errorHndlg("execute", 1);
            else doExecute();
            break;
        case 2:
            if (args != 0) errorHndlg("debug", 1);
            else doDebug();
            break;
        case 3:
            if (args != 2) errorHndlg("dump", argCountChk(2, args));
            else doDump(firstArg, secondArg);
            break;
        case 4:
            if (args != 0) errorHndlg("help", 1);
            else doHelp();
            break;
        case 5:
            if (args != 1) errorHndlg("assemble", argCountChk(1, args));
            else doAssemble(firstArg);
            break;
        case 6:
            if (args != 0) errorHndlg("directory", 1);
            else doDirectory();
            break;
        case 7:
            //'Exit' handled in main function
            break;
        default:
            errorHndlg("", 99);
            break;

    }
    
}

/*
 * @param comm This is the struct where the line arguments will be stored
 * @param line This is the input line with any undesired characters.
 * 
 * This function runs the commands necessary to segment the raw input into the 
 * corresponding objects of the created struct. This in turn permits the 
 * program to understand the user's intentions.
 * 
 * This function will determine the amount of total words input by the user.
 * 
 * This function will also protect the struct from stack smashing by validating 
 * the size of chars in each of the words by the user to not exceed the size of 
 * the objects in the struct.
 */
void setCommand(Instruction* comm, char* line) {
        
    int args = 0;
    int start = 0;
    char command[256];
    char first[256];
    char sec[256];
    char excess[256];
    
    start = myScanner(start, &line[0], &args, &command[0]);
    start = myScanner(start, &line[0], &args, &first[0]);
    start = myScanner(start, &line[0], &args, &sec[0]);
    start = myScanner(start, &line[0], &args, &excess[0]);
    
    if(strlen(command) < 10) strcpy(comm->command,command); 
    else strncpy(comm->command,"none",5);
    if(strlen(first) < 261) strcpy(comm->firstArg,first); 
    else strncpy(comm->firstArg,"",2);
    if(strlen(sec) < 30) strcpy(comm->secondArg,sec); 
    else strncpy(comm->secondArg,"",2);
    
    comm->wordCnt = args - 1;
    comm->id = getId(comm->command);
    
}

/*
 * @param comm This is the struct that has been created in main to store the 
 * segmented input
 * 
 * The following function receives the user input and cleans any characters 
 * beyond the size of the input 'line' possibility
 * 
 * In the event that a new line is not found in the the input, the stdin is 
 * cleared and an error is displayed for an excessive amount of characters.
 * 
 * If a new line is found the function runs the setCommand function which sets 
 * all the received parameters in it's corresponding locations within the struct
 * 
 * The runCommand function will receive the user input in a segmented format as 
 * the command id, the wordCnt: number of parameter (w/o command), 
 * firstArg: first parameter, secondArg: second parameter
 */
void nextInstruction(Instruction* comm) {
    
    char line[350];
    int i;
    
    fgets(line, 350, stdin); 
    
    if(!strchr(line, '\n')){
        
        while(fgetc(stdin) != '\n');
        
        printf("\n");
        errorHndlg("",3);
        return;
        
    }
    
    if(line[0]=='\n' || line[0]=='\0') {errorHndlg("",-99); return;}
    
    setCommand(comm, line);
    
    runCommand(comm->id, comm->wordCnt, comm->firstArg, comm->secondArg);
}

/*
 * @param start This is the start location where line should be read
 * @param line This is the input from the user
 * @param args This is the current number of arguments
 * @param part This is the current argument collected
 * 
 * The following function scans through the input 'line' starting from the 
 * position indicated by 'start' and finds all characters before the next space.
 * 
 * This function returns the next space in the input.
 */
int myScanner(int start, char* line, int* args, char* part){
    
    start = nextChar(line,start);
    int count = 0;
    
    if(start != -1){
        *args += 1;
        while(1){
            part[count] = line[start];
            if(isspace(line[start++]) == 0){
                count++;
            }
            else {
                part[count]='\0';
                return start;
            }
        }
    }
    
}

/*
 * @param comm This is the command from the user
 * 
 * The following function returns the command id for the received command.
 * 
 * It will test for the given size of the command, (if the command was 'du' this 
 * function will interpret it as dump.
 * 
 * All function will required at least the first 2 characters to be interpreted 
 * correctly except the 'Execute' and 'Exit' function which required at least 
 * the first 3 characters.
 * 
 * If the command input does not exist or if there is an insufficient amount of 
 * characters to determine the command this function will return 8.
 * 
 * In the event of a blank input this function will return -1.
 */
int getId(char* comm) {
    
    int  size = strlen(comm);
    
    if ((size < 2 && size > 0) || (size == 2 && (comm[0] == 'e' || 
            comm[0] == 'E'))) return -1;
    
    if (strncasecmp(comm, "load", size) == 0) return 0;
    if (strncasecmp(comm, "execute", size) == 0) return 1;
    if (strncasecmp(comm, "debug", size) == 0) return 2;
    if (strncasecmp(comm, "dump", size) == 0) return 3;
    if (strncasecmp(comm, "help", size) == 0) return 4;
    if (strncasecmp(comm, "assemble", size) == 0) return 5;
    if (strncasecmp(comm, "directory", size) == 0) return 6;
    if (strncasecmp(comm, "exit", size) == 0) return 7;
    
    return -1;
    
}

/*
 * @param expected This is the amount of arguments expected according to command
 * @param args This is the actual amount of arguments received by user
 * 
 * The following function is used to determine if the number of parameters 
 * received by the user are the correct amount of parameters for the 
 * indicated command.
 */
int argCountChk(int expected, int args) {
    if (args < expected) return -1;
    if (args > expected) return 1;
    return 0;
}

/*
 * @param line This is the user input
 * @param i This is the location of where to start reading line
 * 
 * The following function determines the next non space character in the input 
 * character array 'line' from the position in the array indicated by input 'i'.
 * 
 * This function will return the position of the next non space character 
 * or -1 if non is found.
 */
int nextChar(char* line, int i){
    for(;i<strlen(line);i++){
        if(isspace(line[i])==0) return i;
    }
    return -1;
}

/*
 * @param hex This is the value to be tested
 * 
 * This function received an array of characters and validates it as a value in
 * base 16. The value can start with 0x.
 */
int validateHex(char *hex){
    int i=0,n=0;
    if(hex[0]=='0'&&(hex[1]=='x'||hex[1]=='X')) i=2;
    for(i;i<strlen(hex);i++){
        if((int) hex[i] == 39) return n;
        if(isxdigit(hex[i])) n++;
        else return 0;
    }
    return n;
}