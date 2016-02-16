#include "OPFincremental.h"

// 
// Classification function - classifies the instance 'i' inside 'sg'
// 
void opf_OPFClassifyOneInstance(Subgraph *sgtrain, Subgraph *sg, int i)
{
	int j, k, l, label = -1;
	float tmp, weight, minCost;

	// primeiro, faz-se uma tentativa de classificacao fora do laco para 
	// o primeiro node do conjunto de treinamento, para resolver o problema
	// de inicializacao, presumo.
	j = 0;
	k = sgtrain->ordered_list_of_nodes[j];	// k eh o indice do primeiro no do subgrafo de treinamento
		
	// pega a distancia entre o no k de treinamento e o no i de teste (sendo classificado atualmente)
	if(!opf_PrecomputedDistance)
		// usa valor padrao do arco (distancia euclidiana)
		weight = opf_ArcWeight(sgtrain->node[k].feat,sg->node[i].feat,sg->nfeats);
	else
		// pega valor da distancia de uma matriz pre-calculada
		weight = opf_DistanceValue[sgtrain->node[k].position][sg->node[i].position];

	// o custo minimo eh o maior entre o pathval de k e a distancia calculada ali em cima
	// node[k].pathval eh o valor do caminho do no k para a sua raiz
	minCost = MAX(sgtrain->node[k].pathval, weight);
	label   = sgtrain->node[k].label;

	// percorre todos os nos j de treinamento ate minCost ser menor que o pathval deles (achar o mais proximo?)
	while ((j < sgtrain->nnodes-1) &&
    	   (minCost > sgtrain->node[sgtrain->ordered_list_of_nodes[j+1]].pathval)){
    //printf("j+1 eh: %d, ", j+1);
    //printf("no ordenado[%d] eh %d, ", j+1, sgtrain->ordered_list_of_nodes[j+1]);
    //printf("nao inicializado? = %f",sgtrain->node[sgtrain->ordered_list_of_nodes[j+1]].pathval);
    //printf("\n");

		// l eh o j+1-esimo no de treinamento
	    	l  = sgtrain->ordered_list_of_nodes[j+1];

	    	// calcula a distancia entre o no l e o no i
		if(!opf_PrecomputedDistance)
			weight = opf_ArcWeight(sgtrain->node[l].feat,sg->node[i].feat,sg->nfeats);
		else
			weight = opf_DistanceValue[sgtrain->node[l].position][sg->node[i].position];

		tmp = MAX(sgtrain->node[l].pathval, weight);
		if(tmp < minCost) {
			minCost = tmp;
			label = sgtrain->node[l].label;
		  	k = l;
	        }
       		j++;
	}
	sg->node[i].label = label;
	sg->node[i].pred = k;
}


/*----------- OPF-Incremental Code ------------------------------*/

/* ideia:
     - ao adicionar um no, classificar

     - caso ele pertenca a mesma classe (esteja correto) e nao seja ligado a um prototipo, chama INSERT e atualiza pathvals.

     - caso pertenca a mesma classe e seja ligado a um prototipo, decidir o novo prototipo. Para isso usar lista de adjacencia ou 
       lista de pares de prototipos

     - caso nao pertenca a mesma classe, adicionar no Grafo o novo no, e ambos viram prototipos (ou seja o novo no eh uma nova arvore), e entao 
       - percorrer a arvore com a qual o novo no se conectou, no sentido da raiz pois teremos que quebrar a arvore antiga em duas
       - para cada no adjacente, tentar conquista-lo: se a sua distancia + caminho ate o novo prototipo for menor do que o pathval
         entao o no fara parte da arvore nova, senao o processo para. A seguir atualizar os pathvals (apos ou durante a conquista com lista de adjacencia).
*/
// TODO: lista ordenada de nos!
         
