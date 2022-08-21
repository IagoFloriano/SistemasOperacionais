// GRR20196049 Iago Mello Floriano

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
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

// filas de mensagens

/*
   typedef struct mqbuffer_t
   {
   struct mqbuffer_t *prev, *next;
   void *data;
   } mqbuffer_t;

   typedef struct
   {
   semaphore_t vagas, dados;
   int data_size;
   mqbuffer_t *buffer;
   } mqueue_t ;
*/

// cria uma fila para até max mensagens de size bytes cada
int mqueue_create (mqueue_t *queue, int max, int size){
  if (sem_create(&(queue->vagas)    , max)) return -1;
  if (sem_create(&(queue->dados)    , 0  )) return -1;
  if (sem_create(&(queue->buffersem), 1  )) return -1;
  queue->data_size = size;
  queue->buffer = NULL;
  queue->alive = 1;
  return 0;
}

// envia uma mensagem para a fila
int mqueue_send (mqueue_t *queue, void *msg){
  // Cria memória pra mensagem a ser enviada
  if(!queue || !queue->alive) return -1;
  mqbuffer_t *new = malloc(sizeof(mqbuffer_t));
  if(!new) return -1;

  new->data = malloc(queue->data_size);
  if(!(new->data)) {free(new); return -1;}

  new->next = new->prev = NULL;
  bcopy(msg, new->data, queue->data_size);

  // bloquear semaforos
  sem_down(&(queue->vagas    ));
  sem_down(&(queue->buffersem));
  //inserir item no buffer
  if(!queue || !queue->alive) return -1;
  queue_append( (queue_t**) &(queue->buffer), (queue_t*) new);

  // liberar semaforos
  sem_up(&(queue->buffersem));
  sem_up(&(queue->dados    ));
  return 0;
}

// recebe uma mensagem da fila
int mqueue_recv (mqueue_t *queue, void *msg){
  if(!queue || !queue->alive) return -1;
  // bloquear semaforos
  sem_down(&(queue->dados    ));
  sem_down(&(queue->buffersem));
  // retirar item do buffer
  if(!queue || !queue->alive) return -1;
  mqbuffer_t *temp = queue->buffer;
  queue_remove((queue_t**)&(queue->buffer), (queue_t*)queue->buffer);

  // liberar semaforos
  sem_up(&(queue->buffersem));
  sem_up(&(queue->vagas    ));

  // salva item
  bcopy(temp->data, msg, queue->data_size);
  free(temp);
  return 0;
}

// destroi a fila, liberando as tarefas bloqueadas
int mqueue_destroy (mqueue_t *queue){
  if(!queue || !queue->alive) return -1;
  sem_destroy(&queue->vagas    );
  sem_destroy(&queue->dados    );
  sem_destroy(&queue->buffersem);
  queue->alive = 0;
  return 0;
}

// informa o número de mensagens atualmente na fila
int mqueue_msgs (mqueue_t *queue){
  if(!queue || !queue->alive) return -1;
  return queue_size((queue_t*)queue->buffer);
}
