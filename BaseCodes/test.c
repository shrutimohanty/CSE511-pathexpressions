#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

//Parse and save it in an array
void parser(char* str, char* save[],const char* delim){

  char* pch;
  //char temp[100];
  int i=0;
  int len=0;
  printf ("111\n");
  char path_exp[strlen(str)];
  strcpy(path_exp,str);
  printf ("111\n");
  //len = strlen(str);
  printf ("%d\n",len);
  //printf("Initializing Synchronizer with path_exp %s\n", str);
  pch = strtok (path_exp,delim);
  printf ("%s\n",pch);

  while (pch != NULL)
  {
    //strcpy(temp, pch);
    save[i]= pch;
    len = strlen(save[i]) + len;
    printf ("%d\n",len);
    printf ("next char = %c\n",str[len]);
    printf ("saveop = %s\n",save[i]);
    printf ("%s\n",pch);
    printf ("=========================\n");
    i++;
    pch = strtok (NULL, delim);
  }

}


int main(){

  char* str = "path {read}+write end";

  char path_exp[strlen(str)];
  strcpy(path_exp,str);
  printf("Initializing Synchronizer with path_exp %s\n", path_exp);
  //char p = path_exp[0];
  char* pch;
  int i=0;
  int len=0;

  char* path_end_exp[3];//[100];
  char* save_op[20];//[20];

  char path1_end_exp[3][100];
  char save1_op[20][20];

  const char* delim1 = " ";
  const char* delim2 = "+;{}";
  //
  // pch = strtok (path_exp,delim1);
  // printf ("%s\n",pch);
  //
  // while (pch != NULL)
  // {
  //   strcpy(path_end_exp[i], pch);
  //   //len = strlen(path_end_exp[i]) + len;
  //   printf ("%d\n",len);
  //   printf ("next char = %c\n",str[len]);
  //   //printf ("saveop = %s\n",path_end_exp[i]);
  //   printf ("%s\n",pch);
  //   printf ("=========================\n");
  //   i++;
  //   pch = strtok (NULL, delim1);
  // }
  parser(str,path_end_exp,delim1);
  // printf("%s =====\n", path_end_exp[0]);
  printf("%s =====\n", path_end_exp[1]);
  char* str2;
  strcmp(str2,path_end_exp[1]);

  printf("%s =====\n", str2);
  parser(path1_end_exp[1],save_op,delim2);
  //for(int j =0;j<)
  return 0;
}
