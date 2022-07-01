#ifndef _INDEX_
#define _INDEX_


#define NDEBUG

#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <inttypes.h>
#include <string.h>
typedef float RectReal;


/*-----------------------------------------------------------------------------
| Global definitions.
-----------------------------------------------------------------------------*/

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

//PGSIZE+NUMDIMS=MAXCARD -> 64+2=2 128+2=5 128+3=3 192+2=7* 192+3=5* 256+2=10 256+3=7
#define PGSIZE	128
#define NUMDIMS	3	/* number of dimensions */

#define NUMSIDES (2*NUMDIMS)
#define ENTRY int
#define NANOS ((unsigned long)1000000000)
#define rpt 35
int MAXTHR;
int threads;
int total_threads;
double* time_push;
int* total_nodes;
int linear_total;
//variaveis usadas nos pthreads
pthread_cond_t pop_cond, push_cond;
pthread_mutex_t queue_mutex;
//fila global
struct Queue* queue;
//rect pesquisado global
struct Rect* rect_search;
//instrução para o pthread morrer ao ler
struct Node* kill;

struct Rect
{
	RectReal boundary[NUMSIDES]; /* xmin,ymin,...,xmax,ymax,... */
};

struct Node;

struct Branch
{
	struct Rect rect;
	struct Node *child;
};

/* max branching factor of a node */
#define MAXCARD (int)((PGSIZE-(2*sizeof(int))) / sizeof(struct Branch))

struct Node
{
	int count;
	int level; /* 0 is leaf, others positive */
	struct Branch branch[MAXCARD];
};

struct ListNode
{
	struct ListNode *next;
	struct Node *node;
};

//struct para a versão paralela da pesquisa
typedef struct data{
	struct Node** node;
	int* hits;
	double* time_wait;
	double* time_search;
} Data;

//structs para o uso da fila
struct Atom;

struct Queue{
	struct Atom* next;
	struct Atom* last;
	volatile int active;
	volatile int inactive;
  long size;
  long maxSize;
  long sum;
  long count;
};

struct Atom{
	struct Node* node;
	struct Atom* next;
};
struct timespec longer[6];
struct timespec getMaior(struct timespec longer[]);


void* InitThread(void* arg);
//int RTreeSearch(struct Node*, struct Rect*, SearchHitCallback, void*);
int RTreeSearch(struct Node*, struct Rect*);//, SearchHitCallback, void*);
void* RTreeSearchParalela(void* data);
int RTreeInsertRect(struct Rect*, int, struct Node**, int depth);
int RTreeDeleteRect(struct Rect*, int, struct Node**);
struct Node * RTreeNewIndex();
struct Node * RTreeNewNode();
void RTreeInitNode(struct Node*);
void RTreeFreeNode(struct Node *);
void RTreePrintNode(struct Node *, int);
void RTreeTabIn(int);
struct Rect RTreeNodeCover(struct Node *);
void RTreeInitRect(struct Rect*);
struct Rect RTreeNullRect();
RectReal RTreeRectArea(struct Rect*);
RectReal RTreeRectSphericalVolume(struct Rect *R);
RectReal RTreeRectVolume(struct Rect *R);
struct Rect RTreeCombineRect(struct Rect*, struct Rect*);
int RTreeOverlap(struct Rect*, struct Rect*);
void RTreePrintRect(struct Rect*, int);
int RTreeAddBranch(struct Branch *, struct Node *, struct Node **);
int RTreePickBranch(struct Rect *, struct Node *);
void RTreeDisconnectBranch(struct Node *, int);
void RTreeSplitNode(struct Node*, struct Branch*, struct Node**);
struct Node * ArtificialRTree(int start,int end,int level); 

int RTreeSetNodeMax(int);
int RTreeSetLeafMax(int);
int RTreeGetNodeMax();
int RTreeGetLeafMax();



void QueuePush(struct Node* node);
struct Node* QueuePop();
void QueueInit();
void QueueKill();


#endif /* _INDEX_ */
