// GRR20196049 Iago Mello Floriano
//
// Testa o uso de semaforos com o uso de produtores consumidores

#include <stdio.h>
#include <stdlib.h>
#include "ppos.h"
#include "queue.h"
#define SLEEPTIME 1000

task_t p1, p2, p3, c1, c2;
semaphore_t s_buffer, s_item, s_vaga;

typedef struct fila{
  struct fila *prev;
  struct fila *next;
  int valor;
} fila_t;

// camp a ser protegido pelo buffer
fila_t *buffer;

void produtor(void *arg){

  while(1){
    task_sleep(SLEEPTIME);

    //produz item
    int temp = random() % 100;
    fila_t *elemtemp = malloc(sizeof(fila_t));
    elemtemp->prev = elemtemp->next = NULL;
    elemtemp->valor = temp;

    sem_down(&s_vaga);
    sem_down(&s_buffer);
    //insere item no buffer;
    queue_append((queue_t**)&buffer, (queue_t*)elemtemp);
    printf("%s%d (%d)\n", (char*)arg, elemtemp->valor, queue_size((queue_t*)buffer));

    sem_up(&s_buffer);
    sem_up(&s_item);
  }

  task_exit(task_id());
}

void consumidor(void *arg){
  while(1){
    sem_down(&s_item);
    sem_down(&s_buffer);
    // retira item do buffer
    fila_t *elemtemp = buffer;
    queue_remove( (queue_t**)&buffer, (queue_t*)buffer);

    sem_up(&s_buffer);
    sem_up(&s_vaga);
    // print item
    printf("%s%2d (%d)\n", (char*)arg, elemtemp->valor, queue_size((queue_t*)buffer));
    free(elemtemp);
    task_sleep(SLEEPTIME);
  }
  task_exit(task_id());
}

int main(int argc, char *argv[]){
  printf("Começo main\n");

  ppos_init();
  sem_create(&s_buffer, 0);
  sem_create(&s_item  , 0);
  sem_create(&s_vaga  , 5);

  //cria o buffer
  buffer = NULL;

  task_create(&p1, produtor, "p1 produziu ");
  task_create(&p2, produtor, "p2 produziu ");
  task_create(&p3, produtor, "p3 produziu ");

  task_create(&c1, consumidor, "                  c1 consumiu ");
  task_create(&c2, consumidor, "                  c2 consumiu ");

  sem_up(&s_buffer);

  task_join(&p1);
  task_join(&p2);
  task_join(&p3);
  task_join(&c1);
  task_join(&c2);

  sem_destroy(&s_buffer);
  sem_destroy(&s_item  );
  sem_destroy(&s_vaga  );

  task_exit(0);
  return 0;
}
