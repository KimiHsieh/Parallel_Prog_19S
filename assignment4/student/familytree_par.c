#include "familytree.h"
int parallel_traverse(tree *node) { 
	if (node == NULL) return 0;
	int father_iq, mother_iq;
	#pragma omp task 
	father_iq = parallel_traverse(node->father); 	    
	mother_iq = parallel_traverse(node->mother);
		    
    node->IQ = compute_IQ(node->data, father_iq, mother_iq);
    genius[node->id] = node->IQ;
	return node->IQ;
}
int traverse(tree *node, int numThreads){
	
	// TODO implement your solution in here.
	
    #pragma omp parallel num_threads( numThreads )
	{
		#pragma omp single 
		{	
		    node->IQ = parallel_traverse(node);
	    }
	}

    return node->IQ;
}