//Incremental classification function: it  classifies labeled samples from sg and adds them to sgtrain-----
void opf_OPFClassifyingIncremental(Subgraph *sgtrain, Subgraph *sg)
{
  int i;

  // DEBUG VARIABLES
  //printf("Classifying %d new nodes...\n", sg->nnodes);
  int totalOfNewNodes = sg->nnodes;
  int totalOfSimple = 0;
  int totalOfRecheckProt = 0;
  int totalOfNewTree = 0;

  opf_OPFClassifying(sgtrain, sg); 

  //sortList(sgtrain);
  
  // Adding each new node
  for (i = 0; i < sg->nnodes; i++)
  {
    // Classify the current node - > it is killing OUR TIME! -> changed to classify all (above)
    //opf_OPFClassifyOneInstance(sgtrain, sg, i); 
  
    // Adds the new node to the subgraph structure
    AddSNode(sgtrain, &(sg->node[i]));
    // The new node is (sgtrain->node[sgtrain->nnodes-1])

    // Gets the index of both the new node and its predecessor (as found by OPF_Classifying)
    int newNode = sgtrain->nnodes-1;
    int predecessor = sgtrain->node[newNode].pred;

    //printf("\tCurrently classifying a node of label %d (%.2f,%.2f) [true: %d]. \n\tIts predecessor is label %d (%.2f,%.2f) [prototype: %d]\n",
     //sgtrain->node[newNode].label, sgtrain->node[newNode].feat[0], sgtrain->node[newNode].feat[1],
     //sgtrain->node[newNode].truelabel, sgtrain->node[predecessor].label,
     //sgtrain->node[predecessor].feat[0], sgtrain->node[predecessor].feat[1],
     //sgtrain->node[predecessor].pred == NIL);

    // Checks if the new node has been correctly classified
    if((sgtrain->node[newNode].label == sgtrain->node[newNode].truelabel)) {
      // Checks if its predecessor is not a prototype
      if (sgtrain->node[predecessor].pred != NIL){
        /*DEBUG*/
        //printf("\tThis node is simple! Running INSERT...\n");
        totalOfSimple++;

        // If so, simply updates the mininum tree
        
        int* oldNodes = (int*)calloc(sgtrain->nnodes, sizeof(int));
        int t = 0;

        INSERT(sgtrain, newNode, predecessor, &t, oldNodes);

        free(oldNodes);
      } else {
        /*DEBUG*/totalOfRecheckProt++;
        //printf("\tThis node is a recheck! Running recheck...\n");

        // If predecessor is a prototype, need to determine new prototype
        recheckPrototype(sgtrain, newNode, predecessor);
      }
    } else {
      // The node has not been classified correctly; 
      // as such, both the new node and its linked predecessor become prototypes

      /*DEBUG*/totalOfNewTree++;
     //printf("\tThis node is a new tree! Prototyping it and reconquering its pred...\n");
      
      // Updating the new node; simply a new prototype whose pair is its pred
      sgtrain->node[newNode].label = sgtrain->node[newNode].truelabel;
      sgtrain->node[newNode].pred = NIL;
      sgtrain->node[newNode].pathval = 0; 
      sgtrain->node[newNode].prototypePair = predecessor;

      // Updates the linked node; becomes a prototype and begin reconquest
      sgtrain->node[predecessor].prototypePair = newNode;
      sgtrain->node[predecessor].pred = NIL;
      sgtrain->node[predecessor].pathval = 0;
      reconquest(sgtrain, predecessor);
    }
    // Reordering the ordered list of nodes
    //printf("Correcting list...\n");
    correctList(sgtrain);
 }

  /*DEBUG*///printf("******* Total of Nodes: %d\n", totalOfNewNodes);
  /*DEBUG*///printf("******* Types: S: %d, RP: %d, NT: %d\n", totalOfSimple, totalOfRecheckProt, totalOfNewTree);
}

