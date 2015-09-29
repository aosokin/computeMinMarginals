function buildComputeMinMarginals
% script to build computeMinMarginals
codePath = 'maxflow-v3.02.src';

srcFiles = { 'computeMinMarginals.cpp', ...
            fullfile(codePath, 'maxflow.cpp'), ...
            fullfile(codePath, 'graph.cpp') };
allFiles = '';
for iFile = 1 : length(srcFiles)
    allFiles = [allFiles, ' ', srcFiles{iFile}];
end

cmdLine = ['mex ', allFiles, ' -output computeMinMarginals -largeArrayDims '];
eval(cmdLine);

