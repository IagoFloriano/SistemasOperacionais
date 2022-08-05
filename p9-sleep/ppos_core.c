// GRR20196049 Iago Mello Floriano

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
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
unsigned int globalSysTime;

struct sigaction tickaction;
struct itimerval ticktimer;
int tickcount;

// retorna tempo do sistema
unsigned int systime(){
  return globalSysTime;
}

// função a rodar todo tick do sistema
void tickfunc(int signum){
  globalSysTime++;
  if (currTask->preemptable){
    if (--tickcount < 0)
      task_yield();
  }
}

// imprime o id de um elemento da fila
void print_elem(void *ptr){
  task_t *task = ptr;

  if(!task)
    return;

  printf("%d",task->id);
}

// retorna qual task será a proxima a ser executada
task_t *scheduler(){
  task_t *oldest = taskQ;
  task_t *temp = taskQ->next;
  oldest->dinprio--;
  while(temp != taskQ){
    if(--(temp->dinprio) < oldest->dinprio)
      oldest = temp;
    temp = temp->next;
  }
  return oldest;
}

// cuida de lançar tasks
void dispatcher_body(){
  while(userTasks > 0){
#ifdef DEBUG
    queue_print("PPOS: taskQ ", (queue_t*)taskQ, print_elem);
#endif
    task_t *proxima = scheduler();

    if (proxima){
      tickcount = 20;
      task_switch(proxima);
      switch(proxima->status){
        // Colocar de volta na fila caso esteja pronta
        case READY:
          // taskQ = taskQ->next;
          break;
          // Remover da fila caso esteja encerrada
        case TERMINATED:
          queue_remove( (queue_t**)&taskQ, (queue_t*)proxima);
          break;
      }

    }
  }

  unsigned int now = systime();
  dispatcher.totalProcTime += now - dispatcher.lastProcTime;
  dispatcher.endSysTime = now;
  dispatcher.totalSysTime = now - dispatcher.startSysTime;
  printf("Task %6d exit: execution time %6d ms, processor time %6d ms, %6d activations\n",
      dispatcher.id,dispatcher.totalSysTime,dispatcher.totalProcTime,dispatcher.activations);
  // task_switch(&mainTask);
}

// faz inicialização do sistema
void ppos_init(){
  debug_print("PPOS: Iniciando sistema\n");
  setvbuf(stdout, 0, _IONBF, 0) ;
  lastID = 0;
  globalSysTime = 0;

  debug_print("PPOS: Criando mainTask...\n");
  mainTask.id = 0;
  mainTask.status = READY;
  currTask = &mainTask;
  getcontext(&(mainTask.context));
  mainTask.status = READY;
  mainTask.preemptable = 1;
  mainTask.startSysTime  = globalSysTime;
  mainTask.endSysTime    = 0;
  mainTask.activations   = 0;
  mainTask.lastProcTime  = 0;
  mainTask.totalProcTime = 0;
  queue_append( (queue_t**)&taskQ, (queue_t*)&mainTask);
  debug_print("PPOS: Criado mainTask\n");

  debug_print("PPOS: Criando dispatcher...\n");
  task_create(&dispatcher, dispatcher_body, NULL);
  queue_remove( (queue_t**)&taskQ, (queue_t*)&dispatcher);
  dispatcher.preemptable = 0;
  userTasks = 1;
  debug_print("PPOS: Criado dispatcher.\n");

  debug_print("PPOS: Criando timer...\n");
  tickaction.sa_handler = tickfunc;
  sigemptyset(&tickaction.sa_mask);
  tickaction.sa_flags = 0;
  if (sigaction (SIGALRM, &tickaction, 0) < 0){
    perror ("Erro em sigaction: ") ;
    exit (1) ;
  }
  // ajustar valores do timer
  ticktimer.it_value.tv_usec    = 1000;
  ticktimer.it_value.tv_sec     = 0;
  ticktimer.it_interval.tv_usec = 1000;
  ticktimer.it_interval.tv_sec  = 0;
  // armar timer
  if (setitimer (ITIMER_REAL, &ticktimer, 0) < 0){
    perror ("Erro em setitimer: ") ;
    exit (1) ;
  }
  debug_print("PPOS: Timer criado.\n");

  debug_print("PPOS: Iniciado o Sistema\n");
  debug_print("PPOS: Lançando dispatcher\n");
#ifdef DEBUG
  queue_print("PPOS: taskQ ", (queue_t*)taskQ, print_elem);
#endif
  task_yield();
}

