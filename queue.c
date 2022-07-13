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

void QueuePush2(struct Node* node){
  if(queue == NULL) return;
  printf("A()%d count: %d level: %d| %p\n",pthread_self(),node->count,node->level,node);
  if(node->count >1){
    printf("count>1\n");
    //pthread_mutex_lock(&queue_mutex);
    struct Atom *atom[node->count];
    int i;
    atom[0] = malloc(sizeof(struct Atom));
    atom[0]->node = (node->branch[0]).child;
    for(i=1;i<node->count;i++){
      printf("f\n");
      atom[i] = malloc(sizeof(struct Atom));
      atom[i]->node = (node->branch[i]).child;
      atom[i-1]->next=atom[i];
      printf("ff\n");
    }
    printf("i = %d\n",i);
    atom[i-1]->next=NULL;
    printf("B()%d count: %d level: %d| %p\n\n",pthread_self(),node->count,node->level,node);
    pthread_mutex_lock(&queue_mutex);
    if(queue->next==NULL){
      queue->next=atom[0];
    }
    else{
      queue->last->next = atom[0];
    }
    queue->last = atom[i-1];
    queue->size = queue->size + node->count;
    if(queue->size > queue->maxSize)queue->maxSize=queue->size;
    //queue->sum+=queue->size;
    queue->count = queue->count + node->count;
    printf("queue->count : %d queue->size : %d ============\n",queue->count,queue->size);
    pthread_cond_signal(&pop_cond);  
    pthread_mutex_unlock(&queue_mutex);
  }
  else{
    printf("entrou no else\n");
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
    printf("queue->count : %d ============\n",queue->count);
    pthread_cond_signal(&pop_cond);  
    pthread_mutex_unlock(&queue_mutex);
  }
  
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
