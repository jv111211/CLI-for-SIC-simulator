/* 
 * File:            assembler.c
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

#include "assembler.h"
#include "errorHandler.h"
#include "strmap.h"
#include "InputHandler.h"

/******************************************************************************
  Implements Pass 1 on @param filename and outputs an intermediate file and a
  symbol table file. It as well prints the errors found in the console along 
  with the error's correponding address for programmer's review.
 ******************************************************************************/
void Pass_Juan(char* filename){
    
    StrMap *symTab = sm_new(510);
    StrMap *opTab = sm_new(30);
    
    char line[500],outLine[250],address[10],currErrors[10],
            temp[100],symbolTabFile[50];
    int PSW = 0, LOCCTR=0, eofFnd=0, labelCnt=0,progLength,start=0;
    struct assyLine commin;
        
    FILE *fp = fopen(filename, "r");
    if(fp == NULL) {errorHndlg("Assemble", 5); return;}
    
    replaceExtension(filename,&symbolTabFile[0], " ", 1);
    FILE *fIntermediate = fopen(filename, "w+");
    //FILE *fSymbolTable = fopen(symbolTabFile,"w");  
    
    initOpTable(opTab);  
    
    fgets(line, 100, fp);
    setLine(&line[0],&commin);
    
    if(strcasecmp(commin.Operation,"start") == 0){
        
        labelCnt++;
        start=1;
        if(validateHex(&commin.Operand[0])!=0){
            //Save program name as value for START key in symTab
            sm_put(symTab,"%START",commin.Label);
            sm_put(symTab,commin.Label,commin.Operand);
            //Initialize LOOCTR and store Starting program address
            LOCCTR = (int) strtol(commin.Operand, NULL, 16);
            
            //Set output line
            setOutLine(&outLine[0],&commin,&currErrors[0]);
            //Write line to Intermediate file
            fputs(line, fIntermediate);
            strcat(commin.Operand,"\n");
            fputs(commin.Operand,fIntermediate);
            commin.Operand[strlen(commin.Operand)-1]='\0';
            strncat(currErrors,"\n",4);
            //Write line to Intermediate file
            fputs(currErrors, fIntermediate);
            
            fgets(line, 500, fp);
            setLine(&line[0],&commin);
            toLow(&commin.Operation[0]);
        }
        else PSW += 8;      //Missing or Illegal operand of START directive
        
    }
    else {LOCCTR=0; warningHndlg(0);} 
    
    while(strcasecmp(commin.Operation, "end")!=0){ 
                
        PSW = 0;
        //End of file reached before END directive was found
        if (feof(fp)) {eofFnd = 1; break;} 
        
        //change location coutner base 10 to location counter string base 16
        snprintf(commin.LOCCTR, 9, "%04X", LOCCTR); 
        
        if(commin.Comment[0]!='.'){
            
            if(labelCnt>=500) PSW+=2;

            if(strcasecmp(commin.Label, "%NOLABEL")!=0&&commin.Label[0]!='\0'){
                
                if(strlen(commin.Label)>6||sm_get(opTab,commin.Label,temp,
                        sizeof(temp))!=0||!isalpha(commin.Label[0])) PSW+=64;    
                
                if(sm_get(symTab, &commin.Label[0],temp, sizeof(temp)) != 0) 
                    PSW += 128;
                else{
                    labelCnt++;
                    if(labelCnt==500) PSW+=2;

                    snprintf(address, 9, "%04X", LOCCTR);
                    sm_put(symTab, &commin.Label[0], &address[0]);
                }
            }

            if(sm_get(opTab, commin.Operation,commin.Opcode,
                    sizeof(commin.Opcode)) != 0) LOCCTR += 3;
            else if(strcasecmp(commin.Operation,"word")==0) {
                LOCCTR += 3;
                strncpy(commin.Opcode,"%WORD",sizeof(commin.Opcode));
            }
            else if(strcasecmp(commin.Operation,"resw")==0) {
                if(validateHex(&commin.Operand[0])!=0) 
                    LOCCTR += (3 * (int) strtol(commin.Operand, NULL, 10));
                //Missing or Illegal Operand on data storage directive
                else PSW += 16;
                strncpy(commin.Opcode,"%RESW",sizeof(commin.Opcode));
            }
            else if(strcasecmp(commin.Operation,"resb")==0) {
                if(validateHex(&commin.Operand[0])!=0) 
                    LOCCTR += (int) strtol(commin.Operand, NULL, 10);
                //Missing or Illegal Operand on data storage directive
                else PSW += 16;
                strncpy(commin.Opcode,"%RESB",sizeof(commin.Opcode));
            }
            else if(strcasecmp(commin.Operation, "byte")==0) {
                if(opSize(&commin.Operand[0])!=-1){
                    strncpy(temp,commin.Operand,sizeof(temp));
                    LOCCTR += opSize(&commin.Operand[0]);
                    toHex(commin.Operand,temp);
                }
                //Missing or Illegal Operand on data storage directive
                else PSW += 16;
                strncpy(commin.Opcode,"%BYTE",sizeof(commin.Opcode));
            }
            else {commin.Opcode[0]='\0'; PSW += 32;} //Illegal Operation


            if(commin.Opcode==NULL) strncpy(commin.Opcode,"%NOOPCODE",10);
            commin.PSW = toBinary(PSW); //Set PSW with corresponding flags 

            //write line to intermediate file
            setOutLine(&outLine[0],&commin,&currErrors[0]);
            
            fputs(line, fIntermediate);
            fputs(outLine, fIntermediate);

        }
        else if(commin.Comment[0]=='.'){
            //current line is a comment
            fputs(line, fIntermediate);    
        }
        //If there is a blank line in the source code it will be skipped
        
        //read next input line
        fgets(line, 500, fp);
        setLine(&line[0],&commin);
        
    } //{while not END}
    
    //Write last line to intermediate file
    if(eofFnd == 0){
        if(sm_get(symTab, &commin.Operand[0],temp, sizeof(temp)) != 0){
            //save (LOCCTR - starting address) as program length
            progLength = LOCCTR - (int)strtol(temp,NULL,16); 
            //Program too long, exceeded 7FFF limit
            if(progLength>32768) PSW+=1;
            snprintf(address, 9, "%04X", progLength);
            //Save program length in symTab Hash
            sm_put(symTab,"%TotLen",address);
            
            fputs(line, fIntermediate);
            fputs("%END\n",fIntermediate);
            fputs("%END\n",fIntermediate);
            fputs(commin.Operand,fIntermediate);
            fputs("\n",fIntermediate);
        }
        else PSW+=4;      //Missing or Illegal operand of END directive
    }
    else PSW+=256;      //END directive not found!
    
    commin.PSW = toBinary(PSW);     //Set PSW with corresponding flags 
    //write line to intermediate file
    setOutLine(&outLine[0],&commin,&currErrors[0]);
    
    fclose(fIntermediate);
    
    Pass_Chuy(symTab,filename,PSW,start);
 
} //End of Pass_Juan (Pass 1)

