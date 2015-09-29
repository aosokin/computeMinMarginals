nNodes=4;

%[Dp(0), Dp(1)] - unary terms
terminalWeights=[
    0,16;
    0,13;
    20,0;
    4,0
];

% [p, q, Vpq(0, 1), Vpq(1,0)] - pairwise terms
edgeWeights=[
    1,2,10,4;
    1,3,12,-1;
    2,3,-1,9;
    2,4,14,0;
    3,4,0,7
    ];

[energy, labels, minMarginals] = computeMinMarginals(terminalWeights,edgeWeights);

% % correct answer: 
% energy = 22; 
% labels = [0; 0; 1; 0];
% minMarginals = [22 29;22 29;24 22;22 25];