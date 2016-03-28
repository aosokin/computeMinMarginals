

#include "maxflow-v3.02.src/graph.h"
#include "mex.h"

#include <limits>
#include <cmath>

#define INFTY INT_MAX

//define types
typedef double EnergyType;
mxClassID MATLAB_ENERGYTERM_TYPE = mxDOUBLE_CLASS;

typedef double EnergyTermType;
mxClassID MATLAB_ENERGY_TYPE = mxDOUBLE_CLASS;

typedef double LabelType;
mxClassID MATLAB_LABEL_TYPE = mxDOUBLE_CLASS;
/*
typedef int EnergyType;
mxClassID MATLAB_ENERGYTERM_TYPE = mxINT32_CLASS;

typedef int EnergyTermType;
mxClassID MATLAB_ENERGY_TYPE = mxINT32_CLASS;

typedef int LabelType;
mxClassID MATLAB_LABEL_TYPE = mxINT32_CLASS;
*/

typedef Graph<EnergyTermType,EnergyTermType,EnergyType> GraphType; 


int roundToInt(double a);
int isInteger(double a);

#define MATLAB_ASSERT(expr,msg) if (!(expr)) { mexErrMsgTxt(msg);}


void mexFunction(int nlhs, mxArray *plhs[], 
    int nrhs, const mxArray *prhs[])
{
	MATLAB_ASSERT( nrhs == 2, "computeMinMarginals: Wrong number of input parameters: expected 2");
    MATLAB_ASSERT( nlhs <= 3, "computeMinMarginals: Too many output arguments: expected 3 or less");
	
	//Fix input parameter order:
	const mxArray *uInPtr = (nrhs >= 1) ? prhs[0] : NULL; //unary
	const mxArray *pInPtr = (nrhs >= 2) ? prhs[1] : NULL; //pairwise
	
	//Fix output parameter order:
	mxArray **eOutPtr = (nlhs >= 1) ? &plhs[0] : NULL; //energy
	mxArray **lOutPtr = (nlhs >= 2) ? &plhs[1] : NULL; //labels
	mxArray **mOutPtr = (nlhs >= 3) ? &plhs[2] : NULL; //min-marginals

	//node number
	int numNodes;
    
	// get unary potentials
	MATLAB_ASSERT(mxGetNumberOfDimensions(uInPtr) == 2, "computeMinMarginals: The first paramater is not 2-dimensional");
	MATLAB_ASSERT(mxGetClassID(uInPtr) == MATLAB_ENERGYTERM_TYPE, "computeMinMarginals: Unary potentials are of wrong type");
	MATLAB_ASSERT(mxGetPi(uInPtr) == NULL, "computeMinMarginals: Unary potentials should not be complex");
	
	numNodes = mxGetM(uInPtr);
	
	MATLAB_ASSERT(numNodes >= 1, "computeMinMarginals: The number of nodes is not positive");
	MATLAB_ASSERT(mxGetN(uInPtr) == 2, "computeMinMarginals: The first paramater is not of size #nodes x 2");
	
	EnergyTermType* termW = (EnergyTermType*)mxGetData(uInPtr);

	//get pairwise potentials
	MATLAB_ASSERT(mxGetNumberOfDimensions(pInPtr) == 2, "computeMinMarginals: The second paramater is not 2-dimensional");
	
	mwSize numEdges = mxGetM(pInPtr);

	MATLAB_ASSERT( mxGetN(pInPtr) == 4, "computeMinMarginals: The second paramater is not of size #edges x 4");
	MATLAB_ASSERT(mxGetClassID(pInPtr) == MATLAB_ENERGYTERM_TYPE, "computeMinMarginals: Pairwise potentials are of wrong type");

	EnergyTermType* edges = (EnergyTermType*)mxGetData(pInPtr);
	for(int i = 0; i < numEdges; i++)
	{
		MATLAB_ASSERT(1 <= roundToInt(edges[i]) && roundToInt(edges[i]) <= numNodes, "computeMinMarginals: error in pairwise terms array: wrong vertex index");
		MATLAB_ASSERT(isInteger(edges[i]), "computeMinMarginals: error in pairwise terms array: wrong vertex index");
		MATLAB_ASSERT(1 <= roundToInt(edges[i + numEdges]) && roundToInt(edges[i + numEdges]) <= numNodes, "computeMinMarginals: error in pairwise terms array: wrong vertex index");
		MATLAB_ASSERT(isInteger(edges[i + numEdges]), "computeMinMarginals: error in pairwise terms array: wrong vertex index");
		MATLAB_ASSERT(edges[i + 2 * numEdges] + edges[i + 3 * numEdges] >= 0, "computeMinMarginals: error in pairwise terms array: nonsubmodular edge");
	}


	// start computing
	if (nlhs == 0){
		return;
	}

	//prepare graph
	GraphType *g = new GraphType( numNodes, numEdges); 
	
	for(int i = 0; i < numNodes; i++)
	{
		g -> add_node(); 
		g -> add_tweights( i, termW[numNodes + i], termW[i]); 
	}
	
	for(int i = 0; i < numEdges; i++)
		if(edges[i] < 1 || edges[i] > numNodes || edges[numEdges + i] < 1 || edges[numEdges + i] > numNodes || edges[i] == edges[numEdges + i] || !isInteger(edges[i]) || !isInteger(edges[numEdges + i])){
			mexWarnMsgIdAndTxt("computeMinMarginals:pairwisePotentials", "Some edge has invalid vertex numbers and therefore it is ignored");
		}
		else
			if(edges[2 * numEdges + i] + edges[3 * numEdges + i] < 0){
				mexWarnMsgIdAndTxt("computeMinMarginals:pairwisePotentials", "Some edge is non-submodular and therefore it is ignored");
			}
			else
			{
				if (edges[2 * numEdges + i] >= 0 && edges[3 * numEdges + i] >= 0)
					g -> add_edge((GraphType::node_id)roundToInt(edges[i] - 1), (GraphType::node_id)roundToInt(edges[numEdges + i] - 1), edges[2 * numEdges + i], edges[3 * numEdges + i]);
				else
					if (edges[2 * numEdges + i] <= 0 && edges[3 * numEdges + i] >= 0)
					{
						g -> add_edge((GraphType::node_id)roundToInt(edges[i] - 1), (GraphType::node_id)roundToInt(edges[numEdges + i] - 1), 0, edges[3 * numEdges + i] + edges[2 * numEdges + i]);
						g -> add_tweights((GraphType::node_id)roundToInt(edges[i] - 1), 0, edges[2 * numEdges + i]); 
						g -> add_tweights((GraphType::node_id)roundToInt(edges[numEdges + i] - 1),0 , -edges[2 * numEdges + i]); 
					}
					else
						if (edges[2 * numEdges + i] >= 0 && edges[3 * numEdges + i] <= 0)
						{
							g -> add_edge((GraphType::node_id)roundToInt(edges[i] - 1), (GraphType::node_id)roundToInt(edges[numEdges + i] - 1), edges[3 * numEdges + i] + edges[2 * numEdges + i], 0);
							g -> add_tweights((GraphType::node_id)roundToInt(edges[i] - 1),0 , -edges[3 * numEdges + i]); 
							g -> add_tweights((GraphType::node_id)roundToInt(edges[numEdges + i] - 1), 0, edges[3 * numEdges + i]); 
						}
						else
							mexWarnMsgIdAndTxt("computeMinMarginals:pairwisePotentials", "Something strange with an edge and therefore it is ignored");
			}

	//compute flow
	EnergyType flow = g -> maxflow();

	//output minimum value
	if (eOutPtr != NULL){
		*eOutPtr = mxCreateNumericMatrix(1, 1, MATLAB_ENERGY_TYPE, mxREAL);
		*(EnergyType*)mxGetData(*eOutPtr) = (EnergyType)flow;
	}

	LabelType* segment = NULL;
	//output minimum cut
	if (lOutPtr != NULL){
		*lOutPtr = mxCreateNumericMatrix(numNodes, 1, MATLAB_LABEL_TYPE, mxREAL);
		segment = (LabelType*)mxGetData(*lOutPtr);
		for(int i = 0; i < numNodes; i++)
			segment[i] = g -> what_segment(i);
	}
	
	//output marginals
	if (mOutPtr != NULL && lOutPtr != NULL){
		*mOutPtr = mxCreateNumericMatrix(numNodes, 2, MATLAB_ENERGY_TYPE, mxREAL);
		EnergyType* marginals = (EnergyType*)mxGetData(plhs[2]);

		for(int i = 0; i < numNodes; i++)
		if (segment[i] == 0){
			marginals[i] = flow;
			//g -> add_tweights(i, termW[i + numNodes], INFTY);
			g -> add_tweights(i, 0, INFTY - termW[i]);
			g -> mark_node(i);
			
			marginals[i + numNodes] = g -> maxflow(true);

			g -> add_tweights(i, 0, -INFTY + termW[i]);
			g -> mark_node(i);
		} else {
			marginals[i + numNodes] = flow;

			g -> add_tweights(i, INFTY - termW[i], 0);
			g -> mark_node(i);

			marginals[i] = g -> maxflow(true);
			
			g -> add_tweights(i, -INFTY + termW[i], 0);
			g -> mark_node(i);
		}
	}
	delete g;
}

int roundToInt(double a)
{
	return floor(a + 0.5);
}

int isInteger(double a)
{
	return (a - roundToInt(a) < 1e-6);
}
