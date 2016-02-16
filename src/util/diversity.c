#include "diversity.h"

// Computes the diversity between classifiers, based on classified subgraphs of the same testing set
float computeDiversity(Subgraph **g, int number_of_classifiers, int diversityFunction){
	// The true/false positive matrixes
	float **matrix_A;
	float *error_vector;	
	float **matrix_D;
	// Allocating the matrixes
	matrix_A = (float**)calloc(number_of_classifiers,sizeof(float*));//amostras classificadas corretamente entre os 2 classificadores (A)
	error_vector = (float *)calloc(number_of_classifiers,sizeof(float));//amostras classificadas incorretamente por cada classificador (B)(C)	
	matrix_D = (float**)calloc(number_of_classifiers,sizeof(float*));//amostras classificadas incorretamente entre os 2 classificadores (D)
	
	int i,j,n; 		// Iterators

	int sample_count = g[0]->nnodes;

	//resultados
	float **div;	
	div = (float **)calloc(number_of_classifiers,sizeof(float*));
	
	//aloca as colunas da matriz
	for (i=0; i < number_of_classifiers; i++){ 
	    matrix_A[i] = (float*)calloc(number_of_classifiers,sizeof(float));	   
	    matrix_D[i] = (float*)calloc(number_of_classifiers,sizeof(float));
	    div[i] = (float*)calloc(number_of_classifiers,sizeof(float));		
	}
	
	//acha a quantidade  de erro de cada classificador
	for(i=0; i<number_of_classifiers; i++){
		for(n=0; n<sample_count; n++){
			if((g[i]->node[n].label) != g[i]->node[n].truelabel){				
				error_vector[i] += 1.0;				
			} 
		}
	}	
	
	//calcula matriz A e D
	for(i=0; i<number_of_classifiers; i++){		
		for(j=0; j<number_of_classifiers; j++){
			if(j != i){
				for(n=0; n<sample_count; n++){					
					//printf("[n = %d]\n",n);
					if((g[i]->node[n].label) == g[i]->node[n].truelabel && (g[j]->node[n].label) == g[i]->node[n].truelabel)
						matrix_A[i][j] += 1.0;
							
					else if((g[i]->node[n].label) != g[i]->node[n].truelabel && (g[j]->node[n].label) != g[i]->node[n].truelabel)
						matrix_D[i][j] += 1.0;
				}
			} 
		}	
	}
		
	//Dividindo os elementos da matriz pelo total de amostras 
	for(i=0; i<number_of_classifiers; i++){
		error_vector[i] = error_vector[i] / sample_count;
				for(j=0; j<number_of_classifiers; j++){
					matrix_D[i][j] = matrix_D[i][j] / sample_count;
					matrix_A[i][j] = matrix_A[i][j] / sample_count;
				}
	}

	//tipos de diversidades
		//0: double fault
		//1: Q- statistic
		//2: Inter-rated agreement
		//3: The Disagree Measure
		//4: Correlation
		
		//calcular a média dessas matrizes
		//colocar eles em uma única matriz
		//usar a estatistica Q 
	
	switch (diversityFunction){
		case 0:		
			for(i=0; i<number_of_classifiers; i++)				
				for(j=0; j<number_of_classifiers; j++)
					div[i][j] = matrix_D[i][j];
		break;
		
		case 1:
			for(i=0; i<number_of_classifiers; i++){				
				for(j=0; j<number_of_classifiers; j++){
					if(j != i){					
						for(n=0; n<sample_count; n++)						
							div[i][j] = ((matrix_A[i][j]* matrix_D[i][j]) - (error_vector[i]*error_vector[j]))/ ((matrix_A[i][j]* matrix_D[i][j]) + (error_vector[i]*error_vector[j]));
					} 
				}	
			}
		break;
						
		case 2:	
			for(i=0; i<number_of_classifiers; i++){				
				for(j=0; j<number_of_classifiers; j++){
					if(j != i){						
						for(n=0; n<sample_count; n++)							
							div[i][j] = (2*((matrix_A[i][j]* matrix_D[i][j])-(error_vector[i]*error_vector[j])))/(((matrix_A[i][j] + error_vector[j])*(error_vector[j]+ matrix_D[i][j]))+((matrix_A[i][j] + error_vector[i])*(error_vector[i] +matrix_D[i][j])));
					} 
				}	
			}
		break;
		
		case 3:
			for(i=0; i<number_of_classifiers; i++){				
				for(j=0; j<number_of_classifiers; j++){
					if(j != i){						
						for(n=0; n<sample_count; n++)							
							div[i][j] = error_vector[i]+error_vector[j];
					} 
				}	
			}
			break;
			
		case 4:
			for(i=0; i<number_of_classifiers; i++){				
				for(j=0; j<number_of_classifiers; j++){
					if(j != i){						
						for(n=0; n<sample_count; n++)							
							div[i][j] = ((matrix_A[i][j]* matrix_D[i][j])-(error_vector[i]*error_vector[j]))/(sqrt((matrix_A[i][j] + error_vector[i])*(error_vector[j]+ matrix_D[i][j])*(matrix_A[i][j] + error_vector[j])*(error_vector[i]+matrix_D[i][j])));
					} 
				}	
			}
			break;
			
	}
	
	
	//calculando a diagonal superior - so ta somando os diferentes...nao soma 0 e 0 etc...tem q ver se nao sao 0
	float resD = 0.0f;	// Results sum
	float resA = 0.0f;	
	int count = 0;		// Results count
	float result = 0.0f;//guarda o resultado da matriz
	for(i=0; i<number_of_classifiers-1; i++){
		for(j=i+1; j<number_of_classifiers; j++){
			result = result + div[i][j];
			resD = resD + matrix_D[i][j];
			resA = resA + matrix_A[i][j];
			count++;
		}
	}
	
	return result/count;
}