// Cria uma task retorna seu id em caso de sucesso
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
  task->statprio = task->dinprio = 0;
  task->preemptable = 1;
  task->startSysTime  = systime();
  task->endSysTime    = 0;
  task->activations   = 0;
  task->lastProcTime  = 0;
  task->totalProcTime = 0;
  task->joined = NULL;
  queue_append( (queue_t**)&taskQ, (queue_t*)task);
  userTasks++;
  debug_print("PPOS: Criado task com id %d, atualmente %d userTasks\n", lastID,userTasks);
  return task->id;
}

// Termina task atual em execução
void task_exit(int exit_code){
  debug_print("PPOS: Encerrando tarefa com id %d\n", currTask->id);
  currTask->status = TERMINATED;
  userTasks--;

  // Retorna o exit code pra todas as task que deram join
  if(currTask->joined){
    while(currTask->joined){
      currTask->joined->rcvexit = exit_code;
      task_resume(currTask->joined, &(currTask->joined));
    }
  }

  unsigned int now = systime();
  currTask->totalProcTime += now - currTask->lastProcTime;
  currTask->endSysTime = now;
  currTask->totalSysTime = now - currTask->startSysTime;
  printf("Task %6d exit: execution time %6d ms, processor time %6d ms, %6d activations\n",
      currTask->id,currTask->totalSysTime,currTask->totalProcTime,currTask->activations);
  task_switch(&dispatcher);
}

// Troca de contexto para o de task
int task_switch(task_t *task){
  task_t *lastTask = currTask;
  currTask = task;
  lastTask->status = lastTask->status == TERMINATED ? TERMINATED : READY;
  currTask->status = RUNNING;
  currTask->dinprio = currTask->statprio;
  lastTask->dinprio = lastTask->statprio;

  unsigned int now = systime();
  if(lastTask->endSysTime == 0)
    lastTask->totalProcTime += now - lastTask->lastProcTime;

  currTask->activations++;
  currTask->lastProcTime = now;
  debug_print("PPOS: Trocando de tarefas %d->%d\n", lastTask->id, currTask->id);
  swapcontext(&(lastTask->context), &(currTask->context));
  return 0;
}

// retorna o id da task atual
int task_id(){
  return currTask->id;
}

// suspende a tarefa atual na fila "queue"
void task_suspend (task_t **queue){
  int rmstatus = queue_remove((queue_t**)(&taskQ), (queue_t*)currTask);
  if(rmstatus < 0 && rmstatus > -4){
    fprintf(stderr, "PPOS: WARNING: tentativa invalida de suspender tarefa\n");
  }
  currTask->status = SUSPENDED;
  /*int addstatus =*/ queue_append((queue_t**)queue, (queue_t*)currTask);
  task_yield();
}

// acorda a tarefa indicada, que está suspensa na fila indicada
void task_resume (task_t *task, task_t **queue){
  int rmstatus = queue_remove((queue_t**)queue, (queue_t*)task);
  if(rmstatus < 0 && rmstatus > -4){
    fprintf(stderr, "PPOS: WARNING: tentativa invalida de suspender tarefa\n");
  }
  task->status = READY;
  /*int addstatus =*/ queue_append((queue_t**)(&taskQ), (queue_t*)task);
}

// devolve a cpu para o sistema
void task_yield (){
  task_switch(&dispatcher);
}

// troca prioridade de task
// se task for nulo troca prioridade da task atual
void task_setprio (task_t *task, int prio){
  if (!task)
    task = currTask;

  if(prio < -20)
    prio = -20;
  if(prio > 20)
    prio = 20;

  task->statprio = task->dinprio = prio;
}

// retorna prioridade de task
// se task for nulo retorna prioridade da task atual
int task_getprio (task_t *task){
  if (!task)
    task = currTask;

  return task->statprio;
}

// a tarefa corrente aguarda o encerramento de outra task
int task_join (task_t *task){
  if(currTask->id == task->id) return -1;
  if(task->status == TERMINATED) return -1;
  task_suspend(&(task->joined));
  return currTask->rcvexit;
}
