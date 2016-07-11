#!/usr/bin/env bash
#
# See project's README.md for details
#

sample="Wplus_enu"
#sample=Wplus_taunu
#sample=Wminus_enu
#sample=Wminus_taunu
#sample=Z_eplus_eminus_inter
#sample=QCD


: ${INPUT_FILE_LIST:=${HOME}/star-travex/filelist_zerobias_test.txt}

# Path to your local `star-travex` directory
: ${SOURCE_DIR:=${HOME}/star-travex} && SOURCE_DIR=`cd "$SOURCE_DIR"; pwd`

# STAR_TRAVEX_INSTALL_DIR is the directory where star-travex was installed
: ${STAR_TRAVEX_INSTALL_DIR:=${HOME}/star-travex/build_install} && STAR_TRAVEX_INSTALL_DIR=`cd "$STAR_TRAVEX_INSTALL_DIR"; pwd`

# $HOME/scratch/wbos_embed/
: ${OUT_DIR:=/path/to/some_dir} && mkdir -p $OUT_DIR && OUT_DIR=`cd "$OUT_DIR"; pwd`

# Various macros from the source directory with customizalbe parameters
# controlling event reconstruction.
: ${BFC_MIXER_MACRO:="$SOURCE_DIR/supple/bfcMixer_pythia.C"}
: ${VERTEX_GEN_MACRO:="$SOURCE_DIR/ext/star-macros/macros/embedding/GenerateVertex.C"}
#: ${VERTEX_PARAMS_DB_FILE:="$SOURCE_DIR/ext/star-db-calibrations/./tracker/PrimaryVertexCuts.20121221.000000.C"}
: ${STARSIM_KUMAC:="$SOURCE_DIR/supple/simWRequest.kumac"}


echo -e "Named arguments and their values:"
echo -e "\t sample:                   $sample"
echo -e "\t SOURCE_DIR:               $SOURCE_DIR"
echo -e "\t STAR_TRAVEX_INSTALL_DIR:  $STAR_TRAVEX_INSTALL_DIR"
echo -e "\t OUT_DIR:                  $OUT_DIR"
echo -e "\t BFC_MIXER_MACRO:          $BFC_MIXER_MACRO"
echo -e "\t VERTEX_GEN_MACRO:         $VERTEX_GEN_MACRO"
echo -e "\t VERTEX_PARAMS_DB_FILE:    $VERTEX_PARAMS_DB_FILE"
echo -e "\t STARSIM_KUMAC:            $STARSIM_KUMAC"


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
norm=1.0
#norm=1.0 for 110K events (W+)
#norm=0.32 for 35K events (W-)
#norm=0.23 for 25K events (Z/gamma*)


IFS=\  #set space as delimeter
while read JOB_INPUT_FILE RUN_ID JOB_NEVENTS
do

  nEventsScaled=$(expr $JOB_NEVENTS*$norm |tr -d $'\r'| bc)
  nEventsPretty=`echo $nEventsScaled | awk '{printf("%d\n",$1+0.5)}'`
  JOB_NEVENTS=$nEventsPretty


  #set queue length by # of target events
  filesPerHour=0.01
  if [ $JOB_NEVENTS -lt 50 ]
  then
    filesPerHour=0.21
  fi
  if [ $JOB_NEVENTS -lt 10 ]
  then
    filesPerHour=0.34
  fi


  echo -e "\nProcessing new line from input file list"
  echo -e "\t RANDOM_SEED: $RANDOM_SEED"
  echo -e "\t JOB_NEVENTS [nEventsScaled, nEventsPretty]: $JOB_NEVENTS [$nEventsScaled, $nEventsPretty]"
  echo -e "\t JOB_INPUT_FILE: $JOB_INPUT_FILE"
  echo -e "\t RUN_ID: $RUN_ID"
  echo -e "\t filesPerHour: $filesPerHour"

  star-submit-template -template $SOURCE_DIR/supple/job_template_embedding_wbos.xml \
     -entities  JOB_NEVENTS=$JOB_NEVENTS,OUT_DIR=$OUT_DIR,SOURCE_DIR=$SOURCE_DIR,JOB_INPUT_FILE=$JOB_INPUT_FILE,RUN_ID=$RUN_ID,sample=$sample,RANDOM_SEED=$RANDOM_SEED,filesPerHour=$filesPerHour

  RANDOM_SEED=`expr $RANDOM_SEED \+ 1`

done < $INPUT_FILE_LIST
