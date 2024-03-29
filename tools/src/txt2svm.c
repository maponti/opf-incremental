#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){

	if (argc != 3) 
	{
		fprintf(stderr,"\nusage txt2svm <P1> <P2>\n");
		fprintf(stderr,"\nP1: input file name in the OPF ASCII format");
		fprintf(stderr,"\nP2: output file name in the OPF binary format\n");
		exit(-1);
	}

	printf("\nProgram to convert files written in the OPF ASCII format to the SVM data format.");

	FILE *fpIn = NULL,*fpOut = NULL;
	int n, ndata, nclasses, i,j, id,label;
	float aux;
	size_t result;

	fpIn = fopen(argv[1],"r");
	fpOut = fopen(argv[2],"wb");

	/*writting the number of samples*/
	result = fscanf(fpIn,"%d",&n); printf("\nnobjects: %d",n);
	//result = fwrite(&n,sizeof(int),1,fpOut);

	/*writting the number of classes*/
	result = fscanf(fpIn,"%d",&nclasses); 	printf("\nnclasses: %d",nclasses); 
	//result = fwrite(&nclasses,sizeof(int),1,fpOut);

	/*writting the number of features*/
	result = fscanf(fpIn,"%d",&ndata); printf("\nndata: %d\n",ndata);
	//result = fwrite(&ndata,sizeof(int),1,fpOut);
	
	/*writting data*/
	for(i = 0; i < n; i++)	{
		result = fscanf(fpIn,"%d",&id);	//fwrite(&id,sizeof(int),1,fpOut);
		//printf("%d ",id);
		result = fscanf(fpIn,"%d",&label); 
		fprintf(fpOut,"%d ",label);
		//fwrite(&label,sizeof(int),1,fpOut);
		for(j = 0; j < ndata; j++){
			result = fscanf(fpIn,"%f",&aux);
			fprintf(fpOut,"%d:%.4f ",j+1,aux);
			//fwrite(&aux,sizeof(float),1,fpOut);
		}
		fprintf(fpOut,"\n");
	}


	fclose(fpIn);
	fclose(fpOut);

	return 0;
}
