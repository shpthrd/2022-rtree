#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "index.h"
#include <time.h>
#include <sys/time.h>
#include <inttypes.h>
FILE *infile; 
struct Node* aux;
void read(struct Node* node){
	printf("aqui\n");
    RTreeInitNode(node);
	if(fread(node,sizeof(struct Node),1,infile)!=0){
	int i;
    RTreePrintRect(&(node->branch[0].rect),0);
	//if(node->level>0) for(i=0;i<node->count;i++) read(node->branch[i].child);
	//printf("%d\t %d\n",node->count,node->level);
    }
	else{
		printf("fim\n");
	}
	printf("fim da func\n");
}
int main(){
	infile = fopen ("data", "r"); 
    if (infile == NULL){ 
        fprintf(stderr, "\nError opening file\n"); 
        exit (1); 
    }
    struct Node* root;
    root = malloc(sizeof(struct Node));
    //fread(root,sizeof(struct Node),1,infile);
    //RTreePrintNode(root,0);
    printf("antes do read\n");
    read(root);
    // close file 
    fclose (infile); 
    //RTreePrintNode(root,0);
	return 0;
}