void Pass_Chuy(StrMap *symTab, char* intermediateFile, int errors, int start){
    
    char obFileName[60],listFileName[65],curRecord[75],tmp1[10],tmp2[10];
    struct assyLine commin;
    
    replaceExtension(intermediateFile,&obFileName[0],&listFileName[0],2);
    
    FILE* fIntermediate = fopen(intermediateFile, "r");
    FILE* fObject = fopen(obFileName, "w");
    FILE* fListing = fopen(listFileName, "w");
    tmp2[0]='\0';
    
    if(start==1){
        char name[20];
        setArtifacts(fIntermediate,&commin,1);
        putListingLine(fListing,&commin);
        sm_get(symTab,"%START",tmp2,sizeof(tmp2));
    }
    
    //Write header record
    if(errors==0){
        sm_get(symTab,"%TotLen",tmp1,sizeof(tmp1));
        placeHeadEndRecord(fObject,&commin,0,tmp2,tmp1);
    }
    
    //Get next line
    setArtifacts(fIntermediate,&commin,0);
    
    initTextRecord(&curRecord[0],commin.LOCCTR);
    
    while(strcasecmp(commin.LOCCTR,"%end")!=0){
        if(feof(fIntermediate)) {break;commin.psw[0]='1';}
        //if line is not a comment
        if(commin.line[0]!='.') {  
            //assemble object code according to line
            setObcode(&commin,symTab);
            if(commin.psw[9]=='1') errors=1;
            
            if(strlen(curRecord)+strlen(commin.ObCode)>69)
                TextRecToObFile(&curRecord[0],fObject,&commin.LOCCTR[0],errors);
            else if(strcasecmp(commin.Opcode,"%resb")==0||strcasecmp(commin.Opcode,"%resw")==0){
                snprintf(commin.LOCCTR,sizeof(commin.LOCCTR),"%06X",(strtol(commin.LOCCTR,NULL,16)+strtol(commin.Operand,NULL,10)));
                TextRecToObFile(&curRecord[0],fObject,&commin.LOCCTR[0],errors);
            }
            addTextObcode(&curRecord[0],&commin.ObCode[0]);
        }
        //Write listing line and errors
        putListingLine(fListing,&commin);
        
        //Read next input line
        setArtifacts(fIntermediate,&commin,0);
        if(strcasecmp(commin.LOCCTR,"%end")==0)
            TextRecToObFile(&curRecord[0],fObject,&commin.LOCCTR[0],errors);
    }
    
    commin.LOCCTR[0]='\0';
    sm_get(symTab,commin.Operand,tmp2,sizeof(tmp2));
    placeHeadEndRecord(fObject,&commin,1,tmp2,"");
    putListingLine(fListing,&commin);
    
    fclose(fObject);
    fclose(fListing);
    remove(intermediateFile);
    
    //if there were errors in the input source code
    if(errors==1) {
        remove(obFileName);
        printf("\nObject file failed to be created, listing file (%s) was created!\n",listFileName);
        return;
    }
    
    printf("\nObject file (%s) and Listing file (%s) created successfully!\n", obFileName,listFileName); 
}//End of Pass_Chuy (Pass 2)

