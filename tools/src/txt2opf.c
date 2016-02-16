#include <stdio.h>
#include <stdlib.h>
//#include "OPF.h"

int main(int argc, char **argv){

	if (argc != 3) 
	{
		fprintf(stderr,"\nusage txt2opf <P1> <P2>\n");
		fprintf(stderr,"\nP1: input file name in the OPF ASCII format");
		fprintf(stderr,"\nP2: output file name in the OPF binary format\n");
		exit(-1);
	}

	printf("\nProgram to convert files written in the OPF ASCII format to the OPF binary format.");

	FILE *fpIn = NULL,*fpOut = NULL;
	int n, ndata, nclasses, i,j, id,label;
	float aux;

	fpIn = fopen(argv[1],"r");
	fpOut = fopen(argv[2],"wb");

	/*writting the number of samples*/
	fscanf(fpIn,"%d",&n); printf("\nnobjects: %d",n);
	fwrite(&n,sizeof(int),1,fpOut);

	/*writting the number of classes*/
	fscanf(fpIn,"%d",&nclasses); 	printf("\nnclasses: %d",nclasses); 
	fwrite(&nclasses,sizeof(int),1,fpOut);

	/*writting the number of features*/
	fscanf(fpIn,"%d",&ndata); printf("\nndata: %d",ndata);
	fwrite(&ndata,sizeof(int),1,fpOut);
	
	/*writting data*/
	for(i = 0; i < n; i++)	{
		fscanf(fpIn,"%d",&id);	fwrite(&id,sizeof(int),1,fpOut);
		fscanf(fpIn,"%d",&label); fwrite(&label,sizeof(int),1,fpOut);
		for(j = 0; j < ndata; j++){
			fscanf(fpIn,"%f",&aux);
			fwrite(&aux,sizeof(float),1,fpOut);
		}
	}


	fclose(fpIn);
	fclose(fpOut);

	return 0;
}
