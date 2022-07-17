// GRR20196049 Iago Mello Floriano

#include <stdio.h>
#include <stdlib.h>
#include "ppos.h"
#include "ppos_data.h"
#include "queue.h"

#define STACKSIZE 64*1024	// tamanho de pilha das threads

// print de debug
#ifdef DEBUG
#define debug_print(...) do{ printf( __VA_ARGS__ ); } while(0)
#else
#define debug_print(...) do{ } while (0)
#endif

task_t mainTask, *currTask, *taskQ, dispatcher;
int lastID, userTasks;

void print_elem(void *ptr){
  task_t *task = ptr;

  if(!task)
    return;

  printf("%d",task->id);
}

// retorna qual task será a proxima a ser executada
// criterio FCFS
task_t *scheduler(){
  return taskQ;
}

void dispatcher_body(){
  while(userTasks > 0){
    #ifdef DEBUG
    queue_print("PPOS: taskQ ", (queue_t*)taskQ, print_elem);
    #endif
    task_t *proxima = scheduler();

    if (proxima){
      task_switch(proxima);

      switch(proxima->status){
        // Colocar de volta na fila caso esteja pronta
        case READY:
          taskQ = taskQ->next;
          break;
        // Remover da fila caso esteja encerrada
        case TERMINATED:
          queue_remove( (queue_t**)&taskQ, (queue_t*)proxima);
          break;
      }

    }
  }

  task_switch(&mainTask);
}

void ppos_init(){
  setvbuf(stdout, 0, _IONBF, 0) ;
  lastID = 0;
  mainTask.id = 0;
  mainTask.status = READY;
  currTask = &mainTask;

  getcontext(&(mainTask.context));
  mainTask.status = READY;
  task_create(&dispatcher, dispatcher_body, NULL);
  queue_remove( (queue_t**)&taskQ, (queue_t*)&dispatcher);
  userTasks = 0;
  debug_print("PPOS: Iniciado o Sistema\n");
}

int task_create(task_t *task, void (*start_func)(void *), void *arg){
  task->status = NEW;
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

  task->status = READY;
  task->id = ++lastID;
  queue_append( (queue_t**)&taskQ, (queue_t*)task);
  userTasks++;
  debug_print("PPOS: Criado task com id %d, atualmente %d userTasks\n", lastID-1,userTasks);
  return task->id;
}

void task_exit(int exit_code){
  currTask->status = TERMINATED;
  userTasks--;
  debug_print("PPOS: Encerrando tarefa com id %d\n", currTask->id);
  task_switch(&dispatcher);
}

int task_switch(task_t *task){
  task_t *lastTask = currTask;
  currTask = task;
  lastTask->status = lastTask->status == TERMINATED ? TERMINATED : READY;
  currTask->status = RUNNING;
  debug_print("PPOS: Trocando de tarefas %d->%d\n", lastTask->id, currTask->id);
  swapcontext(&(lastTask->context), &(currTask->context));
  return 0;
}

int task_id(){
  return currTask->id;
}

void task_yield (){
  task_switch(&dispatcher);
}