/******************************************************************************
 ******************************************************************************
  Miscellaneous functions used by Pass 1.                                       ***************************************************     @Pass1
 ******************************************************************************
 ******************************************************************************/

/******************************************************************************
  Used for initializing the Operation table (@param hashtable) with each 
  operation's corresponding opcode.
 ******************************************************************************/
void initOpTable(StrMap *hashtable){
    sm_put(hashtable, "add", "18" );    sm_put(hashtable, "and", "58" );
    sm_put(hashtable, "comp", "28" );   sm_put(hashtable, "div", "24" );
    sm_put(hashtable, "j", "3C" );      sm_put(hashtable, "jeq", "30" );
    sm_put(hashtable, "jgt", "34" );    sm_put(hashtable, "jlt", "38" );
    sm_put(hashtable, "jsub", "48" );   sm_put(hashtable, "lda", "00" );
    sm_put(hashtable, "ldch", "50" );   sm_put(hashtable, "ldl", "08" );
    sm_put(hashtable, "ldx", "04" );    sm_put(hashtable, "mul", "20" );
    sm_put(hashtable, "or", "44" );     sm_put(hashtable, "rd", "D8" );
    sm_put(hashtable, "rsub", "4C" );   sm_put(hashtable, "sta", "0C" );
    sm_put(hashtable, "stch", "54" );   sm_put(hashtable, "stl", "14" );
    sm_put(hashtable, "stx", "10" );    sm_put(hashtable, "sub", "1C" );
    sm_put(hashtable, "td", "E0" );     sm_put(hashtable, "tix", "2C" );
    sm_put(hashtable, "wd", "DC" );
}
/******************************************************************************
  Concatenates the contents in @param commin and @param errors into @param 
  outLn.
 ******************************************************************************/
void setOutLine(char* outLn, assyLine* commin, char *errors){
    snprintf(outLn,250,"%s\n%s\n%s\n%09ld\n",commin->LOCCTR,
            commin->Opcode,commin->Operand,commin->PSW);
    snprintf(errors,10,"%09ld",commin->PSW);
}
/******************************************************************************
  Determines the current line format and sets the struct accordingly.
 ******************************************************************************/
