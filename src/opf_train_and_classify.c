#include "OPF.h"

int main(int argc, char **argv){
	fflush(stdout);
	fprintf(stdout, "\nProgram that executes the training and classification phase of the OPF classifier\n");
	fprintf(stdout, "\nIf you have any problem, please contact: ");
	fprintf(stdout, "\n- riva.mateus@gmail.com");
	fprintf(stdout, "\n- moacirponti@gmail.com\n");
	fprintf(stdout, "\nLibOPF version 2.0 (2009)\n");
	fprintf(stdout, "\n"); fflush(stdout);

	if((argc != 3) && (argc != 2)){
		fprintf(stderr, "\nusage opf_train_and_classify <P1> <P2> <P3>");
		fprintf(stderr, "\nP1: training set in the OPF file format");
		fprintf(stderr, "\nP2: test set in the OPF file format");
		fprintf(stderr, "\nP3: precomputed distance file (leave it in blank if you are not using this resource)\n");
		exit(-1);
	}

	int n, i;
	char fileName[256];
	FILE *f = NULL;
	timer tic, toc,ticC,tocC;
	float time, timeC;

	if(argc == 4) opf_PrecomputedDistance = 1;

	fprintf(stdout, "\nReading data file ..."); fflush(stdout);
	Subgraph *g = ReadSubgraph(argv[1]);
	Subgraph *gTest = ReadSubgraph(argv[1]);
	fprintf(stdout, " OK"); fflush(stdout);

	if(opf_PrecomputedDistance)
		opf_DistanceValue = opf_ReadDistances(argv[3], &n);

	fprintf(stdout, "\nTraining OPF classifier ..."); fflush(stdout);
	gettimeofday(&tic,NULL); opf_OPFTraining(g); gettimeofday(&toc,NULL);
	fprintf(stdout, " OK"); fflush(stdout);

	fprintf(stdout, "\nClassifying test set ..."); fflush(stdout);
	gettimeofday(&ticC,NULL);
	opf_OPFClassifying(g, gTest); gettimeofday(&tocC,NULL);
	fprintf(stdout, " OK"); fflush(stdout);

	fprintf(stdout, "\nWriting classifier's model file ..."); fflush(stdout);
	opf_WriteModelFile(g, "classifier.opf");
	fprintf(stdout, " OK"); fflush(stdout);

	fprintf(stdout, "\nWriting output file ..."); fflush(stdout);
	sprintf(fileName,"%s.out",argv[1]);
	f = fopen(fileName,"w");
	for (i = 0; i < g->nnodes; i++)
		fprintf(f,"%d\n",g->node[i].label);
	fclose(f);
	fprintf(stdout, " OK"); fflush(stdout);

	fprintf(stdout, "\nDeallocating memory ..."); fflush(stdout);
	DestroySubgraph(&g); DestroySubgraph(&gTest);
	if(opf_PrecomputedDistance){
		for (i = 0; i < n; i++)
			free(opf_DistanceValue[i]);
		free(opf_DistanceValue);
	}
	fprintf(stdout, " OK\n");

	time = ((toc.tv_sec-tic.tv_sec)*1000.0 + (toc.tv_usec-tic.tv_usec)*0.001)/1000.0;
	fprintf(stdout, "\nTraining time: %f seconds\n", time); fflush(stdout);
	timeC = ((tocC.tv_sec-ticC.tv_sec)*1000.0 + (tocC.tv_usec-ticC.tv_usec)*0.001)/1000.0;
	fprintf(stdout, "\nTesting time: %f seconds\n", time); fflush(stdout);

	sprintf(fileName,"%s.time",argv[1]);
	f = fopen(fileName,"a");
	fprintf(f,"%f,%f\n",time,timeC);
	fclose(f);

	return 0;
}
