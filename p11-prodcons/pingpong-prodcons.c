// GRR20196049 Iago Mello Floriano
//
// Testa o uso de semaforos com o uso de produtores consumidores

#include <stdio.h>
#include <stdlib.h>
#include "ppos.h"

int main(int argc, char *argv[]){
  printf("Começo main\n");

  ppos_init();

  task_exit(0);

  return 0;
}
