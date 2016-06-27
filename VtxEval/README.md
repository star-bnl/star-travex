VtxEval: Vertex Reconstruction Evaluation and Efficiency at STAR
================================================================

To build the library and the `vtxeval` executable follow the instructions in the
project's [README.md](../README.md).


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

This section is outdated but left here for the reference until after all
mentioned macros are removed from the repository...

1.- The macro VertexRank.C will read the MuDst sample and create a tree with some vertex information. To run (e.g. 100 events and output file outvertex.root):

    $root -l -b -q lMuDst.C 'VertexRank.C(100, "path/to/*.MuDst.root", "outvertex")'

2.- Run TMVA to classify signal and background. A separate set of signal and background must be given, with a define set of variables for the training. Also need to specify the classifier. To run (e.g. with classifiers Fisher and Likelihood):

    $root TMVAClassification.C\(\"Fisher,Likelihood\"\)

3.- Apply the results of the classification with ApplyClassifier.C. The output will be the same tree for step (1) but with a new branch with the classifier response. Run:

    $root ApplyClassifier.C
