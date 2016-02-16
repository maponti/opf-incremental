#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char const *argv[])
{
	if (argc < 2){
		printf("Usage: result_organizer <results file> [<output file>]\n");
		return -1;
	}

	// Vector for storing accumulated time of original and incremental methods
	int totalOfResults = 0;
	float **originalTimes = NULL, **incrementalTimes = NULL;
	float auxiliaryTime;
	int i = 0, j;

	char debug[50];

	// Opening the results file
	printf("Opening results file %s...", argv[1]);
	FILE *resultsFile = fopen(argv[1], "r");
	printf(" open.\n");

	// Obtaining the initial testing times, along with the number of results
	if (fscanf(resultsFile, "Time") < 0) { return -2; }
	while (fscanf(resultsFile, "%f", &auxiliaryTime) > 0){
		totalOfResults++;
		originalTimes = (float**)realloc(originalTimes, sizeof(float*)*totalOfResults);
		incrementalTimes = (float**)realloc(incrementalTimes, sizeof(float*)*totalOfResults);

		originalTimes[totalOfResults-1] = (float*)malloc(11*sizeof(float));
		incrementalTimes[totalOfResults-1] = (float*)malloc(11*sizeof(float));
		incrementalTimes[totalOfResults-1][0] = originalTimes[totalOfResults-1][0] = auxiliaryTime;
	}
	printf("\nTotal of results: %d\n\n", totalOfResults);

	if (fscanf(resultsFile, "Original") < 0) return -2;
	// Obtaining accumulated original times
	i = 0; j = 1;
	while (fscanf(resultsFile, "%f", &auxiliaryTime) > 0){
		originalTimes[i][j] = originalTimes[i][j-1] + auxiliaryTime;
		
		i = (i + 1)%totalOfResults;
		if (i % totalOfResults == 0) j++;
	}

	if (fscanf(resultsFile, "Incremental") < 0) return -2;
	fscanf(resultsFile, "%s", debug);
	// Obtaining accumulated incremental times
	i = 0; j = 1;
	while (fscanf(resultsFile, "%f", &auxiliaryTime) > 0){
		incrementalTimes[i][j] = incrementalTimes[i][j-1] + auxiliaryTime;
		
		i = (i + 1)%totalOfResults;
		if (i % totalOfResults == 0) j++;
	}

	// Outputing to stdout (or file)
	FILE* output;
	if (argc == 3)
		output = fopen(argv[2], "w");
	else
		output = stdout;

	fprintf(output, "Original Times:\n");
	for(i = 0; i < totalOfResults; i++){
		fprintf(output, "result %d\n", i);
		for (j = 0; j < 11; j++)
			fprintf(output, "%f\n", originalTimes[i][j]);
	}
	fprintf(output, "Incremental Times:\n");
	for(i = 0; i < totalOfResults; i++){
		fprintf(output, "result %d\n", i);
		for (j = 0; j < 11; j++)
			fprintf(output, "%f\n", incrementalTimes[i][j]);
	}
	

	if (output != stdout) fclose(output);
	fclose(resultsFile);
	for (i = 0; i < totalOfResults; i++){
		free(originalTimes[i]); free(incrementalTimes[i]);
	}
	free(originalTimes);
	free(incrementalTimes);

	return 0;
}