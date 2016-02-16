#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct{
	int id;
	int label;

	float feat[2];
} data;

void writeData(data newData, FILE* fpTxt, FILE* fpOPF){
	fprintf(fpTxt,"%d\t",newData.id);			
	fprintf(fpTxt,"%d\t",newData.label);

	fwrite(&(newData.id), sizeof(int), 1,fpOPF);
	fwrite(&(newData.label), sizeof(int), 1,fpOPF);
	
	//X
	fprintf(fpTxt,"%.6f\t",(newData.feat[0]));
	fwrite(&(newData.feat[0]), sizeof(float), 1, fpOPF);

	//Y
	fprintf(fpTxt,"%.6f\t",(newData.feat[1]));
	fwrite(&(newData.feat[1]), sizeof(float), 1, fpOPF);
	
	fprintf(fpTxt, "\n");
}

int main(int argc, char **argv)
{

	if (argc != 4) 
	{
		fprintf(stderr,"\nusage: base_creator <base name> <base_type> <numero_exemplos>\n");
		exit(-1);
	}

	printf("\nPrograma para criar uma base para o teste do OPFI");

	FILE *fpOutText = NULL, *fpOutText2 = NULL;
	FILE *fpOutOPF = NULL, *fpOutOPF2 = NULL;
	int n, ndata, nclasses, i, j;

	// Armazena o arquivo completo na memoria
	data* fullData;

	// Vetor auxiliar para sorteio
	char* picked;

	// Inicializando o RNG
	srand((int)time(NULL));

	// ======================== FULL ============================

	printf("\nCriando base full...\n");

	char filenameFullTxt[64];
	strcpy(filenameFullTxt, argv[1]);
	strcat(filenameFullTxt, "full.txt");
	fpOutText2 = fopen(filenameFullTxt, "w");

	char filenameFullOPF[64];
	strcpy(filenameFullOPF, argv[1]);
	strcat(filenameFullOPF, "full.dat");
	fpOutOPF2 = fopen(filenameFullOPF, "wb");

	j = 0;

	/*gravando numero de objetos*/
	n = atoi(argv[3]);
	fprintf(fpOutText2,"%d\t",n); printf("\nnobjects: %d", n);
	fwrite(&n, sizeof(int), 1, fpOutOPF2);

	/*gravando numero de classes*/
	nclasses = argv[2][0] == '1' ? 2 : argv[2][0] == '2' ? 4 : argv[2][0] == '3' ? 3 : 2;
	fprintf(fpOutText2,"%d\t",nclasses); printf("\nnclasses: %d", nclasses); 
	fwrite(&nclasses, sizeof(int), 1, fpOutOPF2);

	/*gravando tamanho vetor de caracteristicas*/
	ndata = 2;
	fprintf(fpOutText2,"%d\n",ndata); printf("\nndata: %d\n\n", ndata);
	fwrite(&ndata, sizeof(int), 1, fpOutOPF2);

	/* Alocando na memoria*/
	fullData = (data*)malloc(sizeof(data)*n);
	picked = (char*)calloc(n,sizeof(char));

	/*gravando dados das classes*/
	for(i = 0; i < n; i++)	{
		fullData[i].id = i;	
		fullData[i].label = nclasses == 2 ? (i < n/4 || (i > n/2 && i < 3*n/4)? 1 : 2)
							: nclasses == 3 ? (i < n/4 || (i > n/2 && i < 3*n/4)? 1 : i > 3*n/4 ? 3 : 2)
							: nclasses == 4 ? (i/(n/4) + 1) 
							: 0;	

		//X
		fullData[i].feat[0] = ((float)rand()/4.0)/(float)RAND_MAX;
		fullData[i].feat[0] += i < n/4 ? 0.0 : i < n/2 ? 0.25 : i < 3*n/4 ? 0.5 : 0.75;

		//Y
		fullData[i].feat[1] = (float)rand()/(float)RAND_MAX;

		writeData(fullData[i], fpOutText2, fpOutOPF2);
	}

	fclose(fpOutText2);
	fclose(fpOutOPF2);

	// ====================== S & T ==========================
	printf("Criando base S...\n");

	char filenameSTxt[64];
	strcpy(filenameSTxt, argv[1]);
	strcat(filenameSTxt, "S.txt");
	fpOutText = fopen(filenameSTxt, "w");

	char filenameSOPF[64];
	strcpy(filenameSOPF, argv[1]);
	strcat(filenameSOPF, "S.dat");
	fpOutOPF = fopen(filenameSOPF, "wb");

	printf("Criando base T...\n");

	char filenameTTxt[64];
	strcpy(filenameTTxt, argv[1]);
	strcat(filenameTTxt, "T.txt");
	fpOutText2 = fopen(filenameTTxt, "w");

	char filenameTOPF[64];
	strcpy(filenameTOPF, argv[1]);
	strcat(filenameTOPF, "T.dat");
	fpOutOPF2 = fopen(filenameTOPF, "wb");

	/*gravando numero de objetos*/
	int halfN = n/2;
	fprintf(fpOutText,"%d\t",n/2); printf("\nnobjects: %d", halfN);
	fwrite(&(halfN), sizeof(int), 1, fpOutOPF);
	fprintf(fpOutText2,"%d\t",halfN); printf("\nnobjects: %d", halfN);
	fwrite(&(halfN), sizeof(int), 1, fpOutOPF2);

	/*gravando numero de classes*/
	fprintf(fpOutText,"%d\t",nclasses); printf("\nnclasses: %d", nclasses); 
	fwrite(&nclasses, sizeof(int), 1, fpOutOPF);
	fprintf(fpOutText2,"%d\t",nclasses); printf("\nnclasses: %d", nclasses); 
	fwrite(&nclasses, sizeof(int), 1, fpOutOPF2);

	/*gravando tamanho vetor de caracteristicas*/
	fprintf(fpOutText,"%d\n",ndata); printf("\nndata: %d\n\n", ndata);
	fwrite(&ndata, sizeof(int), 1, fpOutOPF);
	fprintf(fpOutText2,"%d\n",ndata); printf("\nndata: %d\n\n", ndata);
	fwrite(&ndata, sizeof(int), 1, fpOutOPF2);
	
	/*sorteando metade dos dados. Os 'picked' irao para S, os nao picked irao para T*/
	for(i = 0; i < halfN; i++){
		int randomIndex = rand() % n;

		for(;;){
			if (!picked[randomIndex]) {picked[randomIndex] = 1; break;}
			randomIndex = rand() % n;
		}
	}

	/*gravando dados nos conjuntos S e T*/
	for(i = 0; i < n; i++)	{
		if (picked[i]) writeData(fullData[i], fpOutText, fpOutOPF);
		else writeData(fullData[i], fpOutText2, fpOutOPF2);
	}

	fclose(fpOutText);
	fclose(fpOutOPF);
	fclose(fpOutText2);
	fclose(fpOutOPF2);

	// ====================== S_1 ==========================

	printf("Criando base S_1...\n");

	char filenameS_1Txt[64];
	strcpy(filenameS_1Txt, argv[1]);
	strcat(filenameS_1Txt, "S_1.txt");
	fpOutText = fopen(filenameS_1Txt, "w");

	char filenameS_1OPF[64];
	strcpy(filenameS_1OPF, argv[1]);
	strcat(filenameS_1OPF, "S_1.dat");
	fpOutOPF = fopen(filenameS_1OPF, "wb");

	/* conta o numero de exemplos no conjunto S na coluna do meio */
	int nInS_1 = 0;
	for (i = n/4; i < 3*n/4; i++) if (picked[i]) nInS_1++;

	/*gravando numero de objetos*/
	fprintf(fpOutText,"%d\t",nInS_1); printf("\nnobjects: %d", nInS_1);
	fwrite(&nInS_1, sizeof(int), 1, fpOutOPF);

	/*gravando numero de classes*/
	fprintf(fpOutText,"%d\t",nclasses); printf("\nnclasses: %d", nclasses); 
	fwrite(&nclasses, sizeof(int), 1, fpOutOPF);

	/*gravando tamanho vetor de caracteristicas*/
	fprintf(fpOutText,"%d\n",ndata); printf("\nndata: %d\n\n", ndata);
	fwrite(&ndata, sizeof(int), 1, fpOutOPF);

	/*grava apenas os exemplos das colunas do meio (entre n/4 e 3n/4)*/
	for(i = n/4; i < 3*n/4; i++){
		if (picked[i]) writeData(fullData[i], fpOutText, fpOutOPF);
	}

	fclose(fpOutText);
	fclose(fpOutOPF);

	// ====================== I_i ==========================

	// Reinicia o vetor de sorteio
	char* incPicked = (char*)calloc(n,sizeof(char));

	/* conta o numero de exemplos no conjunto S fora da coluna do meio */
	int nNotInS_1 = n/2 - nInS_1;

	/*Grava os 10 conjuntos incrementais*/
	for (j = 1; j <= 10; j++){
		printf("Criando base I_%d...\n", j);

		char filenameI_iTxt[64],jValue[3];
		jValue[0] = j/10 + '0';
		jValue[1] = j%10 + '0';
		jValue[2] = '\0';

		strcpy(filenameI_iTxt, argv[1]);
		strcat(filenameI_iTxt, "I_");
		strcat(filenameI_iTxt, jValue);
		strcat(filenameI_iTxt, ".txt");
		fpOutText = fopen(filenameI_iTxt, "w");

		char filenameI_iOPF[64];
		strcpy(filenameI_iOPF, argv[1]);
		strcat(filenameI_iOPF, "I_");
		strcat(filenameI_iOPF, jValue);
		strcat(filenameI_iOPF, ".dat");
		fpOutOPF = fopen(filenameI_iOPF, "wb");

		/*gravando numero de objetos*/
		int tenthN = nNotInS_1/10;
		fprintf(fpOutText,"%d\t",tenthN); printf("\nnobjects: %d", (tenthN));
		fwrite(&(tenthN), sizeof(int), 1, fpOutOPF);

		/*gravando numero de classes*/
		fprintf(fpOutText,"%d\t",nclasses); printf("\nnclasses: %d", nclasses); 
		fwrite(&nclasses, sizeof(int), 1, fpOutOPF);

		/*gravando tamanho vetor de caracteristicas*/
		fprintf(fpOutText,"%d\n",ndata); printf("\nndata: %d\n\n", ndata);
		fwrite(&ndata, sizeof(int), 1, fpOutOPF);

		/*sorteando e escrevendo elementos*/
		for (i = 0; i < nNotInS_1/10; i++){
			int randomIndex = rand() % n/2;
			// Ajustando randomIndex para a ultima coluna
			if (randomIndex > n/4) randomIndex += n/2;

			for (;;){
				if (!incPicked[randomIndex]) {
					incPicked[randomIndex] = 1; 
					writeData(fullData[randomIndex], fpOutText, fpOutOPF);
					break;
				}

				randomIndex = rand() % n/2;
				// Ajustando randomIndex para a ultima coluna
				if (randomIndex > n/4) randomIndex += n/2;
			}
		}
	}


	free(fullData);
	free(picked);
	free(incPicked);

	return 0;
}
