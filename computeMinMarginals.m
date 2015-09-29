%	Matlab wrapper Kolmogorov's implementation of dynamic graph cuts 
%	that minimizes a binary function and computes its min-marginals if necessary
%
% 	Energy function:
% 	E(x)   =   \sum_p D_p(x_p)   +   \sum_pq V_pq(x_p,x_q)
% 	where x_p \in {0, 1},
% 	V_pq(kp, kq) = 0 if kp == kq,
% 	Vpq(0, 1), Vpq(1,0) must be non-negative.
% 
% 	Usage:
% 	[E] = computeMinMarginals(unaryTerms, pairwiseTerms);
% 	[E, labels] = computeMinMarginals(unaryTerms, pairwiseTerms);
% 	[E, labels, marginals] = computeMinMarginals(unaryTerms, pairwiseTerms);
%  
% 	Note that data types are crucials!
%  
% 	Inputs:
% 	unaryTerms - of type double, array size [numNodes, 2]; the cost of assigning 0, 1 to the corresponding unary term ([Dp(0), Dp(1)])
% 	pairwiseTerms - of type double, array size [numEdges, 4]; each line corresponds to an edge [p, q, Vpq(0, 1), Vpq(1,0)];
% 					p and q - indices of vertecies from 1,...,numNodes, p != q;
% 
% 	Outputs:
% 	E - of type double, a single number; optimal energy value
% 	labels - of type double, array size [numNodes, 1] of {0, 1}; optimal labeling; 
% 	marginals - of type double, array size [numNodes, 2]; marginals [E(x | xp = 0), E(x | xp = 1)]
%
%	by Anton Osokin, firstname.lastname@gmail.com, Summer 2011 
