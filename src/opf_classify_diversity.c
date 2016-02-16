#include "OPF.h"
#include "diversity.h"

int main(int argc, char **argv){
	fflush(stdout);
	fprintf(stdout, "\nProgram that executes the test phase of the OPF classifier\n");
	fprintf(stdout, "\n\tfor two classifiers and computes the diversity\n");
	fprintf(stdout, "\nIf you have any problem, please contact: ");
	fprintf(stdout, "\n- riva.mateus@gmail.com");
	fprintf(stdout, "\n- moacirponti@gmail.com\n");
	fprintf(stdout, "\nLibOPFi version 1.0 (2015)\n");
	fprintf(stdout, "\n"); fflush(stdout);

	if((argc != 6) && (argc != 5)){
		fprintf(stderr, "\nusage opf_classify <P1> <P2> <P3> <P4> <P5>");
		fprintf(stderr, "\nP1: test set in the OPF file format");
		fprintf(stderr, "\nP2: first classifier in the OPF file format");
		fprintf(stderr, "\nP3: other classifier in the OPF file format");
		fprintf(stderr, "\nP4: diversity function to use (0 is double-fault, 1 is Q-statistic, 2 is inter-rated agreement, 3 is disagreement, 4 is correlation)");
		fprintf(stderr, "\nP5: precomputed distance file (leave it in blank if you are not using this resource\n");
		exit(-1);
	}

	int n,i;
	float time1, time2;
	char fileName[256];
	FILE *f = NULL;
	timer tic1, toc1;
	timer tic2, toc2;

	if(argc == 6) opf_PrecomputedDistance = 1;
	fprintf(stdout, "\nReading data files ..."); fflush(stdout);
	Subgraph *gTest1 = ReadSubgraph(argv[1]), *gTest2 = ReadSubgraph(argv[1]);
	Subgraph *gTrain1 = opf_ReadModelFile(argv[2]), *gTrain2 = opf_ReadModelFile(argv[3]);
	fprintf(stdout, " OK"); fflush(stdout);

	if(opf_PrecomputedDistance)
		opf_DistanceValue = opf_ReadDistances(argv[5], &n);

	fprintf(stdout, "\nClassifying test set with classifier 1..."); fflush(stdout);
	gettimeofday(&tic1,NULL);
	opf_OPFClassifying(gTrain1, gTest1); gettimeofday(&toc1,NULL);
	fprintf(stdout, " OK"); fflush(stdout);

	fprintf(stdout, "\nWriting output file ..."); fflush(stdout);
	sprintf(fileName,"%s1.out",argv[1]);
	f = fopen(fileName,"w");
	for (i = 0; i < gTest1->nnodes; i++){
		fprintf(f,"%d\n",gTest1->node[i].label);
	}
	fclose(f);
	fprintf(stdout, " OK"); fflush(stdout);


	fprintf(stdout, "\nClassifying test set with classifier 2..."); fflush(stdout);
	gettimeofday(&tic2,NULL);
	opf_OPFClassifying(gTrain2, gTest2); gettimeofday(&toc2,NULL);
	fprintf(stdout, " OK"); fflush(stdout);

	fprintf(stdout, "\nWriting output file ..."); fflush(stdout);
	sprintf(fileName,"%s2.out",argv[1]);
	f = fopen(fileName,"w");
	for (i = 0; i < gTest1->nnodes; i++){
		fprintf(f,"%d\n",gTest2->node[i].label);
	}
	fclose(f);
	fprintf(stdout, " OK"); fflush(stdout);

	// Computing diversity between classifiers
	Subgraph** g = (Subgraph**)malloc(2*sizeof(Subgraph*));
	g[0] = gTest1; g[1] = gTest2;
	float diversity = computeDiversity(g, 2, atoi(argv[4]));


	fprintf(stdout, "\nDeallocating memory ...");
	DestroySubgraph(&gTrain1);
	DestroySubgraph(&gTest1);
	DestroySubgraph(&gTrain2);
	DestroySubgraph(&gTest2);
	free(g);
	if(opf_PrecomputedDistance){
		for (i = 0; i < n; i++)
			free(opf_DistanceValue[i]);
		free(opf_DistanceValue);
	}
	fprintf(stdout, " OK\n");

	time1 = ((toc1.tv_sec-tic1.tv_sec)*1000.0 + (toc1.tv_usec-tic1.tv_usec)*0.001)/1000.0;
	fprintf(stdout, "\n1st Testing time: %f seconds\n", time1); fflush(stdout);
	time2 = ((toc2.tv_sec-tic2.tv_sec)*1000.0 + (toc2.tv_usec-tic2.tv_usec)*0.001)/1000.0;
	fprintf(stdout, "\n2nd Testing time: %f seconds\n", time2); fflush(stdout);

	fprintf(stdout, "\nDiversity: %f\n", diversity); fflush(stdout);

	sprintf(fileName,"%s.diversity",argv[1]);
	f = fopen(fileName,"a");
	fprintf(f,"%f\n",diversity);
	fclose(f);

	sprintf(fileName,"%s1.time",argv[1]);
	f = fopen(fileName,"a");
	fprintf(f,"%f\n",time1);
	fclose(f);
	sprintf(fileName,"%s2.time",argv[1]);
	f = fopen(fileName,"a");
	fprintf(f,"%f\n",time2);
	fclose(f);

	return 0;
}
