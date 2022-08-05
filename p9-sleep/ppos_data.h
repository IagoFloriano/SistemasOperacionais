// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 1.4 -- Janeiro de 2022
// GRR20196049 Iago Mello Floriano

// Estruturas de dados internas do sistema operacional

#ifndef __PPOS_DATA__
#define __PPOS_DATA__
#define RUNNING 0
#define READY 1
#define NEW 2
#define SUSPENDED 3
#define TERMINATED 4

#include <ucontext.h>		// biblioteca POSIX de trocas de contexto

// Estrutura que define um Task Control Block (TCB)
typedef struct task_t
{
  struct task_t *prev, *next ;		// ponteiros para usar em filas
  int id ;				// identificador da tarefa
  ucontext_t context ;			// contexto armazenado da tarefa
  short status ;			// pronta, rodando, suspensa, ...
  short preemptable ;			// pode ser preemptada?
  short statprio;
  short dinprio;
  unsigned int startSysTime  ;
  unsigned int endSysTime    ;
  unsigned int totalSysTime  ;
  unsigned int activations   ;
  unsigned int lastProcTime  ;
  unsigned int totalProcTime ;
  struct task_t *joined;
  int rcvexit;
  // ... (outros campos serão adicionados mais tarde)
} task_t ;

// estrutura que define um semáforo
typedef struct
{
  // preencher quando necessário
} semaphore_t ;

// estrutura que define um mutex
typedef struct
{
  // preencher quando necessário
} mutex_t ;

// estrutura que define uma barreira
typedef struct
{
  // preencher quando necessário
} barrier_t ;

// estrutura que define uma fila de mensagens
typedef struct
{
  // preencher quando necessário
} mqueue_t ;

#endif

