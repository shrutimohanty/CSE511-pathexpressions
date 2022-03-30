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
#define MAX_OP_LENGTH 20
#define MAX_EXP_LENGTH 50

//  GLOBAL VARS

int DMSG = 0;

//char READ_OP[] = "read", WRITE_OP[] = "write", PATH_EXP[] = "path {read}+write end", RM_PATH[] = "path ", RM_END[] = " end";
char READ_OP[] = "read", WRITE_OP[] = "write", RM_PATH[] = "path", RM_END[] = " end";
char PATH_EXP[] = "path open;{read1}+{read2;write1}+{read3+write2};close end";
char ops[MAX_EXP_LENGTH][MAX_OP_LENGTH];
char spc = ' ';
// END GLOBAL VARS

struct string_list
{
    char data[MAX_OP_LENGTH];
    struct string_list* next;
    //struct string_list* prev;
};
typedef struct string_list Node;
typedef struct string_list* NodePointer;

void insertWord(char word2[], NodePointer *head2){
//pointer to a new node to be inserted into linked list
   NodePointer newNode = NULL;
//pointer to the previous node in the linked list
   NodePointer previous = NULL;
//pointer to the current node in the linked list
   NodePointer current = *head2;

//create a node on the heap
   newNode = malloc(sizeof(Node));
/*
check to see if space is available
if no space on heap, malloc() will return NULL
*/
   if(NULL != newNode){
   //copy over word to the node
      strcpy(newNode->data, word2);
   //figure out where to insert in linked list
      while(NULL != current && strcmp(word2, current->data)>0){
      //move previous to current
         previous = current;
      //move current to next node
         current = current->next;
      }//end of while
      //insert at beginning of linked list
      if(NULL == previous){
         newNode->next = current;
         //change the address stored in head
         *head2 = newNode; 
      }//end of if
      else{
      //insert between previous and current
         previous->next = newNode;
         newNode->next = current;
      }//end of else
   }//end of if
}//end of function


void deleteWord(char word3[], NodePointer *head3){
//pointer to a temp node to be deleted from linked list
   NodePointer tempNode = NULL;
//pointer to the previous node in the linked list
   NodePointer previous = NULL;
//pointer to the current node in the linked list
   NodePointer current = *head3;

//check for empty list
   if(NULL == current){
      printf("Cannot delete from empty list!\n");
      return;
   }

/*
check to see if 1st node can be deleted
*/
   if(0 == strcmp(word3, current->data)){
   //get address of 1st node
      tempNode = current;
   //change head to next node
      *head3 = current->next;
   //delete the node
      printf("deleting \"%s\" . . .\n", tempNode->data);
      free(tempNode);
   }
   else{
   //loop through linked list
      while(NULL != current && 0 != strcmp(word3, current->data)){
         //printf("current = %s\n", current->data);
      //move to next node in linked list
         previous = current;
         current = current->next;   
      }//end of while
      if(NULL != current){
      //get current node's address and store in tempNode
         tempNode = current;
         //printf("tempNode = %s\n", tempNode->data);
      //skip over current node
         previous->next = current->next;
      //delete node at current pointer
         printf("deleting \"%s\" . . .\n", tempNode->data);
         free(tempNode);
      }//end of if
   }//end of else    
}//end of function

void displayList(NodePointer current){
   //for empty list
   if(NULL == current){
      printf("The linked list is empty!\n\n");
      return;
   }
   printf("linked list items: ");
   //loop through list
   while(NULL != current){
   //display each node
      printf("%s, ", current->data);
      //go to next node
      current = current->next;
   }
   printf("\n\n");
}//end of function



// Useful Funcs

void removeSubstring(char *s,const char *toremove)
{
  while( s=strstr(s,toremove) )
    memmove(s,s+strlen(toremove),1+strlen(s+strlen(toremove)));
}

int parse_pathop(){
    
    int i = 0, opcnt = 0, opslen = 0;
    int length = strlen(PATH_EXP);
    char op[MAX_OP_LENGTH];
    char sm[MAX_OP_LENGTH];

    while (i < length){
       *op = '\0';
        *sm = '\0';
        while (!((PATH_EXP[i] == ' ') || (PATH_EXP[i] == '{') || (PATH_EXP[i] == '}') || (PATH_EXP[i] == '+') || (PATH_EXP[i] == ';') || (PATH_EXP[i] == ',') ) ) {
            strncat (op, &PATH_EXP[i],1);
            i++;
        }
        strcpy(ops[opcnt], op);
        opcnt ++;
        strncat (sm, &PATH_EXP[i],1);
        strcpy(ops[opcnt], sm);
        opcnt ++;
        i++;
    }
    return opcnt;

}


// End useful funcs

//main function
void main() {

//  local vars
    int oplength = 0;
    int pcnt = 0;
    int parse_cnt = 0;
    char parsed_string[MAX_EXP_LENGTH][MAX_OP_LENGTH];
    NodePointer head = NULL; 
    //char endchar = ',';
//  local vars end
    
    //strncat(PATH_EXP, &endchar, 1);
    oplength = parse_pathop();
    for (pcnt = 0; pcnt < oplength; pcnt++){
        if (!((*ops[pcnt] == '\0') || (!strcmp(ops[pcnt]," ")))) {
            //printf("op is \t %s\n", ops[pcnt]);
            strcpy(parsed_string[parse_cnt], ops[pcnt]);
            parse_cnt++;
        }
    }

    //for (pcnt = 0; pcnt < parse_cnt; pcnt++) {
    for (pcnt = 0; pcnt < 5; pcnt++) {
            printf("op is \t %s\n", parsed_string[pcnt]);
            insertWord(parsed_string[pcnt], &head);
            displayList(head);
    }
}

