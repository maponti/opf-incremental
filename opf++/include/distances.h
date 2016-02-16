#ifndef _DISTANCES_H_
#define _DISTANCES_H_

#include "OPFGraph.h"
#include <cmath>

// Euclidean distance function
double euclideanDistance(OPFNode& a, OPFNode& b){
	av = a.getFeats(); bv = b.getFeats();	// Getting the feature vectors

	if (av.size() != bv.size()) { return NIL; } // Check if same dimension

	double distance = 0.0;	// Computing the distance
	for (int i = 0; i < av.size(); ++i)	{
		distance += (av[i]-bv[i])*(av[i]-bv[i]);
	}

	return sqrt(distance);
}

#endif