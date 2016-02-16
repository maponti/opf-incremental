#include "OPF.h"
#include <stdio.h>
#include <string.h>
#include "util/subgraph.h"

void CheckInputData(float TrPercentage, float TestPercentage){
	fprintf(stderr, "\nSummation of set percentages = %.1f ...",TrPercentage+TestPercentage);
	if((float)(TrPercentage+TestPercentage) != (float)1.0)
		Error("Percentage summation is not equal to 1","CheckInputData");
	fprintf(stderr, " OK");

	fprintf(stderr, "\nChecking set percentages ...");
	if(TrPercentage == 0.0f || TestPercentage == 0.0f)
		Error("Percentage of either training set or test set is equal to 0", "CheckInputData");
	printf(" OK");
}

int main(int argc, char **argv){
	fflush(stdout);
	fprintf(stdout, "\nProgram that generates testing and multiple training sets for the OPF-i classifier\n");
	fprintf(stdout, "\nIf you have any problem, please contact: ");
	fprintf(stdout, "\n- riva.mateus@gmail.com");
	fprintf(stdout, "\n- moacirponti@gmail.com\n");
	fprintf(stdout, "\nLibOPFi version 1.0 (2015)\n");
	fprintf(stdout, "\n"); fflush(stdout);

	if(argc != 7){
		fprintf(stderr, "\nusage opf_split <P1> <P2> <P3> <P4> <P5> <P6>");
		fprintf(stderr, "\nP1: input dataset in the OPF file format");
		fprintf(stderr, "\nP2: percentage for the training sets total size [0,1]");
		fprintf(stderr, "\nP3: percentage for the test set size [0,1]");
		fprintf(stderr, "\nP4: percentage of the initial training set");
		fprintf(stderr, "\nP5: total of incremental training sets, including the initial");
		fprintf(stderr, "\nP6: normalize features? 1 - Yes  0 - No\n\n");
		exit(-1);
	}
	Subgraph *g = NULL, *gAux = NULL, **gTraining = NULL, *gTesting = NULL;
	float training_p = atof(argv[2]), testing_p = atof(argv[3]), training_0_p = atof(argv[4]);
	int normalize = atoi(argv[6]), training_count = atoi(argv[5]);
	char full_base_name[64];
	strcpy(full_base_name, argv[1]);
	char* base_name = strtok(full_base_name,".");

	CheckInputData(training_p, testing_p);

	fprintf(stdout, "\nReading data set ..."); fflush(stdout);
	g = ReadSubgraph(argv[1]);
	fprintf(stdout, " OK"); fflush(stdout);

	if(normalize) opf_NormalizeFeatures(g);

	fprintf(stdout, "\nSplitting data set into main training and testing..."); fflush(stdout);
	opf_SplitSubgraph(g, &gAux, &gTesting, training_p);

	fprintf(stdout, " OK"); fflush(stdout);

	// Allocates all training sets
	gTraining = (Subgraph**)malloc(training_count*sizeof(Subgraph*));

	// Splitting training set into 'count' subsets
	gTraining[0] = CopySubgraph(gAux);
	fprintf(stdout, "\nSplitting main training set into S_0 and %d subsets...", training_count); fflush(stdout);
	
	fprintf(stdout, "\n\tCurrently splitting subset %d of size %d (remainder: %f)...", 0, gTraining[0]->nnodes, training_0_p); fflush(stdout);
	opf_SplitSubgraph(gTraining[0], &(gTraining[0]), &(gTraining[0+1]), training_0_p);
	fprintf(stdout, " OK"); fflush(stdout);
	int i;
	for (i = 1; i < training_count-1; ++i) {
		fprintf(stdout, "\n\tCurrently splitting subset %d of size %d (remainder: %f)...", i, gTraining[i]->nnodes, 1.0 - (((float)training_count - (float)i - 1.0)/((float)training_count - (float)i))); fflush(stdout);
		opf_SplitSubgraph(gTraining[i], &(gTraining[i]), &(gTraining[i+1]), 1.0 - (((float)training_count - (float)i - 1.0)/((float)training_count - (float)i)));
		fprintf(stdout, " OK"); fflush(stdout);
	}

	fprintf(stdout, " OK"); fflush(stdout);

	fprintf(stdout, "\nWriting data sets to disk ..."); fflush(stdout);
	for(i = 0; i < training_count; i++){
		char subset_name[64], incrementalID[5];
		sprintf(incrementalID, "%04d", i);
		strcpy(subset_name, base_name);
		strcat(subset_name, "I_");
		strcat(subset_name, incrementalID);
		strcat(subset_name, ".dat");
		WriteSubgraph(gTraining[i], subset_name);
	}
	char subset_name[64];
	strcpy(subset_name, base_name);
	strcat(subset_name, "T");
	strcat(subset_name, ".dat");
	WriteSubgraph(gTesting, subset_name);
	fprintf(stdout, " OK"); fflush(stdout);

	fprintf(stdout, "\nDeallocating memory ...");
	DestroySubgraph(&g);
	DestroySubgraph(&gAux);
	for(i = 0; i < training_count; i++)
		DestroySubgraph(&(gTraining[i]));
	free(gTraining);
	DestroySubgraph(&gTesting);
	fprintf(stdout, " OK\n");

	return 0;
}