void setLine(char* line,assyLine* commin){
    clearStruct(commin);
    if(isspace(line[0])){    
        sscanf(line," %s",commin->Operation);
        if(strcasecmp(commin->Operation,"rsub")==0) {
            sscanf(line, "%s %[^\n]", commin->Operation,commin->Comment);
            strncpy(commin->Operand,"%NOOPERAND",15);
        }
        else 
            sscanf(line, " %s %s %[^\n]", commin->Operation,
                    commin->Operand,commin->Comment);
        strncpy(commin->Label,"%NOLABEL",10);
    }
    else if(line[0]=='.') 
        sscanf(line, "%[^\n]", commin->Comment);
    else if(!isspace(line[0])) 
        sscanf(line, "%s %s %s %[^\n]", commin->Label,commin->Operation,
                commin->Operand,commin->Comment);
    toLow(commin->Operation);
    if(strcasecmp(commin->Operation,"word")==0){
        strcpy(commin->Opcode,"%NOOPCODE");
        snprintf(commin->Operand,8,"%06X",(int)strtol(commin->Operand,NULL,10));
    }
}
/******************************************************************************
 Returns size of Operand in BYTE directive when it is in correct format, -1
 otherwise.
 ******************************************************************************/
int opSize(char *Operand){
    char constant[20];
    constant[0]='\0';
    int i, is=0, j=0, apos=0;
    //printf("Operand is: %s\n",Operand);
    if(Operand[0]=='C'||Operand[0]=='c') {
        for(i=0;i<strlen(Operand);i++){
            if((Operand[i]=='C'||Operand[0]=='c')&&is==0) is==1;
            else if((int)Operand[i] != 39) {constant[j]=Operand[i]; j++;}
            else if((int)Operand[i] == 39&&apos==0) apos=1;
            else if((int)Operand[i] == 39&&apos==1) {constant[j]='\0';break;}
        }
        return isAlpha(&constant[0]);
    }
    else if(Operand[0]=='X'||Operand[0]=='x'){
        for(i=0;i<strlen(Operand);i++){
            if((Operand[i]=='X'||Operand[0]=='x')&&is==0) is==1;
            else if((int)Operand[i] != 39) {constant[j]=Operand[i]; j++;}
            else if((int)Operand[i] == 39&&apos==0) apos=1;
            else if((int)Operand[i] == 39&&apos==1) {constant[j]='\0';break;}
        }
        if(validateHex(&constant[0])!=0&&validateHex(&constant[0])<=32&&
                validateHex(&constant[0])%2==0) return strlen(constant)/2;
    }
    else return -1;
}
/******************************************************************************
 Returns size of @param myChar when it's in alphabetic format, -1 otherwise.
 ******************************************************************************/
int isAlpha(char* myChar){
    int i, size=0;
    for(i=0;i<strlen(myChar);i++){
        if(isalpha(myChar[i])) size++;
        else if((int)myChar[i]== 39) return size;
        else return -1;
    }
    return size;
}
/******************************************************************************
  Sets @param instruction to lowercase.
 ******************************************************************************/
void toLow(char *instruction){
    int i;
    for(i=0;i<strlen(instruction);i++){
        instruction[i] = tolower(instruction[i]);
    }
}
/******************************************************************************
  Translates decimal format to its binary form.
 ******************************************************************************/
long toBinary(int n) {
    int remain;
    long bin = 0;
    long i = 1;
    while(n != 0) {remain = n%2; n = n/2; bin = bin + (remain * i); i = i * 10;}
    return bin;
}
/******************************************************************************
  Converts the BYTE directive operand to its corresponding hex value.
 ******************************************************************************/
int toHex(char* buffer, char* op){
    int i, is=0, apos=0, tmp;
    char temp[30];
    temp[0] = buffer[0]='\0';
    if(op[0]=='C'||op[0]=='c') {
        for(i=0;i<strlen(op);i++){
            if((op[i]=='C'||op[0]=='c')&&is==0) is==1;
            else if((int)op[i] != 39) {
                snprintf(temp,4, "%02X", (int)op[i]);
                strcat(buffer,temp);
            }
            else if((int)op[i] == 39&&apos==0) apos=1;
            else if((int)op[i] == 39&&apos==1) return 0;
        }
    }
    else if(op[0]=='X'||op[0]=='x'){
        for(i=2;i<strlen(op)-1;i++) {
            snprintf(temp,2,"%c",op[i]);
            strcat(buffer,temp);            
        }
        return 0;
    }
    return -1;
}

