/*
  Copyright (C) <2009> <Alexandre Xavier Falcão and João Paulo Papa>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  please see full copyright in COPYING file.
  -------------------------------------------------------------------------
  written by A.X. Falcão <afalcao@ic.unicamp.br> and by J.P. Papa
  <papa.joaopaulo@gmail.com>, Oct 20th 2008

  This program is a collection of functions to manage the Optimum-Path Forest (OPF)
  classifier.*/

#include "subgraph.h"

/*----------- Constructor and destructor ------------------------*/
// Allocate nodes without features
Subgraph *CreateSubgraph(int nnodes){
  Subgraph *sg=(Subgraph *)calloc(1,sizeof(Subgraph));
  int i;

  sg->nnodes = nnodes;
  sg->node   = (SNode *)calloc(nnodes,sizeof(SNode));
  sg->ordered_list_of_nodes = (int *) calloc(nnodes,sizeof(int));

  if (sg->node == NULL){
    Error("Cannot allocate nodes","CreateSubgraph");
  }

  for (i=0; i < sg->nnodes; i++){
    sg->node[i].feat   = NULL;
	sg->node[i].relevant = 0;
  }

  return(sg);
}

// Deallocate memory for subgraph
void DestroySubgraph(Subgraph **sg){
    int i;

    if ((*sg)!=NULL)
    {
        for (i=0; i < (*sg)->nnodes; i++)
        {
            if ((*sg)->node[i].feat != NULL)
                free((*sg)->node[i].feat);
            if ((*sg)->node[i].adj != NULL)
                DestroySet(&(*sg)->node[i].adj);
        }
        free((*sg)->node);
        free((*sg)->ordered_list_of_nodes);
        free((*sg));
        *sg = NULL;
    }
}

//write subgraph to disk
void WriteSubgraphPrototype(Subgraph *g, char *file){
  FILE *fp = NULL;
  int i, j;

  if(g != NULL){
    fp = fopen(file, "wb");
    fwrite(&g->nnodes, sizeof(int), 1, fp);
    fwrite(&g->nlabels, sizeof(int), 1, fp);
    fwrite(&g->nfeats, sizeof(int), 1, fp);

    /*writing position(id), label and features*/
    for (i = 0; i < g->nnodes; i++){
      fwrite(&g->node[i].position, sizeof(int), 1, fp);
      fwrite(&g->node[i].truelabel, sizeof(int), 1, fp);
      fwrite(&g->node[i].pred, sizeof(int), 1, fp);
      for (j = 0; j < g->nfeats; j++) {
  fwrite(&g->node[i].feat[j], sizeof(float), 1, fp);
      }
    }
    fclose(fp);
  }
}

//write subgraph to disk
void WriteSubgraph(Subgraph *g, char *file){
  FILE *fp = NULL;
  int i, j;

  if(g != NULL){
    fp = fopen(file, "wb");
    fwrite(&g->nnodes, sizeof(int), 1, fp);
    fwrite(&g->nlabels, sizeof(int), 1, fp);
    fwrite(&g->nfeats, sizeof(int), 1, fp);

    /*writing position(id), label and features*/
    for (i = 0; i < g->nnodes; i++){
      fwrite(&g->node[i].position, sizeof(int), 1, fp);
      fwrite(&g->node[i].truelabel, sizeof(int), 1, fp);
      for (j = 0; j < g->nfeats; j++)
	     fwrite(&g->node[i].feat[j], sizeof(float), 1, fp);
      
    }

    fclose(fp);
  }
}

//read subgraph from opf format file
Subgraph *ReadSubgraph(char *file){
  Subgraph *g = NULL;
  FILE *fp = NULL;
  int nnodes, i, j;
  char msg[256];

  if((fp = fopen(file, "rb")) == NULL){
    sprintf(msg, "%s%s", "Unable to open file ", file);
    Error(msg,"ReadSubGraph");
  }

  /*reading # of nodes, classes and feats*/
  fread(&nnodes, sizeof(int), 1, fp);
  g = CreateSubgraph(nnodes);
  fread(&g->nlabels, sizeof(int), 1, fp);
  fread(&g->nfeats, sizeof(int), 1, fp);

  /*reading features*/
  for (i = 0; i < g->nnodes; i++){
    g->node[i].feat = AllocFloatArray(g->nfeats);
    fread(&g->node[i].position, sizeof(int), 1, fp);
    fread(&g->node[i].truelabel, sizeof(int), 1, fp);
    for (j = 0; j < g->nfeats; j++)
      fread(&g->node[i].feat[j], sizeof(float), 1, fp);

    
    
   }

  fclose(fp);

  return g;
}

