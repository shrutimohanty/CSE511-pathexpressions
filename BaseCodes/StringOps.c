#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

//#include <regex.h>

//regex_t reg_ex;
//int reti;
//char msgbuf[100];

// #defs
#define MAX_OP_LENGTH 50

//  GLOBAL VARS

int DMSG = 0;

//char READ_OP[] = "read", WRITE_OP[] = "write", PATH_EXP[] = "path {read}+write end", RM_PATH[] = "path ", RM_END[] = " end";
char READ_OP[] = "read", WRITE_OP[] = "write", RM_PATH[] = "path", RM_END[] = " end";
char PATH_EXP[] = "path open;{read1}+{read2;write1}+{read3+write2};close end";
//char PATH_EXP[] = "path {read}+close end";
char spc = ' ';
// END GLOBAL VARS


// Useful Funcs

void removeSubstring(char *s,const char *toremove)
{
  while( s=strstr(s,toremove) )
    memmove(s,s+strlen(toremove),1+strlen(s+strlen(toremove)));
}

void spitops(){
    
    int i = 0, opcnt = 1;
    int length = strlen(PATH_EXP);
    char ops[MAX_OP_LENGTH];
    
    for(i = 0; i < length; i++){
        //i = 0;
        if ((PATH_EXP[i] == ' ') || (PATH_EXP[i] == '{') || (PATH_EXP[i] == '}') || (PATH_EXP[i] == '+') || (PATH_EXP[i] == ';') ) {
            opcnt = 1;
            while (!((PATH_EXP[i + opcnt] == '{') || (PATH_EXP[i + opcnt] == '}') || (PATH_EXP[i + opcnt] == '+') || (PATH_EXP[i + opcnt] == ';') || (PATH_EXP[i + opcnt] == ',') ) ) {
                strncat(ops, &PATH_EXP[i + opcnt], 1);
                opcnt++;
            }
                strncat(ops, &spc, 1);

        }

    }

    printf("%d .... %s\n", DMSG++, ops);
}

void get_string() {
    int i = 0, opcnt = 0, opslen = 0;
    int length = strlen(PATH_EXP);
    char ops[MAX_OP_LENGTH][MAX_OP_LENGTH];
    char op[MAX_OP_LENGTH];
    char sm[MAX_OP_LENGTH];

    while (i < length){
       *op = '\0';
        //memset(op,0,strlen(op));
        memset(sm,0,strlen(sm));
        *sm = '\0';
        while (!((PATH_EXP[i] == ' ') || (PATH_EXP[i] == '{') || (PATH_EXP[i] == '}') || (PATH_EXP[i] == '+') || (PATH_EXP[i] == ';') || (PATH_EXP[i] == ',') ) ) {
            strncat (op, &PATH_EXP[i],1);
            i++;
            //printf("DEBUG index op i %d\n", i);
            //printf("DEBUG sting %s\n", op);
        }
        strcpy(ops[opcnt], op);
        opcnt ++;
        //i++;
        //printf("DEBUG index op i %d\n", i);
        strncat (sm, &PATH_EXP[i],1);
        //printf("DEBUG path %s\n", sm);
        strcpy(ops[opcnt], sm);
        opcnt ++;
        i++;
        //printf("DEBUG index op i %d\n", i);
    }

    opslen = opcnt;
    printf("opslen is %d\n", opslen);
    for (i = 0; i < opcnt; i++)
        printf("the operation is %s\n", ops[i]);
}

void strip_string(){
    char delimit[]= " {}+";
    char *pChr = strtok (PATH_EXP, " {}+");
    while (pChr != NULL) {
        printf ("%s ", pChr);    
        pChr = strtok (NULL, ",*");
    }
    putchar ('\n');
    printf("the 1st string %s\n", pChr);
}

// End useful funcs

//main function
void main() {

//  local vars
    
    //int i = 0, j = 0; 
    //char op[] = "";
    //int opcnt = 0;
    //char endchar = ',';
    int plength = 0;
    int pcnt = 0;

//  local vars end

    printf("%d .... Initializing Synchronizer with path_exp %s\n", DMSG++, PATH_EXP);


    //char* token = strtok(PATH_EXP, "path"); 
    //removeSubstring (PATH_EXP, RM_PATH);
    //removeSubstring (PATH_EXP, RM_END);
    printf("%d .... Initializing Synchronizer with path_exp %s\n", DMSG++, PATH_EXP);
    //strncat(PATH_EXP, &endchar, 1);
    plength = strlen(PATH_EXP);

    printf("%d .... string length %d\n", DMSG++, plength);
    //while (pcnt < plength){
        //printf("PRNING STRING: %s", PATH_EXP[pcnt]);
    //}
    //strncat(ops, &PATH_EXP[5], 1);
    //spitops();
    get_string();
    //strip_string();
}

