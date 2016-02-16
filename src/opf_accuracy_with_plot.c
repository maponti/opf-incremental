#include "OPF.h"

void output_plottable_file(Subgraph* g, char* fileName){
	FILE* output = fopen(fileName, "w");

	fprintf(output, "%d %d %d\n", g->nnodes, g->nlabels, g->nfeats);
	int i;
	for (i = 0; i < g->nnodes; i++){
		fprintf(output, "%d %d %d ", i, g->node[i].label, g->node[i].truelabel);
		int j;
		for (j = 0; j < g->nfeats; j++){
			fprintf(output, "%f ", g->node[i].feat[j]);
		}
		fprintf(output, "\n");
	}

	fclose(output);
}

int main(int argc, char **argv){
	fflush(stdout);
	fprintf(stdout, "\nProgram that computes OPF accuracy of a given set and outputs results in a plottable file\n");
	fprintf(stdout, "\nIf you have any problem, please contact: ");
	fprintf(stdout, "\n- riva.mateus@gmail.com");
	fprintf(stdout, "\n- moacirponti@gmail.com\n");
	fprintf(stdout, "\nLibOPFi version 1.0 (2015)\n");
	fprintf(stdout, "\n"); fflush(stdout);

	if(argc != 2){
		fprintf(stderr, "\nusage opf_accuracy_with_plot <P1>");
		fprintf(stderr, "\nP1: data set in the OPF file format");
		exit(-1);
	}

	int i;
	float Acc;
	FILE *f = NULL;
	char fileName[256];

	fprintf(stdout, "\nReading data file ..."); fflush(stdout);
	Subgraph *g = ReadSubgraph(argv[1]);
	fprintf(stdout, " OK"); fflush(stdout);

	fprintf(stdout, "\nReading output file ..."); fflush(stdout);
	sprintf(fileName,"%s.out",argv[1]);
	f = fopen(fileName,"r");
	if(!f){
		fprintf(stderr,"\nunable to open file %s", argv[2]);
		exit(-1);
	}
	for (i = 0; i < g->nnodes; i++)
		fscanf(f,"%d",&g->node[i].label);
	fclose(f);
	fprintf(stdout, " OK"); fflush(stdout);

	fprintf(stdout, "\nComputing accuracy ..."); fflush(stdout);
	Acc = opf_Accuracy(g);
	fprintf(stdout, "\nAccuracy: %.2f%%", Acc*100); fflush(stdout);

	fprintf(stdout, "\nWriting plot file ..."); fflush(stdout);
	sprintf(fileName,"%s.plot",argv[1]);
	output_plottable_file(g, fileName);
	fprintf(stdout, " OK"); fflush(stdout);

	fprintf(stdout, "\nWriting accuracy in output file ..."); fflush(stdout);
	sprintf(fileName,"%s.acc",argv[1]);
	f = fopen(fileName,"a");
	fprintf(f,"%f\n",Acc*100);
	fclose(f);
	fprintf(stdout, " OK"); fflush(stdout);

	fprintf(stdout, "\nDeallocating memory ..."); fflush(stdout);
	DestroySubgraph(&g);
	fprintf(stdout, " OK\n");

	return 0;
}
