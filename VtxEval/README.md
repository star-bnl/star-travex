VtxEval: Vertex Reconstruction Evaluation and Efficiency at STAR
================================================================

To build the library and the `vtxeval` executable follow the instructions in the
project's top level [README.md](../README.md). Since in this study we are
focusing on evaluation of the vertex finders one can build and install only the
relevant libraries:

    $ mkdir build/ && cd build/
    $ cmake -D CMAKE_INSTALL_PREFIX=../build-install -D BOOST_ROOT=$OPTSTAR ../
    $ make -j4 star-vertex
    $ make install


How to create embedding samples
===============================

Use STAR's `get_file_list.pl` to select a set of desired zerobias files from the
database. Here is an example command with some selection constraints:

    $ get_file_list.pl -keys path,filename,events -cond 'filename~st_zerobias_adc_1412%,year=2013,runnumber>14125000,sanity=1' -delim / -limit 20

You may choose to copy the files from `hpss` to some location on disk. In that
case, the paths to files returned by `get_file_list.pl` can be adjusted to point
to the actual file location. The final list of zerobias data files has to be in
the following format:

    path/to/file.daq run_id num_of_events_to_process

The contents of your file may look like the following:

    $ cat filelist_zerobias.lis
    /star/data05/daq/2013/embedding/st_zerobias_adc_14150005_raw_3330002.daq 14150005 103
    /star/data05/daq/2013/embedding/st_zerobias_adc_14150004_raw_1530002.daq 14150004 103
    ...
    /star/data05/daq/2013/embedding/st_zerobias_adc_14150005_raw_8660002.daq 14150005 102
    ...


W boson
-------

The script used for submitting W embedding jobs to condor is called
`submit_jobs_embedding_wbos.sh` and can be found in `star-travex/supple/`. There
is a number of named parameters which user may need to set to appropriate
values. The two parameters likely to be changed often are `OUT_DIR` and
`INPUT_FILE_LIST`. All named variables can be passed to the script in command
line like this:

    $ OUT_DIR=/tmp/scratch/wbos_embed_results/ INPUT_FILE_LIST=~/star-travex/supple/filelist_zerobias.lis submit_jobs_embedding_wbos.sh


J/psi
-----

Create a directory from where condor jobs will be submitted and run the script
as:

    mkdir submit_jpsi && cd submit_jpsi
    /path/to/star-travex/supple/submit_jobs_embedding_jpsi.sh 200

If the package was not checked out in the default location of
${HOME}/star-travex/ then you should specify the correct location as:

    SOURCE_DIR=/path/to/star-travex/ \
    OUTPUT_DIR=/tmp/jpsi_embed \
    /path/to/star-travex/supple/submit_jobs_embedding_jpsi.sh 200

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