/******************************************************************************
 ******************************************************************************
  Miscellaneous functions used by Pass 2.                                       ***************************************************     @Pass2
 ******************************************************************************
 ******************************************************************************/

/******************************************************************************
  Sets the artifacts from file @param fp accordingly into struct @param commin.
  
 When @param head = 1
 * Placing artifacts for head line.
 When @param head = 0
 * Placing artifacts for regular line.
 ******************************************************************************/
void setArtifacts(FILE* fp, assyLine* commin, int head){
    clearStruct(commin);
    fgets(commin->line,sizeof(commin->line),fp);
    commin->line[strlen(commin->line)-1]='\0';
    if(commin->line[0]!='.'){
        fgets(commin->LOCCTR,sizeof(commin->LOCCTR),fp);
        commin->LOCCTR[strlen(commin->LOCCTR)-1]='\0';
        if(head==0){
            fgets(commin->Opcode,sizeof(commin->Opcode),fp);
            fgets(commin->Operand,sizeof(commin->Operand),fp);
            commin->Opcode[strlen(commin->Opcode)-1]='\0';
            commin->Operand[strlen(commin->Operand)-1]='\0';
        }
        fgets(commin->psw,sizeof(commin->psw),fp);
        commin->psw[strlen(commin->psw)-1]='\0';
    }
}
/******************************************************************************
  Places header and end records with correspoding contents from @param commin 
 into @param fp.
 
 When @param op = 0: 
 * Writing Header Record
 * @param s0 = Program Name
 * @param s1 = Program Length
 * 
 When @param op = 1:
 * Writing End Record
 * @param s0 = Address of first executable instruction in the program.
 ******************************************************************************/
void placeHeadEndRecord(FILE* fp, assyLine* commin,int op,char* s0,char* s1){
    char head[19],end[7];
    head[0]=end[0]='\0';
    if(op==0){              //Place header record
        //Col. 1        -H
        strncat(head,"H",sizeof(head));
        //Col. 2-7      -Program Name
        fillSpaces(s0,6);
        strncat(head,s0,sizeof(head));
        //Col. 8-13     -Starting address of object program (hexadecimal)
        fillZeros(commin->LOCCTR);
        strncat(head,commin->LOCCTR,sizeof(head));
        //Col. 14-19    -Length of object program in bytes (hexadecimal)
        fillZeros(s1);
        strncat(head,s1,sizeof(head));
        strcat(head,"\n");
        //Place in Object File
        fputs(head,fp);
        return;
    }
    else if (op==1){        //Place end record
        //Col. 1        -E
        strncat(end,"E",sizeof(end));
        //Col. 2-7      -Address of first executable instruction in object prog
        fillZeros(s0);
        strncat(end,s0,sizeof(end));
        //Place in Object File
        fputs(end,fp);
        return;
    }
    else
        return;
}
/******************************************************************************
  Places the current Text record @param record into the Object File @param fp 
  proceeds to intialize the new text record.
 ******************************************************************************/
void TextRecToObFile(char* record, FILE* fp, char* address,int err){
    setTextLength(record);
    strcat(record,"\n");
    if((strlen(record)>10)&&(err!=1)) fputs(record, fp);
    initTextRecord(record,address);
}
/******************************************************************************
  Adds the current Object Code @param obcode to current record @param record.
 ******************************************************************************/
void addTextObcode(char* record,char* obcode){
    strncat(record,obcode,8);
    setTextLength(record);
}
/******************************************************************************
  Adds the current Text record's size into the record @param record.
 ******************************************************************************/
void setTextLength(char* record){
    char lenHex[5];
    int lenDec = ((strlen(record)-9)/2);
    snprintf(lenHex,sizeof(lenHex),"%02X",lenDec);
    record[7]=lenHex[0];
    record[8]=lenHex[1];
}
/******************************************************************************
  Initialize the text record in @param record with @param address.
 ******************************************************************************/
void initTextRecord(char* record, char* address){
    char tmp[12];
    record[0]='\0'; 
    strncpy(tmp,address,sizeof(tmp));
    fillZeros(&tmp[0]);
    snprintf(record,10,"T%s00",tmp);
}
/******************************************************************************
  Assembles the Object from current contents of @param commin and @param symTab.
 ******************************************************************************/
