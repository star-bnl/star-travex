[![Build Status](https://travis-ci.org/star-bnl/star-vertex-eval.svg?branch=master)](https://travis-ci.org/star-bnl/star-vertex-eval)


How to build and use the library in a ROOT session
==================================================

Checkout the code using the following command:

    git clone --recursive https://github.com/star-bnl/star-vertex-eval.git

Compile and build the library:

    cd star-vertex-eval/
    mkdir build && cd build/
    cmake -D CMAKE_INSTALL_PREFIX=./ ../
    make install

The above commands should build `libstar-vertex-eval.so` in the `build`
directory. Now we can load it in ROOT and call `MuMcPrVKFV2012()` to create a
ROOT file (e.g. `my_output_file.root`) with support histograms used to calculate
the vertex finding efficiency. To produce and the efficiency plots one can
follow up with a call to `VxEff()` as in the snipped below:

    root -l ../load_libs.C
    root [0] MuMcPrVKFV2012(<num_events>, "path/to/*.MuDst.root", "my_output_file.root");
    root [1] std::vector<std::string> myKeys, myFiles; myKeys.push_back("DFLT"); myFiles.push_back("my_output_file.root");
    root [2] VxEff(myKeys, myFiles);

To create histograms from muDst files one can do from the same location:

    root -l ../load_libs.C
    root [0] VertexRank.C(<num_events>, "path/to/*.MuDst.root", "my_output_file")


How to create embedding samples
===============================

Checkout the code using the following command:

    git clone --recursive https://github.com/star-bnl/star-vertex-eval.git

Select desired zerobias files from the database:

    get_file_list.pl -keys path,filename,events -cond 'filename~st_zerobias_adc_1412%,year=2013,runnumber>14125000' -delim / -limit 20

Create a text file with a list of zerobias data files in the following format
(`path/to/file` `run_id` `num_of_events`), e.g.:

    $cat supple/filelist_zerobias.txt
    /star/data05/daq/2013/embedding/st_zerobias_adc_14150005_raw_3330002.daq 14150005 103
    /star/data05/daq/2013/embedding/st_zerobias_adc_14150004_raw_1530002.daq 14150004 103
	 ...
    /star/data05/daq/2013/embedding/st_zerobias_adc_14150005_raw_8660002.daq 14150005 102
	 ...


W boson
-------

Create a directory from where condor jobs will be submitted and run the script
as:

    mkdir submit_wbos && cd submit_wbos
    /path/to/star-vertex-eval/supple/submit_jobs_embedding_wbos.sh


J/psi
-----

Create a directory from where condor jobs will be submitted and run the script
as:

    mkdir submit_jpsi && cd submit_jpsi
    /path/to/star-vertex-eval/supple/submit_jobs_embedding_jpsi.sh 200

If the package was not checked out in the default location of
${HOME}/star-vertex-eval/ then you should specify the correct location as:

    SOURCE_DIR=/path/to/star-vertex-eval/ \
    OUTPUT_DIR=/tmp/jpsi_embed \
    /path/to/star-vertex-eval/supple/submit_jobs_embedding_jpsi.sh 200

There is a number of other script parameters which can be modified if the
default values are not appropriate. Search for "NAMED ARGUMENTS" to find these
values.


Study the ranking scheme in STAR
================================

1.- The macro VertexRank.C will read the MuDst sample and create a tree with some vertex information. To run (e.g. 100 events and output file outvertex.root):

    $root -l -b -q lMuDst.C 'VertexRank.C(100, "path/to/*.MuDst.root", "outvertex")'

2.- Run TMVA to classify signal and background. A separate set of signal and background must be given, with a define set of variables for the training. Also need to specify the classifier. To run (e.g. with classifiers Fisher and Likelihood):

    $root TMVAClassification.C\(\"Fisher,Likelihood\"\)

3.- Apply the results of the classification with ApplyClassifier.C. The output will be the same tree for step (1) but with a new branch with the classifier response. Run:

    $root ApplyClassifier.C
