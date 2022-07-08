#include "index.h"
#include <stdio.h>

int main(int argc, char **argv){
	uint64_t diff_p;
	struct timespec tick_p, tock_p;
	clock_gettime(CLOCK_REALTIME, &tick_p);
	
	//dois parâmetros de entrada
	//1: quantidade de dados
	//2: quantidade de threads
	//3: sufixo
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
	int THRDINT = 0,temp_thrd = MAXTHR;
	while(temp_thrd != 1){//faz a conta de quantas repetições por numero de threads (2 4 8 16 32 ...)
		temp_thrd = temp_thrd / 2;
		THRDINT++;
	}
	printf("THRDINT: %d\n",THRDINT);
	char* sufix;//sufixo para identificar o arquivo
	if(argc>=4){
			sufix = argv[3];
	}
	if(sufix != NULL)
		printf("%s\n",sufix);
	else
		sufix = "no-name";
	struct Rect *rects = malloc(sizeof(struct Rect) * n_rects);
	int i;
	//GERADOR DOS RECTS
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
	//vars para calculo do tempo do algoritmo total e tempo de inserção
	uint64_t diff_ins;
	double t_insertion = 0;
	double t_insertionsum = 0;
	struct timespec tick_ins, tock_ins;
	double t_searchsum_linear,t_searchsum_parallel;

	int n;
	int n_init=0;
	if(n_rects <=1000){
		n = n_rects;
	}
	else{
		n = 1000;
	}
	//inicio do loop para inserção, toda a pesquisa se repete para cara nova ordem de tamanho de pontos inseridos
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
		
		double t_searchthread_parallel[THRDINT];
		double parallel_desvpad[THRDINT];
		double t_searchthread_linear;
		double linear_desvpad;
		
		int k=0;
		int n_search_linear = 0;
		double time_per_repeat_linear[rpt];
		memset(time_per_repeat_linear,0,rpt*sizeof(double));
		//WARM UP CACHE
		for(k=0;k<rpt;k++){
			linear_total = 0;
			double t_search_linear = 0;
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

		}
		t_searchsum_linear = 0;
		for(k=0;k<rpt;k++){
			linear_total = 0;
			double t_search_linear = 0;
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

		}
		t_searchsum_linear = 0;

		//LINEAR RUN
		for(k=0;k<rpt;k++){
			linear_total = 0;
			double t_search_linear = 0;
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

		}
		t_searchthread_linear = t_searchsum_linear/rpt;
		linear_desvpad = 0;
		for(k=0;k<rpt;k++){
			linear_desvpad += pow(time_per_repeat_linear[k]-t_searchthread_linear,2); 
		}
		linear_desvpad = sqrt(linear_desvpad/rpt);



		int THRDCOUNT=THRDINT;
		//REPETIÇÕES
		double time_per_repeat_parallel[rpt];
		int count_queue = 0,max_queue =0;
		int n_search_parallel = 0;
		int THRDCOUNT2;
		first_kill = 1;
		//WARMUP
		for(THRDCOUNT=THRDINT;THRDCOUNT<=THRDINT;THRDCOUNT++){
			THRDCOUNT2 = (int)pow(2,THRDINT);
			t_searchsum_linear=0;
			t_searchsum_parallel=0;
			memset(time_per_repeat_parallel,0,rpt*sizeof(double));
			n_search_parallel = 0;
			count_queue = 0;
			max_queue =0;
			for(k=0;k<rpt;k++){
				double t_search_parallel = 0;
				//vetor usado para parar os threads
				kill = malloc(sizeof(struct Node));
				kill->level=-1;
				//RTREESEARCH PARALELA
				pthread_t thre[THRDCOUNT2];
				QueueInit();
				queue->active= THRDCOUNT2;
				queue->sum=0;
				queue->size=0;
				queue->maxSize=0;
				queue->count=0;
				uint64_t diff2;
				struct timespec tick2, tock2;
				Data data_parallel[THRDCOUNT2];
				n_search_parallel=0;
				queue->active= 0;
				queue->inactive =THRDCOUNT2;
				double t_waitw[THRDCOUNT2];
				memset( t_waitw, 0, THRDCOUNT2*sizeof(double) );
				double t_schw[THRDCOUNT2];
				memset( t_schw, 0, THRDCOUNT2*sizeof(double) );
				for(i=0;i<THRDCOUNT2;i++){
					data_parallel[i].node=NULL;
					data_parallel[i].hits = &n_search_parallel;
					data_parallel[i].time_wait = &t_waitw[i];
					data_parallel[i].time_search = &t_schw[i];
				}
				
				for(i=0;i<root->count;i++)
					QueuePush(root->branch[i].child);
				clock_gettime(CLOCK_REALTIME, &tick2);
				for(i=0;i<THRDCOUNT2;i++)
					pthread_create(&thre[i],NULL,InitThread,&data_parallel[i]);
				for(i=0;i<THRDCOUNT2;i++)
					pthread_join(thre[i],NULL);
				clock_gettime(CLOCK_REALTIME, &tock2);
				diff2 = NANOS * (tock2.tv_sec - tick2.tv_sec) + tock2.tv_nsec - tick2.tv_nsec;
				//ESTE É A MEDIDA DO TEMPO EM CADA REPETIÇÃO EM CADA THREAD A CADA DELTA QTD
				t_search_parallel =  (double)diff2/NANOS;
				t_searchsum_parallel += t_search_parallel;
				time_per_repeat_parallel[k] = t_search_parallel;
				count_queue += queue->count;
				max_queue += queue->maxSize;
				QueueKill();
				free(kill);
			}
			t_searchthread_parallel[THRDCOUNT-1] = t_searchsum_parallel/rpt;
			parallel_desvpad[THRDCOUNT-1] = 0;
			
			for(k=0;k<rpt;k++){
				parallel_desvpad[THRDCOUNT-1] += pow(time_per_repeat_parallel[k]-t_searchthread_parallel[THRDCOUNT-1],2);
			}
			parallel_desvpad[THRDCOUNT-1] = sqrt(parallel_desvpad[THRDCOUNT-1]/rpt);
			count_queue = count_queue/rpt;
			max_queue = max_queue/rpt;
		}
		//================================================
		//REAL
		for(THRDCOUNT = 1;THRDCOUNT <=THRDINT;THRDCOUNT++){
			THRDCOUNT2 = (int)pow(2,THRDCOUNT);
			printf("THRDCOUNT: %d\nTHRDCOUNT2: %d\n",THRDCOUNT,THRDCOUNT2);
			t_searchsum_linear=0;
			t_searchsum_parallel=0;
			double t_worksum[THRDCOUNT2];
			double t_waitsum[THRDCOUNT2];
			memset( t_worksum, 0, THRDCOUNT2*sizeof(double) );
			memset( t_waitsum, 0, THRDCOUNT2*sizeof(double) );
			memset(time_per_repeat_parallel,0,rpt*sizeof(double));
			n_search_parallel = 0;
			int n_search_parallel_thread[THRDCOUNT2];

			count_queue = 0;
			max_queue =0;
			killtime_sum = 0;
			for(k=0;k<rpt;k++){
				double t_search_parallel = 0;
				//vetor usado para parar os threads
				kill = malloc(sizeof(struct Node));
				kill->level=-1;
				//RTREESEARCH PARALELA
				pthread_t thre[THRDCOUNT2];
				QueueInit();
				queue->active= THRDCOUNT2;
				queue->sum=0;
				queue->size=0;
				queue->maxSize=0;
				queue->count=0;
				uint64_t diff2;
				struct timespec tick2, tock2;
				Data data_parallel[THRDCOUNT2];
				//n_search_parallel=0;
				
				memset( n_search_parallel_thread, 0, THRDCOUNT2*sizeof(int));
				queue->active= 0;
				queue->inactive =THRDCOUNT2;
				double t_wait[THRDCOUNT2];
				memset( t_wait, 0, THRDCOUNT2*sizeof(double) );
				double t_sch[THRDCOUNT2];
				memset( t_sch, 0, THRDCOUNT2*sizeof(double) );
				for(i=0;i<THRDCOUNT2;i++){
					data_parallel[i].node=NULL;
					//data_parallel[i].hits = &n_search_parallel;
					data_parallel[i].hits = &n_search_parallel_thread[i];
					data_parallel[i].time_wait = &t_wait[i];
					data_parallel[i].time_search = &t_sch[i];
				}
				first_kill = 0;
				for(i=0;i<root->count;i++)
					QueuePush(root->branch[i].child);
				clock_gettime(CLOCK_REALTIME, &tick2);
				for(i=0;i<THRDCOUNT2;i++)
					pthread_create(&thre[i],NULL,InitThread,&data_parallel[i]);
				for(i=0;i<THRDCOUNT2;i++)
					pthread_join(thre[i],NULL);
				clock_gettime(CLOCK_REALTIME, &tock2);
				diff2 = NANOS * (tock2.tv_sec - tick2.tv_sec) + tock2.tv_nsec - tick2.tv_nsec;
				diff_kill = NANOS * (tock2.tv_sec - tick_kill.tv_sec) + tock2.tv_nsec - tick_kill.tv_nsec;
				double killtime = (double) diff_kill/NANOS;
				killtime_sum += killtime;
				//ESTE É A MEDIDA DO TEMPO EM CADA REPETIÇÃO EM CADA THREAD A CADA DELTA QTD
				t_search_parallel =  ((double)diff2/NANOS)-killtime;

				t_searchsum_parallel += t_search_parallel;
				time_per_repeat_parallel[k] = t_search_parallel;

				for(i=0;i<THRDCOUNT2;i++){
					t_worksum[i] += *(data_parallel[i].time_search);
					t_waitsum[i] += *(data_parallel[i].time_wait);
				
				}
				count_queue += queue->count;
				max_queue += queue->maxSize;
				QueueKill();
				free(kill);
			}
			t_searchthread_parallel[THRDCOUNT-1] = t_searchsum_parallel/rpt;
			parallel_desvpad[THRDCOUNT-1] = 0;
			
			for(k=0;k<rpt;k++){
				parallel_desvpad[THRDCOUNT-1] += pow(time_per_repeat_parallel[k]-t_searchthread_parallel[THRDCOUNT-1],2);
			}
			parallel_desvpad[THRDCOUNT-1] = sqrt(parallel_desvpad[THRDCOUNT-1]/rpt);
			for(i=0;i<THRDCOUNT2;i++){
				n_search_parallel +=n_search_parallel_thread[i];
			}
			printf("RTREESEARCH: %d \t RTREESEARCHparalela: %d\n",n_search_linear,n_search_parallel);
			printf("NUMERO DE THREADS: %d\n",THRDCOUNT2);
			printf("THREAD \t\tWORK \t\tWAIT\n");
			for(i=0;i<THRDCOUNT2;i++){
				printf("THREAD %d: \t%.6lf\t%.6lf\n", i, t_worksum[i]/rpt,t_waitsum[i]/rpt);
			}
			count_queue = count_queue/rpt;
			max_queue = max_queue/rpt;
			printf("queue:count %d\tmax %d\n",count_queue,max_queue);
			printf("#rpt: %d\n#linear avg:\tdesvpad:\n",rpt);
			printf("%.6lf\t%.6lf\n",t_searchthread_linear,linear_desvpad);
			printf("parallel avg:\tdesvpad:\n");
			printf("%.6lf\t%.6lf\n",t_searchthread_parallel[THRDCOUNT-1],parallel_desvpad[THRDCOUNT-1]);
			printf("time to kill: %.6f\n",killtime_sum/rpt);
			printf("\n");
		}
		//printf("total de nodes: %d\n",linear_total);
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
		printf("output: %s\n",file1Name);
		char output[1024] = "";


		printf("\"LINEAR-%d DATA %d THREADS\"\n",n,MAXTHR);
		strcat(output,"\"LINEAR-");
		sprintf(temp,"%d",n);
		strcat(output,temp);
		strcat(output," DATA ");
		sprintf(temp,"%d",MAXTHR);
		strcat(output,temp);
		strcat(output," THREADS\"\n");

		//for(i=0;i<MAXTHR-1;i++){
		for(i=0;i<THRDINT;i++){
			printf("%d\t%.6lf\t%.6lf\n",(int)pow(2,i+1),t_searchthread_linear,linear_desvpad);
			sprintf(temp,"%d",(int)pow(2,i+1));
			strcat(output,temp);
			strcat(output,"\t");
			sprintf(temp,"%.6lf",t_searchthread_linear);
			strcat(output,temp);
			strcat(output,"\t");
			sprintf(temp,"%.6lf",linear_desvpad);
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

		//for(i=0;i<MAXTHR-1;i++){
		for(i=0;i<THRDINT;i++){	
			printf("%d\t",(int)pow(2,i+1));
			printf("%.6lf\t%.6lf\n",t_searchthread_parallel[i],parallel_desvpad[i]);
			sprintf(temp,"%d",(int)pow(2,i+1));
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
		n_init = n;//precisa sim, para nao inserir dados repetidos
	}
	//RTreePrintNode(root,0);
	RTreeFreeNode(root);
	//free rects
	printf("MAXCARD: %d\n",MAXCARD);
	printf("total time of insertion: %.6lf\n",t_insertionsum);
	
	clock_gettime(CLOCK_REALTIME, &tock_p);
	diff_p = NANOS * (tock_p.tv_sec - tick_p.tv_sec) + tock_p.tv_nsec - tick_p.tv_nsec;
	printf("total time: %.6lf\n",(double)diff_p/NANOS);
	//printf("node: %d, branch: %d, rect: %d, float: %d\n",sizeof(struct Node),sizeof(struct Branch), sizeof(struct Rect),sizeof(float));
	
	return 0;
}
