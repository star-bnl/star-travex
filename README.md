[![Build Status](https://travis-ci.org/star-bnl/star-vertex-eval.svg?branch=master)](https://travis-ci.org/star-bnl/star-vertex-eval)


How to build and use the library in ROOT session
================================================

Checkout the code using one of the following commands:

    git clone https://github.com/star-bnl/star-vertex-eval.git   # If you do not have an account on github.com
    git clone git@github.com:star-bnl/star-vertex-eval.git       # otherwise.

Compile and build the library:

    cd star-vertex-eval/
    mkdir build && cd build/
    cmake -D CMAKE_CXX_FLAGS="-m32" ../
    make

The above commands should build `libstar-vertex-eval.so` in the `build`
directory. Now we can load it in ROOT and call `MuMcPrVKFV2012()` to create a
ROOT file (e.g. `my_output_file.root`) with support histograms used to calculate
the vertex finding efficiency. To produce and the efficiency plots one can
follow up with a call to `VxEff()` as in the snipped below:

    root -l ../load_libs.C
    root [0] MuMcPrVKFV2012(<num_events>, "path/to/*.MuDst.root", "my_output_file.root");
    root [1] std::vector<std::string> myKeys, myFiles; myKeys.push_back("DFLT"); myFiles.push_back("my_output_file.root");
    root [2] VxEff(myKeys, myFiles);


Study the ranking scheme in STAR
================================

1.- The macro VertexRanking.C will read the MuDst sample and create a tree with some vertex information. To run (e.g. 100 events and output file outvertex.root):

    $root -l -b -q lMuDst.C 'VertexRank.C(100, "path/to/*.MuDst.root", "outvertex")'

2.- Run TMVA to classify signal and background. A separate set of signal and background must be given, with a define set of variables for the training. Also need to specify the classifier. To run (e.g. with classifiers Fisher and Likelihood):

    $root TMVAClassification.C\(\"Fisher,Likelihood\"\)

3.- Apply the results of the classification with ApplyClassifier.C. The output will be the same tree for step (1) but with a new branch with the classifier response. Run:

    $root ApplyClassifier.C
