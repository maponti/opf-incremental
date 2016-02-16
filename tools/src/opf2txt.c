#include <stdio.h>
#include <stdlib.h>
//#include "OPF.h"

int main(int argc, char **argv)
{

	if (argc != 3) 
	{
		fprintf(stderr,"\nusage: opf2txt <binary_descriptor_file_name> <txt file name>\n");
		exit(-1);
	}

	printf("\nPrograma para converter arquivo binario (formato OPF) \nem arquivo texto");

	FILE *fpIn = NULL,*fpOut = NULL;
	int n, ndata, nclasses, i,j, id,label,pred;
	float aux;

	fpIn = fopen(argv[1],"rb");
	fpOut = fopen(argv[2],"w");

	/*lendo numero de objetos*/
	fread(&n,sizeof(int),1,fpIn);
	fprintf(fpOut,"%d\t",n); printf("\nnobjects: %d", n);

	/*lendo numero de classes*/
	fread(&nclasses,sizeof(int),1,fpIn);
	fprintf(fpOut,"%d\t",nclasses); printf("\nnclasses: %d", nclasses); 

	/*lendo tamanho vetor de caracteristicas*/
	fread(&ndata,sizeof(int),1,fpIn);
	fprintf(fpOut,"%d\n",ndata); printf("\nndata: %d\n\n", ndata);
	
	/*lendo dados*/
	for(i = 0; i < n; i++)	{
		fread(&id,sizeof(int),1,fpIn);
		fprintf(fpOut,"%d\t",i+1);	
		
		fread(&label,sizeof(int),1,fpIn);
		fprintf(fpOut,"%d\t",label);
		
		for(j = 0; j < ndata; j++) {
		  fread(&aux,sizeof(float),1,fpIn);
		  fprintf(fpOut,"%.6f\t",aux);
		}
		fprintf(fpOut,"\n");
	}


	fclose(fpIn);
	fclose(fpOut);

	return 0;
}
