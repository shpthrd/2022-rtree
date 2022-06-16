#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "index.h"
#include <time.h>
#include <sys/time.h>
#include <inttypes.h>
FILE *outfile;
void write(struct Node* node){
    // write struct to file 
    fwrite (node, sizeof(struct Rect), 1, outfile);
    if(fwrite != 0)  
        printf("contents to file written successfully !\n"); 
    else 
        printf("error writing file !\n");
    int i; 
    if(node->level>0)for(i=0;i<node->count;i++)write(node->branch[i].child);

    
  
}
int main (int argc, char **argv){ 
    int i;
    if(argc!=2){
        printf("digite o tamanho do vetor\n");
        exit(1);
    }
    int n_rects = atoi(*(argv+1));
    if(n_rects<=0){
        printf("input invalido\n" );
        exit(1);
    }
    struct Rect *rects = malloc(sizeof(struct Rect) * n_rects);
    struct Node* root = RTreeNewIndex();
    
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
    //inserindo os dados na árvore a partir da raiz
    for(i=0;i<n_rects;i++)
        RTreeInsertRect(&rects[i],i+1,&root,0);
    // open file for writing 
    outfile = fopen ("data", "w"); 
    if (outfile == NULL) 
    { 
        fprintf(stderr, "\nError opend file\n"); 
        exit (1); 
    } 
    RTreePrintNode(root,0);
    write(root);
    
    
    // close file 
    fclose (outfile); 
  
    return 0; 
} 