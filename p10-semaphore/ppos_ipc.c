// GRR20196049 Iago Mello Floriano

#include <stdio.h>
#include "ppos.h"
#include "ppos_data.h"
#include "queue.h"

// cria um semáforo com valor inicial "value"
int sem_create (semaphore_t *s, int value){
  if(!s) return -1;

  s->lock = 0;
  s->counter = value;
  s->fila = NULL;
  s->vivo = 1;
  return 0;
}

// requisita o semáforo
int sem_down (semaphore_t *s){
  if(!s || !s->vivo) return -1;

  while(__sync_fetch_and_or(&(s->lock), 1))
    task_yield();

  if(--(s->counter) < 0){
    s->lock = 0;
    task_suspend(&(s->fila));
  } else
    s->lock = 0;

  if(!s->vivo) return -1;

  return 0;
}

// libera o semáforo
int sem_up (semaphore_t *s){
  if(!s || !s->vivo) return -1;

  while(__sync_fetch_and_or(&(s->lock), 1))
    task_yield();

  if(++(s->counter) <= 0){
    s->lock = 0;
    task_resume(s->fila, &(s->fila));
  } else
    s->lock = 0;

  return 0;
}

// destroi o semáforo, liberando as tarefas bloqueadas
int sem_destroy (semaphore_t *s){
  if(!s || !s->vivo) return -1;

  while(__sync_fetch_and_or(&(s->lock), 1))
    task_yield();

  s->vivo = 0;
  while(s->fila)
    task_resume(s->fila, &(s->fila));

  return 0;
}
