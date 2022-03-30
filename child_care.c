/* Specification for the child-care problem
 * Imagine you are running a child-care centre. State law stipulates
 * that every child-care centre must be manned by at least one
 * caregiver, before children can be dropped off. Caregivers arrive at the
 * child care centre at any time. Children are allowed to be dropped
 * off if there exists at least one caregiver. Children can leave anytime.
 * The last caregiver is allowed to leave only if there are no more children.
 *
 * Operations: CaregiverArrive, CaregiverLeave, ChildArrive, ChildLeave
 * IMPLEMENT:
 * - Provide the path expressions in PATH_EXP[]
 * - Operations CaregiverArrive, CaregiverLeave, ChildArrive, ChildLeave
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>

#include "child_care.h"

// The synchro library header
#include "pe_sync.h"

const char CHILD_ARRIVE[] = "ChildArrive", CHILD_LEAVE[] = "ChildLeave",
            CAREGIVER_ARRIVE[] = "CaregiverArrive", CAREGIVER_LEAVE[] = "CaregiverLeave",
            FIRSTCAREGIVER_ARRIVE[] = "fcg_a", LASTCAREGIVER_LEAVE[] = "lcg_l",
            LOCK_C[]= "lock_c", LOCK_CG[]= "lock_cg";

//const char cg_a,cg_l,c_a,c_l,fcg_a,lcg_l,lock_c,lock_cg;
// TODO: Write the Path Expression solving the Child-care problem
//const char PATH_EXP[] = "path fcg_a;{ChildArrive;ChildLeave};lcg_l end path {CaregiverArrive;CaregiverLeave} end path lock_c end path lock_cg end";
const char PATH_EXP[] = "path fcg_a;{ChildArrive;ChildLeave} end path {CaregiverArrive;CaregiverLeave} end path lock_c end path lock_cg end";
// Base Case:   {ACg}; {ACh} + {VCg; ECg} + {VCh; ECh}
// Bonus Case:  {ACg}; {VCg; ACh} + {VCg; ECg} + {VCh; ECh}

int c =0;
int cg = 0;

pthread_mutex_t lock_last_cg = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t sig_last_cg = PTHREAD_COND_INITIALIZER;

void set_global_clock()
{
  clock_gettime(CLOCK_MONOTONIC, &global_clock);
}

void* _handle_thread(void *arg)
{
   _thread_info_t *myinfo = (_thread_info_t *)arg;

   sleep(myinfo->start_time);        // Wait till thread-start-time
  clock_gettime(CLOCK_MONOTONIC, &myinfo->arrival_time);

  if(strcmp(myinfo->op_name, CAREGIVER_ARRIVE) == 0) {
    caregiver_arrive(myinfo);
  } else if(strcmp(myinfo->op_name, CAREGIVER_LEAVE) == 0) {
    caregiver_leave(myinfo);
  } else if(strcmp(myinfo->op_name, CHILD_ARRIVE) == 0) {
    child_arrive(myinfo);
  } else if(strcmp(myinfo->op_name, CHILD_LEAVE) == 0) {
    child_leave(myinfo);
  }

  // Stats for verification
  printf("%s Thread %d Entry %lld.%09ld Start %lld.%09ld End %lld.%09ld\n",
      myinfo->op_name, myinfo->tid,
      (long long)myinfo->arrival_time.tv_sec-global_clock.tv_sec, myinfo->arrival_time.tv_nsec-global_clock.tv_nsec,
      (long long)myinfo->cs_enter_time.tv_sec-global_clock.tv_sec, myinfo->cs_enter_time.tv_nsec-global_clock.tv_nsec,
      (long long)myinfo->cs_exit_time.tv_sec-global_clock.tv_sec, myinfo->cs_exit_time.tv_nsec-global_clock.tv_nsec
      );
  return NULL;
}

void caregiver_arrive(_thread_info_t *myinfo)
{
  ENTER_OPERATION(CAREGIVER_ARRIVE);
  clock_gettime(CLOCK_MONOTONIC, &myinfo->cs_enter_time);
  ENTER_OPERATION(LOCK_CG);
  cg++;
  if(cg == 1)
  {
    ENTER_OPERATION(FIRSTCAREGIVER_ARRIVE);
    EXIT_OPERATION(LOCK_CG);
    EXIT_OPERATION(CAREGIVER_ARRIVE);
    EXIT_OPERATION(FIRSTCAREGIVER_ARRIVE);
    sleep(myinfo->exec_time);
    clock_gettime(CLOCK_MONOTONIC, &myinfo->cs_exit_time);
    pthread_cond_signal(&sig_last_cg);
  }

  else
  {
    sleep(myinfo->exec_time);
    clock_gettime(CLOCK_MONOTONIC, &myinfo->cs_exit_time);
    EXIT_OPERATION(LOCK_CG);
    EXIT_OPERATION(CAREGIVER_ARRIVE);
  }

}

void caregiver_leave(_thread_info_t *myinfo)
{
  ENTER_OPERATION(CAREGIVER_LEAVE);
  ENTER_OPERATION(LOCK_CG);
  //cg--;
  if(cg == 1 )
  {
    ENTER_OPERATION(LOCK_C);
    //printf("DEBUG -1 FIXME acq last cg lock, child %d cg %d\n", c, cg);
    if( c != 0 )
    {
      //printf("DEBUG 0 FIXME acq last cg lock, child %d cg %d\n", c, cg);
      EXIT_OPERATION(LOCK_C);
      //ENTER_OPERATION(LASTCAREGIVER_LEAVE);
      EXIT_OPERATION(LOCK_CG);

      pthread_mutex_lock(&lock_last_cg);
      //printf("DEBUG 1 FIXME acq last cg lock, child %d cg %d\n", c, cg);
      pthread_cond_wait(&sig_last_cg, &lock_last_cg);
      //printf("DEBUG 2 FIXME wait last cg lock, child %d cg %d\n", c, cg);
      pthread_mutex_unlock(&lock_last_cg);
      //printf("DEBUG 3 FIXME leave last cg lock, child %d cg %d\n", c, cg);
      
      ENTER_OPERATION(LOCK_CG);
      cg--;
      EXIT_OPERATION(LOCK_CG);

      clock_gettime(CLOCK_MONOTONIC, &myinfo->cs_enter_time);
      EXIT_OPERATION(CAREGIVER_LEAVE);
      sleep(myinfo->exec_time);
      clock_gettime(CLOCK_MONOTONIC, &myinfo->cs_exit_time);
      //EXIT_OPERATION(LASTCAREGIVER_LEAVE);
    }
    else
    {
      EXIT_OPERATION(LOCK_C);

      clock_gettime(CLOCK_MONOTONIC, &myinfo->cs_enter_time);
      cg--;

      EXIT_OPERATION(LOCK_CG);
      EXIT_OPERATION(CAREGIVER_LEAVE);
      sleep(myinfo->exec_time);
      clock_gettime(CLOCK_MONOTONIC, &myinfo->cs_exit_time);
    }

  }
  else
  {
    clock_gettime(CLOCK_MONOTONIC, &myinfo->cs_enter_time);
    cg--;
    EXIT_OPERATION(LOCK_CG);
    EXIT_OPERATION(CAREGIVER_LEAVE);
    sleep(myinfo->exec_time);
    clock_gettime(CLOCK_MONOTONIC, &myinfo->cs_exit_time);

  }


}

void child_arrive(_thread_info_t *myinfo)
{
  clock_gettime(CLOCK_MONOTONIC, &myinfo->cs_enter_time);
  ENTER_OPERATION(CHILD_ARRIVE);
  ENTER_OPERATION(LOCK_C);
  c++;
  EXIT_OPERATION(LOCK_C);
  EXIT_OPERATION(CHILD_ARRIVE);
  sleep(myinfo->exec_time);
  clock_gettime(CLOCK_MONOTONIC, &myinfo->cs_exit_time);


}

void child_leave(_thread_info_t *myinfo)
{
  clock_gettime(CLOCK_MONOTONIC, &myinfo->cs_enter_time);
  ENTER_OPERATION(CHILD_LEAVE);
  ENTER_OPERATION(LOCK_C);
  c--;
  //printf("DEBUG: FIXME Child Leaving %d\n", c);
  if (c == 0)
  {
      pthread_cond_signal(&sig_last_cg);
      //printf("DEBUG: chl0 FIXME Child Leaving %d\n", c);
  }
  EXIT_OPERATION(LOCK_C);
  //printf("DEBUG: chl1 FIXME Child Leaving %d\n", c);
  EXIT_OPERATION(CHILD_LEAVE);
  //printf("DEBUG: chl2 FIXME Child Leaving %d\n", c);
  sleep(myinfo->exec_time);
  clock_gettime(CLOCK_MONOTONIC, &myinfo->cs_exit_time);
  //printf("DEBUG: chl3 FIXME Child Leaving %d\n", c);

  ENTER_OPERATION(LOCK_C);
  if (c == 0)
  {
  //printf("DEBUG: chl4 FIXME Child Leaving %d\n", c);
  EXIT_OPERATION(LOCK_C);
  ENTER_OPERATION(LOCK_CG);
  if (cg !=0){
  //printf("DEBUG: chl5 FIXME Child Leaving %d\n", c);
      ENTER_OPERATION(FIRSTCAREGIVER_ARRIVE);
      EXIT_OPERATION(FIRSTCAREGIVER_ARRIVE);
  }
  EXIT_OPERATION(LOCK_CG);
  //printf("DEBUG: chl6 FIXME Child Leaving %d\n", c);
  }
  else
  {
      EXIT_OPERATION(LOCK_C);
      //printf("DEBUG: chl3 FIXME Child Leaving %d\n", c);
  }
}