// Copy subgraph (does not copy Arcs)
Subgraph *CopySubgraph(Subgraph *g){
  Subgraph *clone = NULL;
  int i;

  if(g != NULL){
    clone = CreateSubgraph(g->nnodes);

    clone->bestk = g->bestk;
    clone->df = g->df;
    clone->nlabels = g->nlabels;
    clone->nfeats = g->nfeats;
    clone->mindens = g->mindens;
    clone->maxdens = g->maxdens;
    clone->K = g->K;

    for(i=0; i< g->nnodes; i++){
      CopySNode(&clone->node[i], &g->node[i], g->nfeats);
      clone->ordered_list_of_nodes[i] = g->ordered_list_of_nodes[i];
    }

    return clone;
  }else return NULL;
}

//Copy nodes
void CopySNode(SNode *dest, SNode *src, int nfeats){
	dest->feat = AllocFloatArray(nfeats);
	memcpy(dest->feat, src->feat, nfeats*sizeof(float));
	dest->pathval = src->pathval;
	dest->dens = src->dens;
	dest->label  = src->label;
	dest->root = src->root;
	dest->pred  = src->pred;
	dest->truelabel = src->truelabel;
	dest->position = src->position;
	dest->status = src->status;
	dest->relevant = src->relevant;
	dest->radius = src->radius;
  dest->nplatadj = src->nplatadj;

  dest->prototypePair = src->prototypePair;

    dest->adj = CloneSet(src->adj);
    dest->incAdj = CloneSet(src->incAdj);
}


//Swap nodes
void SwapSNode(SNode *a, SNode *b){
	SNode tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}


/*----------- OPF-Incremental Code ------------------------------*/
//Adds a new node
void AddSNode(Subgraph *sg, SNode *newSNode){
  //Grows the node vector
  (sg->nnodes)++;
  sg->node   = (SNode *)realloc(sg->node, sg->nnodes*sizeof(SNode));
  sg->ordered_list_of_nodes = (int *) realloc(sg->ordered_list_of_nodes, sg->nnodes*sizeof(int));

  //Copies the node to the end of the node vector
  SNode *tempNode = (SNode *) malloc(sizeof(SNode));
  CopySNode(tempNode, newSNode, sg->nfeats);
  sg->node[sg->nnodes-1] = *tempNode;
}


// Auxiliary function for sortList: sort indexes by associated pathval
int comparePathval(const void* a, const void* b){
  if (((IndexPathPair*)a)->pathval < ((IndexPathPair*)b)->pathval) return -1;
  else if (((IndexPathPair*)b)->pathval < ((IndexPathPair*)a)->pathval) return 1;
  else return 0;
}

// Corrects the list of pathvalues
void correctList(Subgraph *sg){
  IndexPathPair* npp = (IndexPathPair*)malloc(sg->nnodes * sizeof(IndexPathPair));

  int i;
  for (i=0; i < sg->nnodes-1; i++){
    npp[i].index = sg->ordered_list_of_nodes[i];
    npp[i].pathval = sg->node[sg->ordered_list_of_nodes[i]].pathval;
  }
  npp[i].index = i;
  npp[i].pathval = sg->node[i].pathval;

  qsort(npp, sg->nnodes, sizeof(IndexPathPair), comparePathval);

  /*int ckey = npp[sg->nnodes-1].pathval;
  int cind = npp[sg->nnodes-1].index;

  i = sg->nnodes-2;

  while ((i >= 0) && (npp[i].pathval > ckey)) {
    npp[i+1].pathval = npp[i].pathval;
    npp[i+1].index   = npp[i].index;
    i--;
  }
  npp[i+1].pathval = ckey;
  npp[i+1].index = cind;*/

  for (i=0; i < sg->nnodes; i++){
    sg->ordered_list_of_nodes[i] = npp[i].index;
  }

  free(npp);
}

// Ordena a lista com qsort
void sortList(Subgraph *sg){
  IndexPathPair* npp = (IndexPathPair*)malloc(sg->nnodes * sizeof(IndexPathPair));

  int i;
  for (i=0; i < sg->nnodes; i++){
    npp[i].index = i;
    npp[i].pathval = sg->node[i].pathval;
  }

  qsort(npp, sg->nnodes, sizeof(IndexPathPair), comparePathval);

  for (i=0; i < sg->nnodes; i++){
    sg->ordered_list_of_nodes[i] = npp[i].index;
    if (npp[i+1].pathval < npp[i].pathval) printf("\n*****mal-ordenado em %d! [%d]: %ff; [%d]: %f\n", i, i,npp[i].pathval,i+1, npp[i+1].pathval); 
      else printf("\n*****ordenado em %d! [%d]: %ff; [%d]: %f\n", i, i,npp[i].pathval,i+1, npp[i+1].pathval);
    
  }

  free(npp);
}

// # of prototypes in subgraph
int prototypeCount(Subgraph *g){
  int i;

  int count = 0;
  if(g != NULL){

    for(i=0; i< g->nnodes; i++){
      if (g->node[i].pred == NIL) count++;
    }
  }
  return count;
}


