
//dentro da funcao combine
//o prototipos nao sao treinados entao nao cria matriz de output
float diversidade(int **matrix_label, int numclas, int namostras, Subgraph *gtesting, int diversidade){
	float **matrix_A;
	float *vetor_erros;	
	float **matrix_D;
	matrix_A = (float**)calloc(numclas,sizeof(float*));//amostras classificadas corretamente entre os 2 classificadores (A)
	vetor_erros = (float *)calloc(numclas,sizeof(float));//amostras classificadas incorretamente por cada classificador (B)(C)	
	matrix_D = (float**)calloc(numclas,sizeof(float*));//amostras classificadas incorretamente entre os 2 classificadores (D)
	int i,j,n;
	float resD = 0.0f;
	float resA = 0.0f;
	int num = 0;
	//resultados
	float result = 0.0f;//guarda o resultado da matriz
	float **div;	
	div = (float **)calloc(numclas,sizeof(float*));
	
	//aloca as colunas da matriz
	for (i=0; i < numclas; i++){ 
	    matrix_A[i] = (float*)calloc(numclas,sizeof(float));	   
	    matrix_D[i] = (float*)calloc(numclas,sizeof(float));
	    div[i] = (float*)calloc(numclas,sizeof(float));		
	}
	
	//acha a quantidade  de erro de cada classificador
	for(i=0; i<numclas; i++){
		for(n=0; n<namostras; n++){
			if((matrix_label[i][n] +1) != gtesting->node[n].truelabel){				
				vetor_erros[i] += 1.0;				
			} 
		}
	}	
	
	//calcula matriz A e D
	for(i=0; i<numclas; i++){		
		for(j=0; j<numclas; j++){
			if(j != i){
				for(n=0; n<namostras; n++){					
					//printf("[n = %d]\n",n);
					if((matrix_label[i][n] +1) == gtesting->node[n].truelabel && (matrix_label[j][n] +1) == gtesting->node[n].truelabel)
						matrix_A[i][j] += 1.0;
							
					else if((matrix_label[i][n] +1) != gtesting->node[n].truelabel && (matrix_label[j][n] +1) != gtesting->node[n].truelabel)
						matrix_D[i][j] += 1.0;
				}
			} 
		}	
	}
		
	//Dividindo os elementos da matriz pelo total de amostras 
	for(i=0; i<numclas; i++){
		vetor_erros[i] = vetor_erros[i] / namostras;
				for(j=0; j<numclas; j++){
					matrix_D[i][j] = matrix_D[i][j] / namostras;
					matrix_A[i][j] = matrix_A[i][j] / namostras;
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
	
	switch (diversidade){
		case 0:		
			for(i=0; i<numclas; i++)				
				for(j=0; j<numclas; j++)
					div[i][j] = matrix_D[i][j];
		break;
		
		case 1:
			for(i=0; i<numclas; i++){				
				for(j=0; j<numclas; j++){
					if(j != i){					
						for(n=0; n<namostras; n++)						
							div[i][j] = ((matrix_A[i][j]* matrix_D[i][j]) - (vetor_erros[i]*vetor_erros[j]))/ ((matrix_A[i][j]* matrix_D[i][j]) + (vetor_erros[i]*vetor_erros[j]));
					} 
				}	
			}
		break;
						
		case 2:	
			for(i=0; i<numclas; i++){				
				for(j=0; j<numclas; j++){
					if(j != i){						
						for(n=0; n<namostras; n++)							
							div[i][j] = (2*((matrix_A[i][j]* matrix_D[i][j])-(vetor_erros[i]*vetor_erros[j])))/(((matrix_A[i][j] + vetor_erros[j])*(vetor_erros[j]+ matrix_D[i][j]))+((matrix_A[i][j] + vetor_erros[i])*(vetor_erros[i] +matrix_D[i][j])));
					} 
				}	
			}
		break;
		
		case 3:
			for(i=0; i<numclas; i++){				
				for(j=0; j<numclas; j++){
					if(j != i){						
						for(n=0; n<namostras; n++)							
							div[i][j] = vetor_erros[i]+vetor_erros[j];
					} 
				}	
			}
			break;
			
		case 4:
			for(i=0; i<numclas; i++){				
				for(j=0; j<numclas; j++){
					if(j != i){						
						for(n=0; n<namostras; n++)							
							div[i][j] = ((matrix_A[i][j]* matrix_D[i][j])-(vetor_erros[i]*vetor_erros[j]))/(sqrt((matrix_A[i][j] + vetor_erros[i])*(vetor_erros[j]+ matrix_D[i][j])*(matrix_A[i][j] + vetor_erros[j])*(vetor_erros[i]+matrix_D[i][j])));
					} 
				}	
			}
			break;
			
	}
	
	
	//calculando a diagonal superior - so ta somando os diferentes...nao soma 0 e 0 etc...tem q ver se nao sao 0
	for(i=0; i<numclas-1; i++){
		for(j=i+1; j<numclas; j++){
			result = result + div[i][j];
			resD = resD + matrix_D[i][j];
			resA = resA + matrix_A[i][j];
			num++;
		}
	}
	
	return result/num;
}