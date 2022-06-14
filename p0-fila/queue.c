#include <stdio.h>
#include "queue.h"

// retorna 1 se estiverem na mesma fila
// retorna 0 se estiverem em fila diferente
// assume que os dois existem
int same_queue(queue_t* queuea, queue_t* queueb){
  if (queuea == queueb) return 1;

  queue_t* aux = queuea->next;
  while (aux != queuea){
    if (aux == queueb)
      return 1;
    aux = aux->next;
  }
  return 0;
}

int queue_size (queue_t *queue){
  if (!queue) return 0;

  int size = 0;
  queue_t* atual = queue->next;
  size++;

  while (atual && atual != queue){
    size++;
    atual = atual->next;
  }
  return size;
}

void queue_print (char *name, queue_t *queue, void print_elem (void*) ){
  printf("%s: [", name);
  if (!queue) {printf("]\n"); return;}

  queue_t* atual = queue->next;
  print_elem(queue);
  while(atual != queue){
    printf(" ");
    print_elem(atual);
    atual = atual->next;
  }
  printf("]\n");
}

int queue_append (queue_t **queue, queue_t *elem){
  // casos de erro
  if (!queue){
    fprintf(stderr, "ERRO QUEUE.C: Tentativa de inserir elemento em fila que não existe\n");
    return -1;
  }
  if (!elem){
    fprintf(stderr, "ERRO QUEUE.C: Tentativa de inserir elemento que não existe em fila\n");
    return -2;
  }
  if ( (elem->next != NULL) || (elem->prev != NULL) ){
    fprintf(stderr, "ERRO QUEUE.C: Tentativa de inserir elemento que já está em uma fila\n");
    return -3;
  }

  // caso fila esteja vazia
  if (!(*queue)){
    elem->next = elem->prev = elem;
    *queue = elem;
    return 0;
  }

  queue_t* last = (*queue)->prev;

  last->next = elem;
  elem->prev = last;

  elem->next = *queue;
  (*queue)->prev = elem;

  return 0;
}

int queue_remove (queue_t **queue, queue_t *elem){
  // casos de erro
  if (!queue){
    fprintf(stderr, "ERRO QUEUE.C: Tentativa de remover elemento de uma fila que não existe\n");
    return -1;
  }
  if (!(*queue)){
    fprintf(stderr, "ERRO QUEUE.C: Tentativa de remover elemento de uma fila vazia\n");
    return -2;
  }
  if (!elem){
    fprintf(stderr, "ERRO QUEUE.C: Tentativa de remover elemento que não existe\n");
    return -3;
  }
  if (!(same_queue(*queue, elem))){
    fprintf(stderr, "ERRO QUEUE.C: Tentativa de remover elemento de uma fila que não o contém\n");
    return -4;
  }

  // se a fila so tem um elemento
  if (elem->next == elem){
    elem->next = elem->prev = NULL;
    *queue = NULL;
    return 0;
  }

  // se for o primeiro elemento transformar no último para facilitar remoção
  if (*queue == elem) *queue = (*queue)->next;

  queue_t* next = elem->next;
  queue_t* prev = elem->prev;

  prev->next = next;
  next->prev = prev;
  elem->next = elem->prev = NULL;

  return 0;
}
