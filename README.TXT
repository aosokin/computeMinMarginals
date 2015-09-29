
This software implements the MATLAB mex-wrapper for Boykov-Kolmogorov max-flow algorithm.
This version allows to compute min-marginals using dynamic cuts.
CAUTION! the interface sligthly differs from the one in graphCut_BK and graphCut_IBFS

Anton Osokin, (firstname.lastname@gmail.com)
11.08.2013

Please cite the following papers in any resulting publication:

Yuri Boykov and Vladimir Kolmogorov, 'An Experimental Comparison of Min-Cut/Max-Flow Algorithms for
Energy Minimization in Vision', IEEE Transactions on Pattern Analysis and Machine Intelligence, vol.
26, no. 9, pp. 1124-1137, Sept. 2004.
   
Dynamic cut approach was proposed in:
Pushmeet Kohli, Philip Torr 
Efficiently Solving Dynamic Markov Random Fields Using Graph Cuts.(Oral Presentation)
In: ICCV 2005.
  
Using dynamic cuts to compute min-marginals was proposed in:
Pushmeet Kohli, Philip Torr 
Measuring Uncertainty in Graph Cut Solutions.
In: CVIU 2008.


PACKAGE
-----------------------------

./computeMinMarginals.cpp - the C++ code of the wrapper

./buildComputeMinMarginals.m - function to build the wrapper

./computeMinMarginals.m - the description of the implemented function

./example_computeMinMarginals.m - the example of usage

./maxflow-v3.02.src - C++ code by Vladimir Kolmogorov (the code is used as is)
http://pub.ist.ac.at/~vnk/software/maxflow-v3.02.src.tar.gz

./computeMinMarginals.mexw64 - binary file for the mex-function compiled using MATLAB R2013a + MSVC 2012

USING THE CODE
-----------------------------

0) Install MATLAB and one of the supported compilers

1) Run buildComputeMinMarginals.m

2) Run eexample_computeMinMarginals.m to check if the code works

The code was tested using MSVC 2012 and MATLAB 2013a

