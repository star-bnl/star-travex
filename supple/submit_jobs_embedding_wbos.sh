#!/usr/bin/env bash
#
# See star-travex/VtxEval/README.md for details.
#

# The file with a list of zerobias data files to use in embedding
: ${INPUT_FILE_LIST:=${HOME}/star-travex/filelist_zerobias.lis}

# Path to the local `star-travex` directory
: ${SOURCE_DIR:=${HOME}/star-travex} && SOURCE_DIR=`cd "$SOURCE_DIR"; pwd`

# STAR_TRAVEX_INSTALL_DIR is the directory where star-travex was installed
: ${STAR_TRAVEX_INSTALL_DIR:=${HOME}/star-travex/build-install} && STAR_TRAVEX_INSTALL_DIR=`cd "$STAR_TRAVEX_INSTALL_DIR"; pwd`

# $HOME/scratch/wbos_embed/
: ${OUT_DIR:=/path/to/some_dir} && mkdir -p $OUT_DIR && OUT_DIR=`cd "$OUT_DIR"; pwd`


# Various macros from the source directory with customizalbe parameters
# controlling event reconstruction.
: ${BFC_MIXER_MACRO:="$SOURCE_DIR/supple/bfcMixer_pythia.C"}
: ${VERTEX_GEN_MACRO:="$SOURCE_DIR/ext/star-macros/macros/embedding/GenerateVertex.C"}
#: ${VERTEX_PARAMS_DB_FILE:="$SOURCE_DIR/ext/star-db-calibrations/./tracker/PrimaryVertexCuts.20121221.000000.C"}
: ${STARSIM_KUMAC:="$SOURCE_DIR/supple/simWRequest.kumac"}


# Parametric option to study available vertex finder performance. Pick one from
# the following options:
JOB_BFC_OPTIONS_VERTEX=""


# Sample three chain options for embedding sample reconstruction
JOB_BFC_OPTIONS_1="in magF tpcDb NoDefault TpxRaw -ittf NoOutput useXgeom"
JOB_BFC_OPTIONS_2="fzin gen_T geomT sim_T TpcRS -ittf -tpc_daq nodefault ry2013_1c"
JOB_BFC_OPTIONS_3="pp2013b mtd btof fmsDat fgt fgtPoint $JOB_BFC_OPTIONS_VERTEX BEmcChkStat Corr4 OSpaceZ2 OGridLeak3D -hitfilt -evout IdTruth TpxClu -hitfilt Embedding TpcMixer McAna -in NoInput useInTracker emcSim EEfs"

# Replace all spaces in the above options with dots in order to pass the strings to condor *.xml files
JOB_BFC_OPTIONS_1=`echo ${JOB_BFC_OPTIONS_1[*]} | sed 's/ /./g'`
JOB_BFC_OPTIONS_2=`echo ${JOB_BFC_OPTIONS_2[*]} | sed 's/ /./g'`
JOB_BFC_OPTIONS_3=`echo ${JOB_BFC_OPTIONS_3[*]} | sed 's/ /./g'`


# This variable defins the simulated signal in kumac passed to starsim. It can
# take one of the following values:
# Wplus_enu, Wplus_taunu, Wminus_enu, Wminus_taunu, Z_eplus_eminus_inter, QCD
STARSIM_MODE="Wplus_enu"


echo -e "Named arguments and their values:"
echo -e "\t INPUT_FILE_LIST:          $INPUT_FILE_LIST"
echo -e "\t SOURCE_DIR:               $SOURCE_DIR"
echo -e "\t STAR_TRAVEX_INSTALL_DIR:  $STAR_TRAVEX_INSTALL_DIR"
echo -e "\t OUT_DIR:                  $OUT_DIR"
echo -e "\t BFC_MIXER_MACRO:          $BFC_MIXER_MACRO"
echo -e "\t VERTEX_GEN_MACRO:         $VERTEX_GEN_MACRO"
echo -e "\t VERTEX_PARAMS_DB_FILE:    $VERTEX_PARAMS_DB_FILE"
echo -e "\t STARSIM_KUMAC:            $STARSIM_KUMAC"
echo -e "\t STARSIM_MODE:             $STARSIM_MODE"
echo -e "\t JOB_BFC_OPTIONS_1:        $JOB_BFC_OPTIONS_1"
echo -e "\t JOB_BFC_OPTIONS_2:        $JOB_BFC_OPTIONS_2"
echo -e "\t JOB_BFC_OPTIONS_3:        $JOB_BFC_OPTIONS_3"


# After this 'trap' command print out all command before execution
trap 'echo "$ $BASH_COMMAND"' DEBUG

# Export STAR_TRAVEX_INSTALL_DIR so we can use it in .xml template
export STAR_TRAVEX_INSTALL_DIR

# Create output directories
mkdir -p $OUT_DIR/root/
mkdir -p $OUT_DIR/log/
mkdir -p $OUT_DIR/fzd/
mkdir -p $OUT_DIR/sums/

rsync -av $BFC_MIXER_MACRO  $STAR_TRAVEX_INSTALL_DIR/
rsync -av $VERTEX_GEN_MACRO $STAR_TRAVEX_INSTALL_DIR/
rsync -av $STARSIM_KUMAC    $STAR_TRAVEX_INSTALL_DIR/

if [ -n "${VERTEX_PARAMS_DB_FILE}" ]; then
   mkdir -p $STAR_TRAVEX_INSTALL_DIR/StarDb/Calibrations/
   rsync -av -R $VERTEX_PARAMS_DB_FILE $STAR_TRAVEX_INSTALL_DIR/StarDb/Calibrations/
else
   echo -e "\nNOTICE: No \$VERTEX_PARAMS_DB_FILE was provided"
fi


# Remove package from previous job execution
rm -fr tarball_wbos_embed.package tarball_wbos_embed.zip

trap '' DEBUG


RANDOM_SEED=2000

IFS=\  #set space as delimeter
while read  JOB_INPUT_FILE  RUN_ID  JOB_NEVENTS
do

   echo -e "\nProcessing new line from input file list"
   echo -e "\t RANDOM_SEED: $RANDOM_SEED"
   echo -e "\t JOB_NEVENTS: $JOB_NEVENTS"
   echo -e "\t JOB_INPUT_FILE: $JOB_INPUT_FILE"
   echo -e "\t RUN_ID: $RUN_ID"

   COMMAND="star-submit-template -template $SOURCE_DIR/supple/job_template_embedding_wbos.xml \
      -entities  JOB_NEVENTS=$JOB_NEVENTS,OUT_DIR=$OUT_DIR,SOURCE_DIR=$SOURCE_DIR,JOB_INPUT_FILE=$JOB_INPUT_FILE,RUN_ID=$RUN_ID,STARSIM_MODE=$STARSIM_MODE,RANDOM_SEED=$RANDOM_SEED,JOB_BFC_OPTIONS_1=$JOB_BFC_OPTIONS_1,JOB_BFC_OPTIONS_2=$JOB_BFC_OPTIONS_2,JOB_BFC_OPTIONS_3=$JOB_BFC_OPTIONS_3"

   echo $COMMAND
   echo

   eval $COMMAND

   RANDOM_SEED=`expr $RANDOM_SEED \+ 1`

done < $INPUT_FILE_LIST
