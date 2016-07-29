#!/usr/bin/env bash
#
# Use this script to submit reconstruction jobs to condor.
#

# The file with a list of zerobias data files to use in embedding
: ${INPUT_FILE_LIST:=${HOME}/star-travex/filelist_zerobias.lis}

# Path to the local `star-travex` directory
: ${SOURCE_DIR:=${HOME}/star-travex} && SOURCE_DIR=`cd "$SOURCE_DIR"; pwd`

# STAR_TRAVEX_INSTALL_DIR is the directory where star-travex was installed
: ${STAR_TRAVEX_INSTALL_DIR:=${HOME}/star-travex/build-install} && STAR_TRAVEX_INSTALL_DIR=`cd "$STAR_TRAVEX_INSTALL_DIR"; pwd`

# $HOME/scratch/wbos_embed/
: ${OUT_DIR:=/path/to/some_dir} && mkdir -p $OUT_DIR && OUT_DIR=`cd "$OUT_DIR"; pwd`

# Sample three chain options for embedding sample reconstruction
JOB_BFC_OPTIONS="pp2013a Sti mtd btof VFPPVnoCTB beamline BEmcChkStat -evout CorrX OSpaceZ2 OGridLeak3D -hitfilt"

# Replace all spaces in the above options with dots in order to pass the strings to condor *.xml files
JOB_BFC_OPTIONS=`echo ${JOB_BFC_OPTIONS[*]} | sed 's/ /./g'`

# We use the output directory as A human readable unique request ID
REQUEST_UID=$(basename "${OUT_DIR}")


echo -e "Named arguments and their values:"
echo -e "\t INPUT_FILE_LIST:          $INPUT_FILE_LIST"
echo -e "\t SOURCE_DIR:               $SOURCE_DIR"
echo -e "\t STAR_TRAVEX_INSTALL_DIR:  $STAR_TRAVEX_INSTALL_DIR"
echo -e "\t OUT_DIR:                  $OUT_DIR"
echo -e "\t JOB_BFC_OPTIONS:          $JOB_BFC_OPTIONS"
echo -e "\t REQUEST_UID:              $REQUEST_UID"


# After this 'trap' command print out all command before execution
trap 'echo "$ $BASH_COMMAND"' DEBUG

# Create output directories
mkdir -p $OUT_DIR/root/
mkdir -p $OUT_DIR/log/
mkdir -p $OUT_DIR/sums/

# Remove package from previous job execution
rm -fr tarball_${REQUEST_UID}.package tarball_${REQUEST_UID}.zip

# Turn off shell debugging
trap '' DEBUG


# Form the command to submit jobs to farm
COMMAND="star-submit-template -template $SOURCE_DIR/supple/job_template_reco.xml \
   -entities  OUT_DIR=$OUT_DIR,INPUT_FILE_LIST=$INPUT_FILE_LIST,JOB_BFC_OPTIONS=$JOB_BFC_OPTIONS,REQUEST_UID=$REQUEST_UID,STAR_TRAVEX_INSTALL_DIR=$STAR_TRAVEX_INSTALL_DIR"

echo
echo "Executing..."
echo $COMMAND

eval $COMMAND