void setObcode(assyLine* commin, StrMap* symTab){
    int i, x=0;
    char address[10],symbol[10];
    commin->ObCode[0]='\0';
    if(commin->Opcode[0]!='%'){
        strncat(commin->ObCode,commin->Opcode,sizeof(commin->ObCode));
        if((commin->Operand[strlen(commin->Operand)-1]=='x'||
                commin->Operand[strlen(commin->Operand)-1]=='X')&&
                commin->Operand[strlen(commin->Operand)-2]==',') {
            for(i=0;i<(strlen(commin->Operand)-2);i++) symbol[i]=commin->Operand[i];
            x=1;
        }
        else strncpy(symbol,commin->Operand,sizeof(commin->Operand));
        
        if(sm_get(symTab,symbol,address,sizeof(address))!=0){
            if(x==1){
                int a;
                sscanf(address,"%X",&a);
                snprintf(address,sizeof(address),"%X",(a+32768));
            }
            strncat(commin->ObCode,address,sizeof(commin->ObCode));
        }
        else if(strcasecmp(symbol,"%NOOPERAND")==0)
            strncat(commin->ObCode,"0000",sizeof(commin->ObCode));            
        else {
            commin->psw[9]='1';
            strncat(commin->ObCode,"0000",sizeof(commin->ObCode));
        }
    }
    else if(strcasecmp(commin->Opcode,"%BYTE")==0||strcasecmp(commin->Opcode,"%WORD")==0){
        strncpy(commin->ObCode,commin->Operand,sizeof(commin->ObCode));
    }
    else if(strcasecmp(commin->Opcode,"%RESW")==0||strcasecmp(commin->Opcode,"%RESB")==0)
        commin->ObCode[0]='\0';    
}

/******************************************************************************
  Places a line with current contents in @param commin into @param fp.
 ******************************************************************************/
void putListingLine(FILE* fp, assyLine* commin){
    char line[250];
    snprintf(line, 500, "%s\t|\t%s\t\t|\t%s\n", commin->LOCCTR, commin->ObCode, 
            commin->line);
    fputs(line,fp);
    setErrors(fp,commin->psw);
}
void fillZeros(char* cur){
    int a;
    sscanf(cur,"%X",&a);
    snprintf(cur,sizeof(cur),"%06X",a);
}
/******************************************************************************
 ***  The following are miscellaneous functions used by Pass 1 and Pass 2.  ***
 ******************************************************************************/
void fillSpaces(char* col, int sizeReq){
    int i;
    for(i = 0; i<abs(strlen(col)-sizeReq);i++) strcat(col," ");
}


/******************************************************************************
 ******************************************************************************
  Miscellaneous functions used by both Pass 1 and Pass 2.                       ***************************************************     @Both
 ******************************************************************************
 ******************************************************************************/


/******************************************************************************
  Sets the names and extensions for files in Pass 1 and Pass 2.
 ******************************************************************************/
void replaceExtension(char* f0, char* f1, char* f2, int pass){
    int i;
    if(pass == 1){
        for(i=0;i<strlen(f0);i++){
            if(f0[i]=='.') {
                f0[i]='\0'; 
                strncpy(f1,f0,260); 
                strncat(f1, "_symTab.txt",59);
                strncat(f0,"_Int.asm",59);
                return;
    }}}
    else if(pass == 2){
        strncpy(f1,f0,sizeof(f1));
        for(i=0;i<strlen(f1);i++){
            if(f1[i]=='_') {
                f1[i]='\0';
                strncpy(f2,f1,sizeof(f2));
                strncat(f2, "_listing.txt",59);
                strncat(f1,".ob",59);
                return;
    }}}
}
/******************************************************************************
  Initializes the struct to NULL all its part to read next line.
 ******************************************************************************/
void clearStruct(assyLine* commin){
    commin->Comment[0] = commin->LOCCTR[0] = commin->Label[0] =
            commin->ObCode[0] = commin->Opcode[0] = commin->Operand[0] = 
            commin->Operation[0] = commin->line[0] = commin->psw[0] = '\0';
    commin->PSW = 0;
}