// GRR20196049 Iago Mello Floriano

#include <stdio.h>
#include "ppos.h"
#include "ppos_data.h"

task_t mainContext, currContext;
int lastID;

void ppos_init(){
  setvbuf (stdout, 0, _IONBF, 0) ;
  lastID = 0;
  contextMain.id = 0;
  currContext = contextMain
}

int task_create (task_t *task, void (*start_func)(void *), void *arg){
  makecontext(task, start_func, 1, arg);
  if(!task){
    #ifdef DEBUG
    perror("PPOS_CORE: ERRO ao criar tarefa que teria id %d\n", lastID + 1);
    #endif
    return -1;
  }
  task->id = ++lastID;
  return task->id;
}

void task_exit (int exit_code){
}

int task_switch (task_t *task){
}

int task_id (){
  return currContext.id;
}
