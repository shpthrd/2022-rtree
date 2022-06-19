#include "index.h"
#include <stdio.h>

int main(int argc, char **argv){

	//dois parâmetros de entrada
	//1: quantidade de dados
	//2: quantidade de threads
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
	
	struct Rect *rects = malloc(sizeof(struct Rect) * n_rects);
	double t_searchsum_linear,t_searchsum_parallel;
	int i;
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

	//node raiz
	struct Node* root = RTreeNewIndex();

	int n;
	int n_init=0;
	if(n_rects <=1000){
		n = n_rects;
	}
	else{
		n = 1000;
	}
	for(n;n<=n_rects;n=n*10){

	
	//inserindo os dados na árvore a partir da raiz
	for(i=n_init;i<n;i++)
		RTreeInsertRect(&rects[i],i+1,&root,0);

	//rect a ser pesquisado(tamanho total, engloba todos os dados inseridos)
	rect_search = malloc(sizeof(struct Rect));
	for(i=0;i<NUMSIDES/2;i++)
		rect_search->boundary[i]=0;

	for(i=NUMSIDES/2;i<NUMSIDES;i++)
		rect_search->boundary[i]=n_rects+1;
	
	double t_searchthread_parallel[MAXTHR-1];
	double t_searchthread_linear = 0;
	int THRDCOUNT;
	for(THRDCOUNT = 2;THRDCOUNT <=MAXTHR;THRDCOUNT++){

		t_searchsum_linear=0;
		t_searchsum_parallel=0;
		double t_worksum[THRDCOUNT];
		double t_waitsum[THRDCOUNT];
		memset( t_worksum, 0, THRDCOUNT*sizeof(double) );
		memset( t_waitsum, 0, THRDCOUNT*sizeof(double) );
		int n_search_linear = 0,n_search_parallel = 0;
		//REPETIÇÕES
		int k=0;
		for(k=0;k<rpt;k++){
	
			linear_total = 0;
			
			//n_search_linear~parallel a quantidade de dados encontrados nas pesquisas
			//t_search_linear~parallel o tempo gasto por cada pesquisa
			//int n_search_linear = 0,n_search_parallel = 0;
			double t_search_linear = 0,t_search_parallel = 0;
			

			//RTreePrintNode(root,0);
			//RTREESEARCH
			uint64_t diff;
			struct timespec tick, tock;
			clock_gettime(CLOCK_REALTIME, &tick);
				n_search_linear = RTreeSearch(root, rect_search);
			clock_gettime(CLOCK_REALTIME, &tock);
			diff = NANOS * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec;
			t_search_linear =  (double)diff/NANOS;
			t_searchsum_linear += t_search_linear;


		//int THRDCOUNT;
		//for(THRDCOUNT = 2;THRDCOUNT <=MAXTHR;THRDCOUNT++){
			//printf("a\n");
			total_nodes = malloc(sizeof(int));
			*total_nodes = 0;
			//vetor usado para parar os threads
			kill = malloc(sizeof(struct Node));
			kill->level=-1;
			//RTREESEARCH PARALELA
			pthread_t thre[THRDCOUNT];
			QueueInit();
			queue->active= THRDCOUNT;
			queue->sum=0;
			queue->size=0;
			queue->maxSize=0;
			queue->count=0;
			uint64_t diff2;
			struct timespec tick2, tock2;
			Data data_parallel[THRDCOUNT];
			n_search_parallel=0;
			queue->active= 0;
			queue->inactive =THRDCOUNT;
			double t_wait[THRDCOUNT];
			memset( t_wait, 0, THRDCOUNT*sizeof(double) );
			double t_sch[THRDCOUNT];
			memset( t_sch, 0, THRDCOUNT*sizeof(double) );
			for(i=0;i<THRDCOUNT;i++){
				data_parallel[i].node=NULL;
				data_parallel[i].hits = &n_search_parallel;
				data_parallel[i].time_wait = &t_wait[i];
				data_parallel[i].time_search = &t_sch[i];
			}
			
			for(i=0;i<root->count;i++)
				QueuePush(root->branch[i].child);
			clock_gettime(CLOCK_REALTIME, &tick2);
			for(i=0;i<THRDCOUNT;i++)
				pthread_create(&thre[i],NULL,InitThread,&data_parallel[i]);
			for(i=0;i<THRDCOUNT;i++)
				pthread_join(thre[i],NULL);
			clock_gettime(CLOCK_REALTIME, &tock2);
			diff2 = NANOS * (tock2.tv_sec - tick2.tv_sec) + tock2.tv_nsec - tick2.tv_nsec;
			t_search_parallel =  (double)diff2/NANOS;
			t_searchsum_parallel += t_search_parallel;

			//prints para ver a quantidade de dados encontrada e o tempo de cada pesquisa
			//printf("RTREESEARCH: %d \t RTREESEARCHparalela: %d\n",n_search_linear,n_search_parallel);

			//print que imprime apenas o tempo
			//printf("RTREESEARCH: %.6lf \t RTREESEARCHparalela: %.6lf\n", t_search_linear,t_search_parallel );
			//printf("THREAD \t\tWORK \t\tWAIT\n");
			for(i=0;i<THRDCOUNT;i++){
				//printf("THREAD %d: \t%.6lf\t%.6lf\n", i, *(data_parallel[i].time_search), *(data_parallel[i].time_wait));
				t_worksum[i] += *(data_parallel[i].time_search);
				t_waitsum[i] += *(data_parallel[i].time_wait);
			
			}
			
			/*
			double soma=0;
			for(i=0;i<THRDCOUNT;i++)
				soma += (*(data_parallel[i].time_wait)) + (*(data_parallel[i].time_search));
			printf("soma dos tempos wait e search: %.6lf\n", soma);
			*/

			//printf("QUEUE: media: %ld\tcontador: %ld\tmax: %ld\n", queue->sum/queue->count,queue->count,queue->maxSize);
			//printf("nodes visitados| RTREESEARCH: %d | RTREESEARCHPARALELA:  %d\n",linear_total,*total_nodes);//no linear aparece um a mais porque o root não é contado no paralelo
			//printf("tamanho da fila: %ld\n", queue->size);
			QueueKill();
			free(total_nodes);
			free(kill);
		}
		printf("RTREESEARCH: %d \t RTREESEARCHparalela: %d\n",n_search_linear,n_search_parallel);

		printf("NUMERO DE THREADS: %d\n",THRDCOUNT);
		printf("THREAD \t\tWORK \t\tWAIT\n");
		for(i=0;i<THRDCOUNT;i++){
			printf("THREAD %d: \t%.6lf\t%.6lf\n", i, t_worksum[i]/rpt,t_waitsum[i]/rpt);
		}
		
		t_searchthread_parallel[THRDCOUNT-2] = t_searchsum_parallel/rpt;
		t_searchthread_linear += t_searchsum_linear/rpt;
		printf("rpt: %d\nlinear avg: %.6lf\nparallel avg: %.6lf\n",rpt,t_searchsum_linear/rpt,t_searchsum_parallel/rpt);
		printf("\n");
	}
	t_searchthread_linear = t_searchthread_linear/(MAXTHR-1);
	printf("avg by threads in linear: %.6lf\n",t_searchthread_linear);
	for(i=0;i<MAXTHR-1;i++){
		printf("avg with %d threads: ",i+2);
		if(t_searchthread_parallel[i]<=t_searchthread_linear){
			printf("\033[0;32m");
		}
		else{
			printf("\033[0;31m");
		}
		printf("%.6lf\n",t_searchthread_parallel[i]);
		printf("\033[0;37m");
	}
	n_init = n;
	}
	//printf("\033[1;31m");
	printf("MAXCARD: %d\n",MAXCARD);
	
	return 0;
}
