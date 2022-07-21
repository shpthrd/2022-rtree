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
  if(queue->size > queue->maxSize)queue->maxSize=queue->size;
  //queue->sum+=queue->size;
  queue->count++;
  pthread_cond_signal(&pop_cond);  
  pthread_mutex_unlock(&queue_mutex);
}

void QueuePush3(struct Node* node,double* time_push_wait){
  if(queue == NULL) return;
  struct Atom *atom = malloc(sizeof(struct Atom));
  atom->node = node;
  atom->next=NULL;
  uint64_t diff_push_w;
	struct timespec tick_push_w, tock_push_w;
  clock_gettime(CLOCK_REALTIME, &tick_push_w);
  pthread_mutex_lock(&queue_mutex);
  clock_gettime(CLOCK_REALTIME, &tock_push_w);
  diff_push_w = NANOS * (tock_push_w.tv_sec - tick_push_w.tv_sec) + tock_push_w.tv_nsec - tick_push_w.tv_nsec;
  *time_push_wait += (double)diff_push_w/NANOS;
  if(queue->next==NULL){
    queue->next=atom;
  }
  else{
    queue->last->next = atom;
  }
  queue->last = atom;
  queue->size++;
  if(queue->size > queue->maxSize)queue->maxSize=queue->size;
  //queue->sum+=queue->size;
  queue->count++;
  pthread_cond_signal(&pop_cond);  
  pthread_mutex_unlock(&queue_mutex);
}



struct Node* QueuePop(){
  if(queue == NULL) return NULL;
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

struct Node* QueuePop2(double* time_pop){
  if(queue == NULL) return NULL;
  uint64_t diff_pop;
	struct timespec tick_pop, tock_pop;
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
  //printf("pop 1\n");
  clock_gettime(CLOCK_REALTIME, &tick_pop);
  (queue->inactive)--;
  struct Node* node = (queue->next->node);
  struct Atom* atom = queue->next->next;
  free(queue->next);
  queue->next=atom;
  queue->size--;
  pthread_mutex_unlock(&queue_mutex);
  clock_gettime(CLOCK_REALTIME, &tock_pop);
  diff_pop = NANOS * (tock_pop.tv_sec - tick_pop.tv_sec) + tock_pop.tv_nsec - tick_pop.tv_nsec;
  *time_pop += (double)diff_pop/NANOS;
  //printf("pop 2\n");
  return node;
}

struct Node* QueuePop3(double* time_pop,double* time_empty){
  if(queue == NULL) return NULL;
  uint64_t diff_pop, diff_empty;
	struct timespec tick_pop, tock_pop, tick_empty,tock_empty;
  pthread_mutex_lock(&queue_mutex);
  int was_empty = 0;
  clock_gettime(CLOCK_REALTIME, &tick_empty);
  while((queue)->next==NULL){
    was_empty = 1;
    
    if((queue->active)-- <0){
      pthread_mutex_unlock(&queue_mutex);
      pthread_cond_broadcast(&pop_cond);
      if(first_kill == 0){
        first_kill = 1;
        clock_gettime(CLOCK_REALTIME, &tick_kill);
      }
      clock_gettime(CLOCK_REALTIME, &tock_empty);
      diff_empty = NANOS * (tock_empty.tv_sec - tick_empty.tv_sec) + tock_empty.tv_nsec - tick_empty.tv_nsec;
      *time_empty += (double)diff_empty/NANOS;
      return kill;
    }
    //printf("%d: empty\n",pthread_self());
    pthread_cond_wait(&pop_cond,&queue_mutex);
    (queue->active)++;
  }
  clock_gettime(CLOCK_REALTIME, &tick_pop);
  if(was_empty == 1){
    diff_empty = NANOS * (tick_pop.tv_sec - tick_empty.tv_sec) + tick_pop.tv_nsec - tick_empty.tv_nsec;
    *time_empty += (double)diff_empty/NANOS;
    //printf("time empty %.6f\n",diff_empty/NANOS);
  }
    

  (queue->inactive)--;
  struct Node* node = (queue->next->node);
  struct Atom* atom = queue->next->next;
  free(queue->next);
  queue->next=atom;
  queue->size--;
  pthread_mutex_unlock(&queue_mutex);
  clock_gettime(CLOCK_REALTIME, &tock_pop);
  diff_pop = NANOS * (tock_pop.tv_sec - tick_pop.tv_sec) + tock_pop.tv_nsec - tick_pop.tv_nsec;
  *time_pop += (double)diff_pop/NANOS;
  //printf("pop 2\n");
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
