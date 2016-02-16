/*
  

  This program is a collection of functions to manage the Optimum-Path Forest (OPF)
  classifier.*/

#include "LibOPF.h"
#include <cfloat>

/*********************************
 * Supervised training functions *
 *********************************/
class opfSupervised
{
public:
	/* Training function
		* parameter - OPFGraph& graph: the untrained graph to be trained. The graph will be modified. */
	static void train(OPFGraph& graph){

		// Computing the minimum spanning tree,
		// along with the prototypes.
		opfSupervised.computeMSTPrototypes(graph);
	}

	/* Computes the minimum spanning tree of the graph.
	   Then, cuts off all cross-class edges (i.e.: where the 
	   two vertexes are of different classes) and makes them
	   prototypes. This is the first part of supervised training.
	   	* parameter - OPFGraph& graph: the untrained graph to be adapted. */
	static void computeMSTPrototypes(OPFGraph& graph){
		// Defining a heap
		/*TODO: the heap*/

		for (std::vector<OPFNode>::iterator i = graph.nodes.begin(); i != graph.nodes.end(); ++i)
			i->pathValue = FLT_MAX;
		
		
	}
};