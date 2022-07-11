// GRR20196049 Iago Mello Floriano

#include <stdio.h>
#include <stdlib.h>
#include "ppos.h"
#include "ppos_data.h"

#define STACKSIZE 64*1024	// tamanho de pilha das threads

task_t mainContext, currContext;
int lastID;

void ppos_init(){
  printf("-----começo ppos_init-----\n");
  setvbuf(stdout, 0, _IONBF, 0) ;
  lastID = 0;
  mainContext.id = 0;
  currContext = mainContext;

  getcontext(&(mainContext.context));
  printf("feito getcontext na main\n");
  char *stack = malloc(STACKSIZE);
  if(stack){
    mainContext.context.uc_stack.ss_sp = stack ;
    mainContext.context.uc_stack.ss_size = STACKSIZE ;
    mainContext.context.uc_stack.ss_flags = 0 ;
    mainContext.context.uc_link = 0 ;
  }else{
    perror("Erro na criação da pilha: ") ;
    exit(1) ;
  }
  printf("-----fim ppos_init-----\n");
}

int task_create(task_t *task, void (*start_func)(void *), void *arg){
  printf("-----começo task_create-----\n");
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

  makecontext(&(task->context), start_func, 1, arg) ;

  task->id = ++lastID;
  printf("-----fim task_create-----\n");
  return task->id;
}

void task_exit(int exit_code){
  printf("-----começo task_exit-----\n");
  task_switch(&mainContext);
  printf("-----fim task_exit-----\n");
}

int task_switch(task_t *task){
  printf("-----começo task_switch-----\n");
  swapcontext(&(currContext.context), &(task->context));
  printf("-----fim task_switch-----\n");
  return 0;
}

int task_id(){
  return currContext.id;
}
