// GRR20196049 Iago Mello Floriano

#include <stdio.h>
#include "ppos.h"
#include "ppos_data.h"

#define STACKSIZE 64*1024	// tamanho de pilha das threads

task_t mainContext, currContext;
int lastID;

void ppos_init(){
  setvbuf(stdout, 0, _IONBF, 0) ;
  lastID = 0;
  contextMain.id = 0;
  currContext = mainContext;

  getcontext(mainContext.context)

  char *stack = malloc(STACKSIZE) ;
  if(stack){
    mainContext.context.uc_stack.ss_sp = stack ;
    mainContext.context.uc_stack.ss_size = STACKSIZE ;
    mainContext.context.uc_stack.ss_flags = 0 ;
    mainContext.context.uc_link = 0 ;
  }else{
    perror("Erro na criação da pilha: ") ;
    exit(1) ;
  }
}

int task_create(task_t *task, void (*start_func)(void *), void *arg){
  getcontext(&(task->context))

  char *stack = malloc(STACKSIZE) ;
  if(stack){
    task->context.uc_stack.ss_sp = stack ;
    task->context.uc_stack.ss_size = STACKSIZE ;
    task->context.uc_stack.ss_flags = 0 ;
    task->context.uc_link = 0 ;
  }else{
    perror("Erro na criação da pilha: ") ;
    exit(1) ;
  }

  makecontext(task, start_func, 1, arg) ;

  task->id = ++lastID;
}

void task_exit(int exit_code){
  task_switch(&contextMain);
}

int task_switch(task_t *task){
  swapcontext(&(currContext.context), &(task->));
}

int task_id(){
  return currContext.id;
}
