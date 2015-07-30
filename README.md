[![Build Status](https://travis-ci.org/star-bnl/star-vertex-eval.svg?branch=master)](https://travis-ci.org/star-bnl/star-vertex-eval)

# Study the ranking scheme in STAR

1.- The macro VertexRanking.C will read the MuDst sample and create a tree with some vertex information. To run (e.g. 100 events and output file outvertex.root):

    $root -l -b -q lMuDst.C 'VertexRank.C+(100, "path/to/*.MuDst.root", "outvertex")'
    
2.- Run TMVA to classify signal and background. A separate set of signal and background must be given, with a define set of variables for the training. Also need to specify the classifier. To run (e.g. with classifiers Fisher and Likelihood):

    $root TMVAClassification.C\(\"Fisher,Likelihood\"\)
    
3.- Apply the results of the classification with ApplyClassifier.C. The output will be the same tree for step (1) but with a new branch with the classifier response. Run:

    $root ApplyClassifier.C
