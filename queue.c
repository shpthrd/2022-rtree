#include "index.h"
#include <pthread.h>

void QueuePush(struct Node* node){
  if(queue == NULL) return;
  struct Atom *atom = malloc(sizeof(struct Atom));
  atom->node = node;
  atom->next=NULL;
  pthread_mutex_lock(&queue_mutex);
  if(queue->next==NULL){
    queue->next=atom;
  }
  else{
    queue->last->next = atom;
  }
  queue->last = atom;
  queue->size++;
  if(queue->size > queue->maxSize)queue->maxSize++;
  queue->sum+=queue->size;
  queue->count++;
  pthread_cond_signal(&pop_cond);  
  pthread_mutex_unlock(&queue_mutex);
}

struct Node* QueuePop(){
  if(queue == NULL) return;
  pthread_mutex_lock(&queue_mutex);
  while((queue)->next==NULL){
    if((queue->active)-- <0){
      pthread_mutex_unlock(&queue_mutex);
      pthread_cond_broadcast(&pop_cond);
      if(first_kill == 0){
        first_kill = 1;
        clock_gettime(CLOCK_REALTIME, &tick_kill);
      }
      return kill;
    }
    
    pthread_cond_wait(&pop_cond,&queue_mutex);
    (queue->active)++;
  }
  (queue->inactive)--;
  struct Node* node = (queue->next->node);
  struct Atom* atom = queue->next->next;
  free(queue->next);
  queue->next=atom;
  queue->size--;
  pthread_mutex_unlock(&queue_mutex);

  return node;
}



void QueueInit() {
    queue = calloc(1, sizeof(struct Queue));
    pthread_mutex_init(&queue_mutex,NULL);
    pthread_cond_init(&pop_cond,NULL);
}


void AtomKill(struct Atom** atom){
  struct Atom* aux;
  int test=0;
  if((*atom) != NULL){
    test=1;
    aux = (*atom)->next;
  }
  free (*atom);
  if(test==1)
    AtomKill(&aux);
}

void QueueKill(){
  AtomKill(&(queue->next));
  free(queue);
  queue = NULL;
  pthread_mutex_destroy(&queue_mutex);
  pthread_cond_destroy(&pop_cond);
}
