#ifndef _DIVERSITY_H_
#define _DIVERSITY_H_

#include "common.h"
#include "subgraph.h"

// Computes the diversity between classifiers, based on classified subgraphs of the same testing set
float computeDiversity(Subgraph **g, int number_of_classifiers, int diversityFunction);


#endif