// GRR20196049 Iago Mello Floriano

#include <stdio.h>
#include <stdlib.h>
#include "ppos.h"
#include "ppos_data.h"

#define STACKSIZE 64*1024	// tamanho de pilha das threads

task_t mainTask, *currTask;
int lastID;

void ppos_init(){
  setvbuf(stdout, 0, _IONBF, 0) ;
  lastID = 0;
  mainTask.id = 0;
  currTask = &mainTask;

  getcontext(&(mainTask.context));
}

int task_create(task_t *task, void (*start_func)(void *), void *arg){
  getcontext(&(task->context));

  char *stack = malloc(STACKSIZE);
  if(stack){
    task->context.uc_stack.ss_sp = stack ;
    task->context.uc_stack.ss_size = STACKSIZE ;
    task->context.uc_stack.ss_flags = 0 ;
    task->context.uc_link = 0 ;
  }else{
    perror("Erro na criação da pilha: ") ;
    exit(1) ;
  }

  makecontext(&(task->context), (void*)(*start_func), 1, arg) ;

  task->id = ++lastID;
  return task->id;
}

void task_exit(int exit_code){
  task_switch(&mainTask);
}

int task_switch(task_t *task){
  task_t *lastTask = currTask;
  currTask = task;
  swapcontext(&(lastTask->context), &(currTask->context));
  return 0;
}

int task_id(){
  return currTask->id;
}
