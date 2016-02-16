#include "OPF.h"
#include "OPFincremental.h"

int main(int argc, char **argv){
	fflush(stdout);
	fprintf(stdout, "\nProgram that incrementally includes a dataset in an OPF model\n");
	fprintf(stdout, "\nIf you have any problem, please contact: ");
	fprintf(stdout, "\n- riva.mateus@gmail.com");
	fprintf(stdout, "\n- moacirponti@gmail.com\n");
	fprintf(stdout, "\nLibOPF version 2.0 (2009)\n");
	fprintf(stdout, "\n"); fflush(stdout);

	if((argc != 3)){
		fprintf(stderr, "\nusage opf_includenew <P1> <P2>");
		fprintf(stderr, "\nP1: new data to include in classifier (also in the OPF file format)");
		fprintf(stderr, "\nP2: new training set (also in the OPF file format)\n");
		exit(-1);
	}

	int i;
	float time;
	char fileName[256];
	FILE *f = NULL;
	timer tic, toc;

	fprintf(stdout, "\nReading data files ..."); fflush(stdout);
	Subgraph *gTrain = opf_ReadModelFile("classifier.opf");
	Subgraph *gNewData = ReadSubgraph(argv[1]);
	fprintf(stdout, " OK"); fflush(stdout);

	fprintf(stdout, "\nIncluding newdata into trainingset ..."); fflush(stdout);
	gettimeofday(&tic,NULL);
 
	opf_OPFClassifyingDIFT(gTrain, gNewData); gettimeofday(&toc,NULL);
	fprintf(stdout, " OK"); fflush(stdout);

	fprintf(stdout, "\nWriting new classifier and output file ..."); fflush(stdout);
	sprintf(fileName,"%s.out",argv[1]);
	opf_WriteModelFile(gTrain, "classifier.opf");
	WriteSubgraph(gTrain, argv[2]);
	WriteSubgraphPrototype(gTrain, "newproto.dat");
	fprintf(stdout,"\n\tNew training set has: %d nodes", gTrain->nnodes);

	f = fopen(fileName,"w");
	for (i = 0; i < gTrain->nnodes; i++){
		//fprintf(f,"%d %d\n",gTest->node[i].label,gTest->node[i].truelabel);
		fprintf(f,"%d\t",gTrain->node[i].label);
	}
	fclose(f);
	fprintf(stdout, " OK"); fflush(stdout);

	fprintf(stdout, "\nDeallocating memory ...");
	DestroySubgraph(&gTrain);
	DestroySubgraph(&gNewData);
	fprintf(stdout, " OK\n");

	time = ((toc.tv_sec-tic.tv_sec)*1000.0 + (toc.tv_usec-tic.tv_usec)*0.001)/1000.0;
	fprintf(stdout, "\nTesting time: %f seconds\n", time); fflush(stdout);

	sprintf(fileName,"%s.time",argv[1]);
	f = fopen(fileName,"a");
	fprintf(f,"%f\n",time);
	fclose(f);

	return 0;
}
