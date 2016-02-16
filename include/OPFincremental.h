#ifndef _OPF_I_H_
#define _OPF_I_H_

#include "OPF.h"

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>

#include "common.h"
#include "set.h"
#include "gqueue.h"
#include "subgraph.h"
#include "sgctree.h"
#include "realheap.h"

/*----------- OPF-Incremental Code ------------------------------*/
void opf_OPFClassifyingIncremental(Subgraph *sgtrain, Subgraph *sg); //Incremental classification function: it  classifies labeled samples from sg and adds them to sgtrain
void opf_OPFClassifyingDIFT(Subgraph *sgtrain, Subgraph *sg);
void INSERT(Subgraph* sgtrain, int r, int z, int* t, int* oldNodes);
void updatePathvals(Subgraph* sgtrain, int* taggedNodes, int i);
void recheckPrototype(Subgraph* sg, int newNode, int predecessor);
void reconquest(Subgraph* sg, int root);
void reconquest_recursive(Subgraph *sg, int predecessor, int node, int* oldNodes);


#endif