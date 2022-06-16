#include "index.h"
#include <stdio.h>

int main(int argc, char **argv){

	//dois parâmetros de entrada
	//1: quantidade de dados
	//2: quantidade de threads
	int id[MAXTHR];
	if(argc!=3){
		printf("digite o tamanho do vetor e o número de threads\n");
		exit(1);
	}

	//VERIFICAÇÃO DE PARAMETROS
	int n_rects = atoi(*(argv+1));
	MAXTHR = atoi(*(argv+2));
	if(n_rects<=0){
			printf("input invalido\n" );
			exit(1);
	}
	total_nodes = malloc(sizeof(int));
	linear_total = 0;
	*total_nodes = 0;
	//VARIÁVEL DOS DADOS A SEREM INSERIDOS
	struct Rect *rects = malloc(sizeof(struct Rect) * n_rects);
	//n_search_linear~5 a quantidade de dados encontrados nas pesquisas
	//t_search_linear~5 o tempo gasto por cada pesquisa
	int i, n_search_linear = 0,n_search_parallel = 0;
	double t_search_linear,t_search_parallel;
	//node raiz
	//struct Node* root = RTreeNewIndex();
	struct Node* root;
	//GERADOR DOS VETORES
	for(i=0;i<n_rects;i++){
		int j;
		for(j=0;j<NUMSIDES;j++){
			if(j<NUMDIMS)
			rects[i].boundary[j]=i;
			else
			rects[i].boundary[j]=i+1;
		}
	}

	//vetor usado para parar os threads
	kill = malloc(sizeof(struct Node));
	kill->level=-1;

	//inserindo os dados na árvore a partir da raiz
	//for(i=0;i<n_rects;i++)
	//	RTreeInsertRect(&rects[i],i+1,&root,0);
	int maxLevel = findMaxLevel(n_rects);
	printf("maxlevel: %d\n",maxLevel);
	root = ArtificialRTree(0,n_rects,maxLevel);
	
	//rect a ser pesquisado(tamanho total, engloba todos os dados inseridos)
	rect_search = malloc(sizeof(struct Rect));
	for(i=0;i<NUMSIDES/2;i++)
		rect_search->boundary[i]=0;

	for(i=NUMSIDES/2;i<NUMSIDES;i++)
		rect_search->boundary[i]=n_rects+1;

	//RTreePrintNode(root,0);
	//RTREESEARCH
	uint64_t diff;
	struct timespec tick, tock;
	clock_gettime(CLOCK_REALTIME, &tick);
	//for(i=0;i < rpt;i++)
		n_search_linear += RTreeSearch(root, rect_search);
	clock_gettime(CLOCK_REALTIME, &tock);
	diff = NANOS * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec;
	t_search_linear =  (double)diff/NANOS;
	//RTREESEARCHPARALELA
	pthread_t thre[MAXTHR];
	QueueInit();
	queue->active= MAXTHR;
	queue->sum=0;
	queue->size=0;
	queue->maxSize=0;
	queue->count=0;
	uint64_t diff5;
	struct timespec tick5, tock5;
	Data data_parallel[MAXTHR];
	n_search_parallel=0;
	queue->active= 0;
	queue->inactive =MAXTHR;
	double t_wait[MAXTHR];
	double t_sch[MAXTHR];
	for(i=0;i<MAXTHR;i++){
		data_parallel[i].node=NULL;
		data_parallel[i].hits = &n_search_parallel;
		data_parallel[i].time_wait = &t_wait[i];
		data_parallel[i].time_search = &t_sch[i];
	}
	int j=0;
	//for(j=0;j<rpt;j++)
		for(i=0;i<root->count;i++)
			QueuePush(root->branch[i].child);
	clock_gettime(CLOCK_REALTIME, &tick5);
	for(i=0;i<MAXTHR;i++)
		pthread_create(&thre[i],NULL,InitThread,&data_parallel[i]);
	for(i=0;i<MAXTHR;i++)
		pthread_join(thre[i],NULL);
	clock_gettime(CLOCK_REALTIME, &tock5);
	diff5 = NANOS * (tock5.tv_sec - tick5.tv_sec) + tock5.tv_nsec - tick5.tv_nsec;
	t_search_parallel =  (double)diff5/NANOS;

	//prints para ver a quantidade de dados encontrada e o tempo de cada pesquisa
	printf("RTREESEARCH: %d | RTREESEARCHparalela: %d\n",n_search_linear,n_search_parallel);

	//print que imprime apenas o tempo
	printf("%.6lf\t%.6lf\n", t_search_linear,t_search_parallel );
	
	for(i=0;i<MAXTHR;i++)
		printf("tempo wait %d: %.6lf\ntempo search %d: %.6lf\n",i, *(data_parallel[i].time_wait),i, *(data_parallel[i].time_search));
	double soma=0;
	for(i=0;i<MAXTHR;i++)
		soma += (*(data_parallel[i].time_wait)) + (*(data_parallel[i].time_search));
	printf("%.6lf\n", soma);
	printf("media: %ld\tcontador: %ld\tmax: %ld\n", queue->sum/queue->count,queue->count,queue->maxSize);
	printf("nodes visitados| RTREESEARCH: %d | RTREESEARCHPARALELA:  %d\n",linear_total,*total_nodes);//no linear aparece um a mais porque o root não é contado no paralelo
	printf("tamanho da fila: %ld\n", queue->size);
	printf("MAXCARD: %d\n",MAXCARD);
	QueueKill();
	return 0;
}
