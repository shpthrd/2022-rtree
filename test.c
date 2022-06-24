#include "index.h"
#include <stdio.h>

int main(int argc, char **argv){
	uint64_t diff_p;
	struct timespec tick_p, tock_p;
	clock_gettime(CLOCK_REALTIME, &tick_p);
	
	//dois parâmetros de entrada
	//1: quantidade de dados
	//2: quantidade de threads
	//3: color printf
	if(argc<3){
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
	int printOption = 0;
	char* sufix;
	if(argc>=4){
		if(atoi(*(argv+3))==1){
			printOption = 1;
		}
		else if(atoi(*(argv+3))==2){
			printOption = 2;
		}
		else{
			sufix = argv[3];
		}
	}
	if(sufix != NULL)
		printf("%s\n",sufix);
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
	FILE * file1;
	//file1 para fazer os arquivos de delta t x n threads
	//file2 e file3 para fazer os arquivos de delta t x delta dados

	uint64_t diff_ins;
	double t_insertion = 0;
	double t_insertionsum = 0;
	struct timespec tick_ins, tock_ins;
	

	int n;
	int n_init=0;
	if(n_rects <=1000){
		n = n_rects;
	}
	else{
		n = 1000;
	}
	
	for(n;n<=n_rects;n=n*10){

	clock_gettime(CLOCK_REALTIME, &tick_ins);
	//inserindo os dados na árvore a partir da raiz
	for(i=n_init;i<n;i++)
		RTreeInsertRect(&rects[i],i+1,&root,0);

	clock_gettime(CLOCK_REALTIME, &tock_ins);
	diff_ins = NANOS * (tock_ins.tv_sec - tick_ins.tv_sec) + tock_ins.tv_nsec - tick_ins.tv_nsec;
	t_insertion =  (double)diff_ins/NANOS;
	t_insertionsum += t_insertion;
	printf("order %d points| time of insertion: %.6lf\n",n,t_insertion);
	printf("height: %d\n",root->level);
	//rect a ser pesquisado(tamanho total, engloba todos os dados inseridos)
	rect_search = malloc(sizeof(struct Rect));
	for(i=0;i<NUMSIDES/2;i++)
		rect_search->boundary[i]=0;

	for(i=NUMSIDES/2;i<NUMSIDES;i++)
		rect_search->boundary[i]=n_rects+1;
	
	double t_searchthread_parallel[MAXTHR-1];
	double t_searchthread_linear[MAXTHR-1];
	double linear_desvpad[MAXTHR-1];
	double parallel_desvpad[MAXTHR-1];
	int THRDCOUNT;
	for(THRDCOUNT = 2;THRDCOUNT <=MAXTHR;THRDCOUNT++){

		t_searchsum_linear=0;
		t_searchsum_parallel=0;
		double t_worksum[THRDCOUNT];
		double t_waitsum[THRDCOUNT];
		memset( t_worksum, 0, THRDCOUNT*sizeof(double) );
		memset( t_waitsum, 0, THRDCOUNT*sizeof(double) );
		int n_search_linear = 0,n_search_parallel = 0;

		int count_queue = 0,max_queue =0;

		//REPETIÇÕES
		int k=0;
		double time_per_repeat_parallel[rpt];
		double time_per_repeat_linear[rpt];
		memset(time_per_repeat_parallel,0,rpt*sizeof(double));
		memset(time_per_repeat_linear,0,rpt*sizeof(double));
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
			time_per_repeat_linear[k] = t_search_linear;


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
			//ESTE É A MEDIDA DO TEMPO EM CADA REPETIÇÃO EM CADA THREAD A CADA DELTA QTD
			t_search_parallel =  (double)diff2/NANOS;

			t_searchsum_parallel += t_search_parallel;
			time_per_repeat_parallel[k] = t_search_parallel;

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

			//printf("QUEUE:contador: %ld\tmax: %ld\n",queue->count,queue->maxSize);
			count_queue += queue->count;
			max_queue += queue->maxSize;

			//printf("nodes visitados| RTREESEARCH: %d | RTREESEARCHPARALELA:  %d\n",linear_total,*total_nodes);//no linear aparece um a mais porque o root não é contado no paralelo
			//printf("tamanho da fila: %ld\n", queue->size);
			QueueKill();
			free(total_nodes);
			free(kill);
		}
		//COLOCAR AQUI O CALCULO DO DESVPAD DESVIO PADRAO

		//este valor é a media do tempo de pesquisa usando o atual numero de threads THRDCOUNT
		t_searchthread_parallel[THRDCOUNT-2] = t_searchsum_parallel/rpt;
		t_searchthread_linear[THRDCOUNT-2] = t_searchsum_linear/rpt;
		parallel_desvpad[THRDCOUNT-2] = 0;
		linear_desvpad[THRDCOUNT-2] = 0;
		for(k=0;k<rpt;k++){
			parallel_desvpad[THRDCOUNT-2] += pow(time_per_repeat_parallel[k]-t_searchthread_parallel[THRDCOUNT-2],2);
			linear_desvpad[THRDCOUNT-2] += pow(time_per_repeat_linear[k]-t_searchthread_linear[THRDCOUNT-2],2); 
		}
		parallel_desvpad[THRDCOUNT-2] = sqrt(parallel_desvpad[THRDCOUNT-2]/rpt);
		linear_desvpad[THRDCOUNT-2] = sqrt(linear_desvpad[THRDCOUNT-2]/rpt);

		printf("RTREESEARCH: %d \t RTREESEARCHparalela: %d\n",n_search_linear,n_search_parallel);
		printf("NUMERO DE THREADS: %d\n",THRDCOUNT);
		printf("THREAD \t\tWORK \t\tWAIT\n");
		for(i=0;i<THRDCOUNT;i++){
			printf("THREAD %d: \t%.6lf\t%.6lf\n", i, t_worksum[i]/rpt,t_waitsum[i]/rpt);
		}
		count_queue = count_queue/rpt;
		max_queue = max_queue/rpt;
		printf("queue:count %d\tmax %d\n",count_queue,max_queue);
		printf("#rpt: %d\n#linear avg:\tdesvpad:\n",rpt);
		printf("%.6lf\t%.6lf\n",t_searchthread_linear[THRDCOUNT-2],linear_desvpad[THRDCOUNT-2]);
		printf("parallel avg:\tdesvpad:\n");
		printf("%.6lf\t%.6lf\n",t_searchthread_parallel[THRDCOUNT-2],parallel_desvpad[THRDCOUNT-2]);
		printf("\n");
	}
	char file1Name[50] = "";
	char temp[50];
	strcat(file1Name,"THRD");
	sprintf(temp,"%d",MAXTHR);
	strcat(file1Name,temp);
	strcat(file1Name,"-");
	sprintf(temp,"%d",n);
	strcat(file1Name,temp);
	strcat(file1Name,"D-");
	if(sufix != NULL)
		strcat(file1Name,sufix);
	strcat(file1Name,".plot");
	printf("teste: %s\n",file1Name);
	char output[1024] = "";


	printf("\"LINEAR-%d DATA %d THREADS\"\n",n,MAXTHR);
	strcat(output,"\"LINEAR-");
	sprintf(temp,"%d",n);
	strcat(output,temp);
	strcat(output," DATA ");
	sprintf(temp,"%d",MAXTHR);
	strcat(output,temp);
	strcat(output," THREADS\"\n");

	for(i=0;i<MAXTHR-1;i++){
		printf("%d\t%.6lf\t%.6lf\n",i+2,t_searchthread_linear[i],linear_desvpad[i]);
		sprintf(temp,"%d",i+2);
		strcat(output,temp);
		strcat(output,"\t");
		sprintf(temp,"%.6lf",t_searchthread_linear[i]);
		strcat(output,temp);
		strcat(output,"\t");
		sprintf(temp,"%.6lf",linear_desvpad[i]);
		strcat(output,temp);
		strcat(output,"\n");



	}
	printf("\n\n\"PARALLEL-%d DATA %d THREADS\"\n",n,MAXTHR);
	strcat(output,"\n\n\"PARALLEL-");
	sprintf(temp,"%d",n);
	strcat(output,temp);
	strcat(output," DATA ");
	sprintf(temp,"%d",MAXTHR);
	strcat(output,temp);
	strcat(output," THREADS\"\n");

	for(i=0;i<MAXTHR-1;i++){
		printf("%d\t",i+2);

		printf("%.6lf\t%.6lf\n",t_searchthread_parallel[i],parallel_desvpad[i]);
		sprintf(temp,"%d",i+2);
		strcat(output,temp);
		strcat(output,"\t");
		sprintf(temp,"%.6lf",t_searchthread_parallel[i]);
		strcat(output,temp);
		strcat(output,"\t");
		sprintf(temp,"%.6lf",parallel_desvpad[i]);
		strcat(output,temp);
		strcat(output,"\n");
	}
	printf("=====================\n\n");
	//printf("teste2\n%s end\n",output);
	file1 = fopen(file1Name,"w");
	fprintf(file1,output);
	fclose(file1);
	free(rect_search);
	n_init = n;//?????????
	}
	
	RTreeFreeNode(root);
	printf("MAXCARD: %d\n",MAXCARD);
	printf("total time of insertion: %.6lf\n",t_insertionsum);
	
	clock_gettime(CLOCK_REALTIME, &tock_p);
	diff_p = NANOS * (tock_p.tv_sec - tick_p.tv_sec) + tock_p.tv_nsec - tick_p.tv_nsec;
	printf("total time: %.6lf\n",(double)diff_p/NANOS);
	
	return 0;
}