//DIFT classification function: adds samples from sg as prototypes to sgtrain-----
void opf_OPFClassifyingDIFT(Subgraph *sgtrain, Subgraph *sg)
{
  int i;

  // DEBUG VARIABLES
  //printf("Classifying %d new nodes...\n", sg->nnodes);
  int totalOfNewNodes = sg->nnodes;
  int totalOfSimple = 0;
  int totalOfRecheckProt = 0;
  int totalOfNewTree = 0;

  //opf_OPFClassifying(sgtrain, sg); 

  //sortList(sgtrain);
  
  // Adding each new node
  for (i = 0; i < sg->nnodes; i++)
  {
    // Classify the current node - > it is killing OUR TIME! -> changed to classify all (above)
    //opf_OPFClassifyOneInstance(sgtrain, sg, i); 
  
    // Adds the new node to the subgraph structure
    AddSNode(sgtrain, &(sg->node[i]));
    // The new node is (sgtrain->node[sgtrain->nnodes-1])

    // Gets the index of both the new node and its predecessor (as found by OPF_Classifying)
    int newNode = sgtrain->nnodes-1;

    // Updating the new node; simply a new, unpaired prototype
    sgtrain->node[newNode].label = sgtrain->node[newNode].truelabel;
    sgtrain->node[newNode].pred = NIL;
    sgtrain->node[newNode].pathval = 0; 
    sgtrain->node[newNode].prototypePair = NIL;

    // Reordering the ordered list of nodes
    //printf("Correcting list...\n");
    correctList(sgtrain);
 }

  /*DEBUG*///printf("******* Total of Nodes: %d\n", totalOfNewNodes);
  /*DEBUG*///printf("******* Types: S: %d, RP: %d, NT: %d\n", totalOfSimple, totalOfRecheckProt, totalOfNewTree);
}

/* Rechecking prototypes function
    checks if a new node is closer to its own prototype (becoming a new node)
    or if it is closer to the other prototype (becoming the new prototype) */
void recheckPrototype(Subgraph* sg, int newNode, int predecessor){
  // Clarification: 'inner prototype': prototype of newNode's tree
  //                'outer prototype': prototype of the closest distinct tree; pair of inner-prototype

  // If the inner prototype is alone (has no outer prototype), simply inserts
  if (sg->node[predecessor].prototypePair == NIL) {
    //insert();
    return;
  }
  // Measures the distance between newNode's prototype and its own pair
  float prototypesDistance = opf_ArcWeight(sg->node[sg->node[predecessor].prototypePair].feat, sg->node[predecessor].feat, sg->nfeats);
  // Measures the distance between newNode and its prototype's pair
  float newNodeDistance = opf_ArcWeight(sg->node[newNode].feat, sg->node[sg->node[predecessor].prototypePair].feat, sg->nfeats);

  // If newNode is closer to the outer prototype than the inner prototype, it becomes the new prototype
  if (newNodeDistance < prototypesDistance){
    // newNode becomes a prototype and begins reconquest
    sg->node[newNode].pred = NIL;
    sg->node[newNode].pathval = 0;

    //adds its former prototype as an adjacent and updates its pathval
    InsertSet(&(sg->node[newNode].incAdj), predecessor);
    sg->node[predecessor].pred = newNode;
    sg->node[predecessor].pathval = opf_ArcWeight(sg->node[predecessor].feat, sg->node[newNode].feat, sg->nfeats);

    //printf("\tThe new node is the new prototype! Begin the reconquest...");
    reconquest(sg, newNode);
  } else { // else, newNode simply gets inserted in the tree
    int* oldNodes = (int*)calloc(sg->nnodes, sizeof(int));
    int t = 0;

    //printf("\tThe new node is not a prototype! Begin INSERT...\n");
    INSERT(sg, newNode, predecessor, &t, oldNodes);

    free(oldNodes);
  }
}

/* Reconquest function
  begin reconquest with 'root' node as the new root */
void reconquest(Subgraph* sg, int root){
  // Flag vector
  int* oldNodes = (int*)calloc(sg->nnodes, sizeof(int));

  // Recursively calls the reconquest function for each adjacent node

  // Clones the list of adjacents
  Set* p = CloneSet(sg->node[root].incAdj);

  // Destroy adjacents, will be refilled as reconquest advances
  DestroySet(&(sg->node[root].incAdj));
  sg->node[root].incAdj = NULL;

  // Goes through all former adjacent nodes
  while(p != NULL){
    reconquest_recursive(sg, root, p->elem, oldNodes);
    p = p->next;
  }

  // Destroy the cloned set
  DestroySet(&p);

  // Frees the flag vector
  free(oldNodes);
}

