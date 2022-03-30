/* Toy synchronizer: Sample template
 * Your synchronizer should implement the three functions listed below.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

//1=P;2=PP;3=V;4=VV
#define MAX_OP  30
#define MAX_OP_LENGTH 20
#define MAX_EXP_LENGTH 50

int sem_num = 0;
int op_num = 0;
int max_len_of_str = 0;

sem_t sem[MAX_OP];
int sem_flag[MAX_OP]; // initialize to 0
int sem_count[MAX_OP]; // initialize to 0

struct _op_str
{
   char* name;
   int enter[10];
   int exit[10];
};
struct _op_str op_str[MAX_OP];


void init_op_str ();
int find_symb(char * save1_op[], int min, int max, const char* symb);
void print_op_str();
int if_op_in_op_str(char * save1_op_j);
void init_sem();
void init_op_str_to_path (char * save1_op[]);
char ** parser ( const char * path_exp);

void _pp(sem_t* mutex1, int* count, sem_t*  mutex2)
{
  //printf("_pp: %d\n",*count);
  sem_wait(mutex1);
  *count = *count + 1;
  //printf("_pp: %d\n",*count);
  if (*count == 1) sem_wait(mutex2);
  //printf("_pp: %d\n",*count);
  sem_post(mutex1);
}

void _vv(sem_t* mutex1, int* count, sem_t*  mutex2)
{
  //printf("_vv: %d\n",*count);
  sem_wait(mutex1);
  *count = *count - 1;
  if (*count == 0) sem_post(mutex2);
  sem_post(mutex1);
}

void ENTER_OPERATION(const char *op_name)
{
    //printf("Entering: \t %s\n", op_name);
    int j = if_op_in_op_str(op_name);
    if(j!=MAX_OP)
    {
      switch (op_str[j].enter[0])
      {
         case 1: sem_wait(&(sem[(op_str[j].enter[1])])); // p
                    break;
         case 2: _pp(&(sem[(op_str[j].enter[1])]),&(sem_count[(op_str[j].enter[1])]),&(sem[(op_str[j].enter[3])]));
                    break;
         case 3: sem_post(&(sem[(op_str[j].enter[1])])); // v
                    break;
         case 4: _vv(&(sem[(op_str[j].enter[1])]),&(sem_count[(op_str[j].enter[1])]),&(sem[(op_str[j].enter[3])]));
                    break;
           default: //printf("SOMETHING IS WRONG!!!");
                    break;
       }
     }
}

void EXIT_OPERATION(const char *op_name)
{
    //printf("Exiting: \t %s\n", op_name);
    int j = if_op_in_op_str(op_name);
    if(j!=MAX_OP)
    {
      switch (op_str[j].exit[0])
      {
        case 1: sem_wait(&(sem[(op_str[j].exit[1])])); // p
                   break;
        case 2: _pp(&(sem[(op_str[j].exit[1])]),&(sem_count[(op_str[j].exit[1])]),&(sem[(op_str[j].exit[3])]));
                   break;
        case 3: sem_post(&(sem[(op_str[j].exit[1])])); // v
                   break;
        case 4: _vv(&(sem[(op_str[j].exit[1])]),&(sem_count[(op_str[j].exit[1])]),&(sem[(op_str[j].exit[3])]));
                   break;
          default: //printf("SOMETHING IS WRONG!!!");
                   break;
       }
     }
}

void INIT_SYNCHRONIZER(const char *path_exp)
{
  printf("Initializing Synchronizer with path_exp %s\n", path_exp);



  int keylen = strlen(path_exp) + 1;
  char * key = (char*)malloc(keylen * sizeof(char));

  strcpy(key, path_exp);

  char ** str = parser(key);
  int r=0;
  for(r=0;r<=max_len_of_str;r++)
  {
    //printf(" r =%d str[r]=%s\n",r,str[r] );
  }


  // char *save1_op[20]={
  //   "path", "append", ";", "remove", "end", "NULL"
  // };

  init_op_str_to_path(str);


  //print_op_str();
  init_sem();



}

void init_op_str_to_path (char * save1_op[])
{
  int i=0;
  int len=0;
  int j=0;
  int r =0;
  int search_bef = 0;
  int search_aft = 0;
  int found_at = 0;
  int max_array = 0;
  int path_index[MAX_OP];
  int end_index[MAX_OP];


  for(r = 0; r< MAX_OP; r++)
  {
    path_index[r] = -1;
    end_index[r] = -1;
  }
  //Put all path_index:
  r=0;
  for(i=0;strcmp(save1_op[i],"NULL")!=0;i++)
  {
    if(strcmp(save1_op[i],"path")==0)
    {
      path_index[r]=i;

      //printf(" path r = %d; path_index[r]= %d \n",r, path_index[r]);
      r++;
    }
  }
  //Put all end_index:
  r=0;
  for(i=0;strcmp(save1_op[i],"NULL")!=0;i++)
  {
    if(strcmp(save1_op[i],"end")==0)
    {
      end_index[r]=i;

      //printf(" end r = %d; end_index[r]= %d \n",r, end_index[r]);
      r++;
    }
  }
  //printf("at this pos 1\n" );
  init_op_str();
  int start_index =0;
  //start doing op sorting path by path///////////////
  for(r=0;path_index[r]!=-1;r++)
  {
    sem_num++;
    //printf("at this pos r = %d and path[r]= %d\n",r , path_index[r]);
    start_index = path_index[r];
    max_array = end_index[r] + 1;
    j=start_index;//index for path
    j++;// next { or synchronizer
    while(strcmp(save1_op[j],"{")==0 && j<max_array) j++;
    if (j<max_array)
    {
      op_str[op_num].name = save1_op[j];
      op_str[op_num].enter[0] = 1; //p
      op_str[op_num].enter[1] = sem_num; //0 here
      op_str[op_num].enter[2] = 1; // sem value initialized
      op_str[op_num].enter[3] = -1;// no P or V after the first one at index 0
    }
    ///////////////////////Path(END)///////////////////////////////
//printf("-------end of start %d max array %d\n",start_index,max_array);
    ///////////////////////End(START)///////////////////////////////
    j=max_array -1; //index for end
    j--;
    while(strcmp(save1_op[j],"}")==0 && j>start_index) j--;
    //printf("-------start of end\n");
    if (j>start_index){ // checking if there are any ops at all
      if(op_str[op_num].name == save1_op[j])
      { // if there is only one op between path and end
        op_str[op_num].exit[0] = 3; //v
        op_str[op_num].exit[1] = sem_num; //0 here
        op_str[op_num].exit[2] = 1; // sem value initialized
        op_str[op_num].exit[3] = -1;// no P or V after the first one at index 0
      }
      else
      {
        //Its another Op so increase op num count by 1
        op_num++;
        op_str[op_num].name = save1_op[j];
        op_str[op_num].exit[0] = 3; //v
        op_str[op_num].exit[1] = sem_num; //0 here
        op_str[op_num].exit[2] = 1; // sem value initialized
        op_str[op_num].exit[3] = -1;// no P or V after the first one at index 0
      }
    }
    ///////////////////////END(END)///////////////////////////////
//printf("-------end of end\n");
    ///////////////////OP: + (START)////////////////////////////
    found_at=find_symb(save1_op,start_index,max_array,"+");
    j = found_at;
    //+ : either this or that
    if (j<max_array){
      //find op before +/////////////////////////////
      j--;
      while(strcmp(save1_op[j],"}")==0 && j>start_index) j--;// look for op before +
      if (j>start_index) //checking if there are any ops at all before +
      {
        search_bef = if_op_in_op_str(save1_op[j]);// check if this op in op_str
        if(search_bef!=MAX_OP)
        {
          op_str[search_bef].exit[0] = op_str[search_bef].enter[0] + 2; // if enter was p then v; if enter was pp exit is vv
          op_str[search_bef].exit[1] = op_str[search_bef].enter[1] ; // enter and exit have same sem number
          op_str[search_bef].exit[2] = 1; // sem value initialized
          op_str[search_bef].exit[3] = -1;// no P or V after the first one at index 0
        }
        else
        {
          // op is not in op_str
          // so increment the op_num count
          op_num++;
          //increase sem_num
          sem_num++;
          op_str[op_num].name = save1_op[j];
          op_str[op_num].enter[0] = 1;
          op_str[op_num].enter[1] = sem_num;
          op_str[op_num].enter[2] = 1; // sem value initialized
          op_str[op_num].enter[3] = -1;// no P or V after the first one at index 0
          op_str[op_num].exit[0] = 3;
          op_str[op_num].exit[1] = sem_num;
          op_str[op_num].exit[2] = 1; // sem value initialized
          op_str[op_num].exit[3] = -1;// no P or V after the first one at index 0
        }

      }
      //printf("-------1\n");
      ///////////////////////////////////////////////
      //find op after +/////////////////////////////
      j = found_at;
      j++;
      while(strcmp(save1_op[j],"{")==0 && j<max_array) j++;// look for op after +
      if (j<max_array) //checking if there are any ops at all after + if yes then:
      {
        search_aft = if_op_in_op_str(save1_op[j]);// check if this op in op_str
        if(search_aft!=MAX_OP)
        {
          op_str[search_aft].enter[0] = op_str[search_aft].exit[0] - 2; // if exit was v then p; if exit was vv exit is pp
          op_str[search_aft].enter[1] = op_str[search_aft].exit[1] ; // enter and exit have same sem number
          op_str[search_aft].enter[2] = 1; // sem value initialized
          op_str[search_aft].enter[3] = -1;// no P or V after the first one at index 0
        }
        else
        {
          // op is not in op_str
          // so increment the op_num count
          op_num++;
          //dont increase sem_num as we need to use same sem as before +
          op_str[op_num].name = save1_op[j];
          op_str[op_num].enter[0] = 1;//p
          op_str[op_num].enter[1] = sem_num;
          op_str[op_num].enter[2] = 1; // sem value initialized
          op_str[op_num].enter[3] = -1;// no P or V after the first one at index 0
          op_str[op_num].exit[0] = 3;//v
          op_str[op_num].exit[1] = sem_num;
          op_str[op_num].exit[2] = 1; // sem value initialized
          op_str[op_num].exit[3] = -1;// no P or V after the first one at index 0
        }

      }
      //////////////////////////////////////////////
    }
  /////////////////OP: + (END)////////////////////////////


  /////////////////OP: ; (Start)////////////////////////////
  found_at=start_index;
while(found_at<max_array)
{
  //printf("-------2\n");
  found_at=find_symb(save1_op,found_at,max_array,";");
  j = found_at;
  //; : first before then afterprintf("-------2\n");
  //printf("-------2\n");
  if (j<max_array){ // found a ";" in the path expression
    //find op before ;/////////////////////////////
    j--;
    while(strcmp(save1_op[j],"}")==0 && j>start_index) j--;// look for op before ;
    if (j>start_index) //checking if there are any ops at all before ;
    {
      search_bef = if_op_in_op_str(save1_op[j]);// check if this op in op_str
      //printf("-------2 %d\n",found_at);
      if(search_bef!=MAX_OP)
      {
        //printf("-------2 %d\n",found_at);
        op_str[search_bef].exit[0] = 3; // it is v(s_new)
        //new sem so inc sem_num count:
        sem_num++;
        op_str[search_bef].exit[1] = sem_num ; // enter and exit have same sem number
        op_str[search_bef].exit[2] = 0; // sem value initialized
        op_str[search_bef].exit[3] = -1;// no P or V after the first one at index 0
      }
      else
      {
        // op is not in op_str
        // so increment the op_num count
        //printf("-------2 %d\n",found_at);
        op_num++;
        op_str[op_num].exit[0] = 3; // it is v(s_new)
        //new sem so inc sem_num count:
        sem_num++;
        op_str[op_num].exit[1] = sem_num ; // enter and exit have same sem number
        op_str[op_num].exit[2] = 0; // sem value initialized
        op_str[op_num].exit[3] = -1;// no P or V after the first one at index 0
      }

    }
    ///////////////////////////////////////////////
    //find op after ;/////////////////////////////
    j = found_at;
    j++;
    while(strcmp(save1_op[j],"{")==0 && j<max_array) j++;// look for op after ;
    if (j<max_array) //checking if there are any ops at all after ; if yes then:
    {
      search_aft = if_op_in_op_str(save1_op[j]);// check if this op in op_str
      if(search_aft!=MAX_OP)
      {
        //printf("-------2 %d\n",found_at);
        op_str[search_aft].enter[0] = 1; // p(sem_new)
        op_str[search_aft].enter[1] = sem_num ; // enter and exit have same sem number
        op_str[search_aft].enter[2] = 0; // sem value initialized
        op_str[search_aft].enter[3] = -1;// no P or V after the first one at index 0
      }
      else
      {
        // op is not in op_str
        // so increment the op_num count
        //printf("-------2 %d\n",found_at);
        op_num++;
        //dont increase sem_num as we need to use same sem as before ;
        op_str[op_num].name = save1_op[j];
        op_str[op_num].enter[0] = 1;
        op_str[op_num].enter[1] = sem_num;
        op_str[op_num].enter[2] = 0; // sem value initialized
        op_str[op_num].enter[3] = -1;// no P or V after the first one at index 0

      }

    }
    //////////////////////////////////////////////
  }
  found_at++;
  //printf("-------2 %d\n",found_at);
}
  /////////////////OP: ; (END)////////////////////////////
  //printf("-------3\n");

  /////////////////OP: {} (Start)////////////////////////////
  found_at=start_index;
  while(found_at<max_array)
  {
    found_at=find_symb(save1_op,found_at,max_array,"{");
    j = found_at;
    //{ : only after
    if (j<max_array){ // found a "{" in the path expression
      //find op after {/////////////////////////////
      j++;
      //while(strcmp(save1_op[j],"{")==0 && j<max_array) j++;// look for op after ;
      if (j<max_array) //checking if there are any ops at all after { if yes then:
      {
        search_aft = if_op_in_op_str(save1_op[j]);// check if this op in op_str
        if(search_aft!=MAX_OP)
        {
          //Make the prev sem at this op to the third argument and keep its sem init value at 4th index
          op_str[search_aft].enter[3] = op_str[search_aft].enter[1];
          op_str[search_aft].enter[4] = op_str[search_aft].enter[2];
          op_str[search_aft].enter[0] = 2; // pp(sem_new)
          sem_num++;
          op_str[search_aft].enter[1] = sem_num ; // enter and exit have same sem number
          op_str[search_aft].enter[2] = 1; // sem value initialized
          op_str[search_aft].enter[5] = -1;
        }
      }
    }
    //////////////////////////////////////////////
    found_at=find_symb(save1_op,found_at,max_array,"}");
    j = found_at;
    //} : only after
    if (j<max_array){ // found a "{" in the path expression
      //find op before ;/////////////////////////////
      j--;
      //while(strcmp(save1_op[j],"{")==0 && j<max_array) j++;// look for op after ;
      if (j>start_index) //checking if there are any ops at all after { if yes then:
      {
        search_bef = if_op_in_op_str(save1_op[j]);// check if this op in op_str
        if(search_bef!=MAX_OP)
        {
          //Make the prev sem at this op to the third argument and keep its sem init value at 4th index
          op_str[search_bef].exit[3] = op_str[search_bef].exit[1];
          op_str[search_bef].exit[4] = op_str[search_bef].exit[2];
          op_str[search_bef].exit[0] = 4; // vv(sem_new)
          //sem_num++;
          op_str[search_bef].exit[1] = sem_num ; // enter and exit have same sem number
          op_str[search_bef].exit[2] = 1; // sem value initialized
          op_str[search_bef].exit[5] = -1;
        }
      }
      //////////////////////////////////////////////
    }
  }

  op_num++;
  //printf(" sss at this pos r = %d and path[r]= %d\n",r , path_index[r]);
  }

}


int find_symb(char * save1_op[], int min, int max, const char* symb){
  int j=min;
  while((j<max) && (strcmp(save1_op[j],symb)!=0)){
    //printf("hi there: %s\n", save1_op[j]);
    //printf("%d\n", max);
    j++;
  }
  return j;
}


void init_sem()
{
  int j=0;
  //printf("In INIT_SEM\n");
  while(strcmp(op_str[j].name,"None")!=0)
  {
    int i=1; // index 0 tells if its p or v or pp or vv
    //printf("In INIT_SEM %s\n",op_str[j].name);
    while(op_str[j].enter[i] != -1 )
    {
      //printf("In INIT_SEM enter [i] %d , %s\n",op_str[j].enter[i],op_str[j].name);
      if(sem_flag[op_str[j].enter[i]]==0)
      {
        sem_init(&(sem[op_str[j].enter[i]]), 0, op_str[j].enter[i+1]);
        sem_flag[op_str[j].enter[i]]=1;

      }
      i = i + 2;
    }
    j++;
  }
}


int if_op_in_op_str(char * save1_op_j)
{
  int j = 0;
  while((j<MAX_OP) && (strcmp(op_str[j].name,save1_op_j)!=0))
  {
    j++;
  }

  return j;
}

void print_op_str()
{
  int j;
  for(j = 0; j<MAX_OP; j++){
    printf("============================================================================\n");
    printf("OP_STR num = %d\n", j );
    printf("OP_name = %s\n", op_str[j].name);
    printf("OP_STR enter[0], [1], [2], [3], [4], [5], [6] = %d, %d, %d, %d, %d, %d\n", op_str[j].enter[0],op_str[j].enter[1],op_str[j].enter[2],op_str[j].enter[3],op_str[j].enter[4],op_str[j].enter[5],op_str[j].enter[6]);
    printf("OP_STR exit [0], [1], [2], [3], [4], [5], [6] = %d, %d, %d, %d, %d, %d\n", op_str[j].exit[0],op_str[j].exit[1],op_str[j].exit[2],op_str[j].exit[3],op_str[j].exit[4],op_str[j].exit[5],op_str[j].exit[6]);
    printf("============================================================================\n");
  }
}

void init_op_str ()
{
  int i;
  for(i=0;i< MAX_OP;i++)
  {
    op_str[i].name = "None";
    sem_flag[i] = 0;
    sem_count[i] = 0;
  }
}

char ** parser ( const char * path_exp)
{
  int keylen = strlen(path_exp) + 1;
  char * key = (char*)malloc(keylen * sizeof(char));

  strcpy(key, path_exp);

  //printf ("3a \n");
  char ** str;
  //int num_of_str = 100;

  str = malloc(100 * (sizeof(char*)));
  //printf ("3b \n");
  int k;
  for (k = 0; k < 100; k++)
  {
    //printf ("3c7788 \n");
    str[k] = malloc((100) * sizeof(char));
    //printf ("3c \n");
  }

  int i = 0;
  int j = -1;
  int x = 0;
  int flag = 0;
  //char * ps;

  //printf ("3a %c\n",*ps);
  //printf ("=========================\n");
  //printf ("%c\n", key[keylen]);
  if(key[keylen]!='\0')
  {
    //printf("incorrect\n");
  }
  else
  {
    //printf("correct\n");
  }

  for(i=0;key[i]!='\0';i++)
  {
    //printf ("1 %c\n", key[i]);
    flag = 0;
    if(key[i]==';' || key[i]=='}' ||key[i]=='{' ||key[i]=='+')
    {

      j++;
      //printf ("5\n");
      //printf ("2 aaaai=%d, j=%d\n",i, j );
      strncat(str[j],&key[i],1);
      //printf ("6\n");

    }

    if(key[i]!=';' && key[i]!='}' && key[i]!='{' && key[i]!='+' && key[i]!=' ')
    {
      j++;
    }

    while((key[i]!=';') && (key[i]!='}') && (key[i]!='{') && (key[i]!='+') && (key[i]!=' ') && (key[i]!='\0'))
    {
      //printf ("2 i=%d, j=%d\n",i, j );
      //j++;
      //ps = str + i;
      //printf ("3a %c\n",key[i]);
      //strncat(str, &ch, 1);
      strncat(str[j],&key[i],1);
      //printf ("3b %s\n",str[j]);
      //ps++;
      i++;
      flag = 1;
    }
    //j++;
    if(flag == 1)
    {
      i--;
      flag =0;
    }
    //printf ("4\n");

    // if( key[i]=='\0')
    // {
    //   i--;
    // }
    //

  }
  j++;
  str[j]="NULL";
  max_len_of_str = j;
  return str;
}
