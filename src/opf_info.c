#include "OPF.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv){
	fflush(stdout);
	fprintf(stdout, "\nProgram that gives information about the OPF file\n");
	fprintf(stdout, "\nIf you have any problem, please contact: ");
	fprintf(stdout, "\n- alexandre.falcao@gmail.com");
	fprintf(stdout, "\n- papa.joaopaulo@gmail.com\n");
	fprintf(stdout, "\nLibOPF version 2.0 (2009)\n");
	fprintf(stdout, "\n"); fflush(stdout);

	if(argc != 2) {
		fprintf(stderr, "\nusage opf_train <P1>");
		fprintf(stderr, "\nP1: OPF file");
		exit(-1);
	}

	Subgraph *g = NULL;
	FILE *fp = NULL;
	int ndata, nfeats, nlabels, id, label, i;
	char msg[128];
	
	if((fp = fopen(argv[1], "rb")) == NULL){
		sprintf(msg, "%s%s", "Unable to open file ", argv[1]);
		Error(msg,"opf_info");
	}

	g = ReadSubgraph(argv[1]);
	
	fread(&ndata, sizeof(int), 1, fp);
	fread(&nlabels, sizeof(int), 1, fp);
	fread(&nfeats, sizeof(int), 1, fp);
	int *countl = (int *) calloc(nlabels+1, sizeof(int));
	for(i = 0; i < ndata; i++)	{
		fread(&id, sizeof(int), 1, fp);	//printf("%d ", id);
		fread(&label, sizeof(int), 1, fp);//	printf("%d\n", label); 
		countl[label]++;
		fseek(fp, nfeats*sizeof(float), SEEK_CUR);
	}
	
	fprintf(stdout, "\nInformations about %s file\n --------------------------------", argv[1]);
	fprintf(stdout,"\nData size: %d",ndata);
	fprintf(stdout,"\nFeatures size: %d",nfeats);
	fprintf(stdout,"\nLabels number: %d",nlabels);
	//fprintf(stdout,"\nPrototype number: %d", prototypeCount(g));
	printf("\n\tPercentage of each class:\n");
	for (i = 1; i <= nlabels; i++) {
	    float perc_i = ((float)countl[i]/(float)ndata)*100;
	    printf("\t\tclass %d: %.2f%% (%d total)\n", i, perc_i, countl[i]);
	}
	fprintf(stdout, "--------------------------------\n");

	DestroySubgraph(&g);
	fclose(fp);

	return 0;
}