/* Recursive reconquest: tries to reconquest all adjacents. If it does, calls reconquest on them */
void reconquest_recursive(Subgraph *sg, int predecessor, int node, int* oldNodes){
  // Tags the current node as 'old', i.e., already tried to reconquer it
  oldNodes[node] = 1;

   // Clones the list of adjacents
  Set* p = CloneSet(sg->node[node].incAdj);

  // Tries to reconquer the node (check if new pathval would be smaller than current pathval)
  float reconquerorPathval = opf_ArcWeight(sg->node[predecessor].feat, sg->node[node].feat, sg->nfeats) + sg->node[predecessor].pathval;
  if (sg->node[node].pathval >= reconquerorPathval){
    
    // Destroy adjacents, will be refilled as reconquest advances
    DestroySet(&(sg->node[node].incAdj));
    sg->node[node].incAdj = NULL;
    
    // Reconquer the node
    sg->node[node].pred = predecessor;
    sg->node[node].pathval = reconquerorPathval;
    InsertSet(&(sg->node[predecessor].incAdj),node);
    InsertSet(&(sg->node[node].incAdj),predecessor);

    // Tries to reconquer its neighbours
    while(p != NULL){
      if (!oldNodes[p->elem]){
        reconquest_recursive(sg, node, p->elem, oldNodes);
      }
      p = p->next;
    }
    DestroySet(&p);
  }

}

/* Insert function for Minimum Spanning Tree Update
   From Paper: "Algorithms for Updating Minimal Spanning Trees", CHIN, F., HOUCK, D.

   @param sg : subgraph to insert new node
   @param z  : new vertex
   @param r  : vertex to be connected to 'z' initially
   @param t  : variable to be the largest edge from w to z
   @param oldNodes: array with old(1)/new(0) labels for each vertex
                    *all vertices are marked 'new' in the beginning

   'm' is the largest edge between r and z
   'E_T' is the set of egdes inside MST
   'L(r)' is the adjacency list of 'r' node
*/
/* FIRST VERSION!!!!!   WARNING
*/
void INSERT(Subgraph* sg, int z, int r, int* t, int* oldNodes) {
    //printf("\t\tINSERT: z is %d, r is %d, t is %d\n", z, r, *t);
    oldNodes[r] = 1; // mark as 'old'
    int m = r; // aresta (r,z);
    int k;

    // for each vertex w adjacent to 'r'
    Set *w = sg->node[r].incAdj;
    while (w != NULL) {
    	// if adjacent node is new and of same label
    	if (!oldNodes[w->elem]) {
    		INSERT(sg, z, w->elem, t, oldNodes); // insert on w->elem
    		
    		double costWR = opf_ArcWeight(sg->node[w->elem].feat, sg->node[r].feat, sg->nfeats);
    		double costT  = opf_ArcWeight(sg->node[*t].feat, sg->node[z].feat, sg->nfeats);
    		double costK, costM;

    		// max edge (t, (w,r))
    		if (costT > costWR) {
    			// insert WR
       			InsertSet(&sg->node[r].incAdj, w->elem);
    		 	sg->node[r].pred = w->elem;
    			k = *t;
    			costK = opf_ArcWeight(sg->node[*t].feat, sg->node[z].feat, sg->nfeats);
    		} else {
    			// insert TZ
    	 		InsertSet(&sg->node[z].incAdj, *t);
    		 	sg->node[z].pred = *t;
    			k = w->elem;
    			costK = opf_ArcWeight(sg->node[w->elem].feat, sg->node[z].feat, sg->nfeats);
    		}	
    		costM = opf_ArcWeight(sg->node[m].feat, sg->node[z].feat, sg->nfeats);

    		if (costK < costM) m = k; // deixa a outra para avaliar depois
    	}
          w = w->next;
    }
    *t = m;
}

