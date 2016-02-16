#ifndef _OPF_GRAPH_H_
#define _OPF_GRAPH_H_

#include <cstdlib>
#include <iostream>
#include <vector>
#include "common.h"

using namespace std;

// Typedefining the distance function
typedef double(* distanceFunctionPointer)(OPFNode& a, OPFNode& b);

// Class that defines a single node in an OPF_Graph.
class OPFNode {
private:
	double pathValue;			// total value of the path to root node

	int label;					// current label of the node (might be inaccurate)
	int truelabel;				// true label of the node, if it is known

	OPFNode* root;				// pointer to this node's tree's root node

	vector<double> feats;		// feature vector

	OPFNode* prototypePair;		// if this node is a prototype, this is a pointer to its pair

	OPFNode* predecessor;		// this node's predecessor
	/*TODO: adjacency lists*/

public:
	// Empty constructor
	OPFNode(){
		this.pathValue = NIL;
		this.label = this.truelabel = NIL;
		this.root = NULL;
		this.prototypePair = NULL;
		this.predecessor = NULL;
	}
	// Feature vector constructor
	OPFNode(vector<double> feats){
		this.feats = feats;

		this.pathValue = NIL;
		this.label = this.truelabel = NIL;
		this.root = NULL;
		this.prototypePair = NULL;
		this.predecessor = NULL;
	}
	~OPFNode(){}

	// Overloading copy operator
	OPFNode& operator=(const OPFNode& other) // copy assignment
	{
		if (this != &other) { // self-assignment check expected
			this.feats = other.feats;

			this.pathValue = other.pathValue;
			this.label = other.label;
			this.truelabel = other.truelabel;
			this.root = other.root;
			this.prototypePair = other.prototypePair;
			this.predecessor = other.predecessor;
		}
		return *this;
	}

	// Getters and setters
	double getPathValue(){ return this.pathValue; }
	void setPathValue(double pathValue){ this.pathValue = pathValue; }

	int getLabel(){ return this.label; }
	void setLabel(int label){ this.label = label; }
	int getTrueLabel(){ return this.truelabel; }
	void setTrueLabel(int truelabel){ this.truelabel = truelabel; }

	int getIndex(){ return this.index; }
	void setIndex(int index){ this.index = index; }

	OPFNode* getRoot(){ return this.root; }
	void setRoot(OPFNode* root){ this.root = root; }

	bool isPrototype() { return (this.root == this); }

	vector<double> getFeats(){ return this.feats; }
	double getFeat(int i){ return this.feats[i]; }
	void setFeats(vector<double> feats){ this.feats = feats; }
	void addFeat(double feat){ this.feats.push_back(feat); }

	int getDimension() { return this.feats.size(); }

	OPFNode* getPrototypePair(){ return this.prototypePair; }
	void setPrototypePair(OPFNode* prototypePair){ this.prototypePair = prototypePair; }

	OPFNode* getPredecessor(){ return this.predecessor; }
	void setPredecessor(OPFNode* predecessor){ this.predecessor = predecessor; }

	/*TODO: adjacency lists*/
};

// Class that defines a graph for OPF algorithms
class OPFGraph
{
private:
	vector<OPFNode> nodes;					// Nodes in this graph

	int classes;							// Number of classes (possible types of label)

	int dimension;							// Number of features (dimension) per node. 
	// Currently, all nodes must have the same dimension.

	vector<OPFNode*> orderedListOfNodes;	// Nodes ordered by path-value, for speeding up supervised learning.

	// Distance function to be used in this graph, as defined in distances.h
	distanceFunctionPointer distanceFunction;

public:
	// Standard constructor. Takes possibly a number of classes.
	// Takes possibly a dimension value and a distanceFunction, defaulted to euclidean
	OPFGraph(int classes = 0, int dimension = 0, distanceFunctionPointer distanceFunction = euclideanDistance){
		this.classes = classes;
		this.distanceFunction = distanceFunction;
		this.dimension = dimension;
	}
	~OPFGraph(){}

	// Getters and setters	
	vector<OPFNode> getNodes(){ return this.nodes; }
	OPFNode getNode(int i){ return this.nodes[i]; }
	void setNodes(vector<OPFNode> nodes){ this.nodes = nodes; }
	void updateNode(OPFNode node, int i){ this.nodes[i] = node; }
	void addNode(OPFNode node){ this.nodes.push_back(node); }

	int getNumberOfNodes(){ return this.nodes.size(); }

	int getDimension(){ return this.dimension; }
	void setDimension(int dimension){ this.dimension = dimension; }

	int getClasses(){ return this.classes; }
	void setClasses(int classes){ this.classes = classes; }

	vector<OPFNode*> getOrderedListOfNodes(){ return this.orderedListOfNodes; }
	OPFNode* getOrderedListOfNodes(int i){ return this.orderedListOfNodes[i]; }
	void setOrderedListOfNodes(vector<OPFNode*> orderedListOfNodes){ this.orderedListOfNodes = orderedListOfNodes; }
	void updateOrderedListOfNodes(OPFNode* node, int i){ this.orderedListOfNodes[i] = node; }
	void addOrderedListOfNodes(OPFNode* node){ this.orderedListOfNodes.push_back(node); }

	// Distance function to be used in this graph
	double distance(OPFNode& a, OPFNode& b){
		return distanceFunction(a,b);
	}

	/*TODO:I/O*/

	// Overloading copy operator
	OPFGraph& operator=(const OPFGraph& other) // copy assignment
	{
		if (this != &other) { // self-assignment check expected
			this.nodes = other.nodes;

			this.classes = other.classes;
			this.dimension = other.dimension;
			this.orderedListOfNodes = other.orderedListOfNodes;
			this.distanceFunction = other.distanceFunction;
		}
		return *this;
	}

	// Correcting the list of ordered values
	void correctListOfOrderedValues(){
		/*TODO: really the best way to do this?*/
	}
};

#endif