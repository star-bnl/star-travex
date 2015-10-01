#!/usr/bin/env bash
#
# Usage example:
#
# JOB_FILE_LIST=../path/to/filelist_fz.txt submit_jobs_hftree.sh 
#
# JOB_FILE_LIST is the full path to a text file with a list of *.fz files
#


# Set typical default values for script variables
: ${SOURCE_DIR:=${HOME}/star-vertex-eval}
: ${JOB_FILE_LIST:=${HOME}/star-vertex-eval/supple/w_sim_nopileup_fzd_short.txt}
: ${OUT_DIR:="/star/institutions/bnl_me/smirnovd/public/w_sim_nopileup_kfv"}
: ${STAR_VERSION:="SL15j"}

BFC_OPTIONS="tpcRS y2014a AgML MakeEvent KFVertex ITTF StiHftC Idst BAna l0 Tree logger Sti tpcDB TpcHitMover TpxClu bbcSim btofsim tags emcY2 EEfs evout IdTruth geantout big fzin McEvOut McAss McEvent clearmem"

# Replace all spaces with underscores
# Will not work with options containing a _
BFC_OPTIONS=`echo ${BFC_OPTIONS[*]} | sed 's/ /_/g'`


echo "The following variables will be used:"
echo -e "\t SOURCE_DIR:      $SOURCE_DIR"
echo -e "\t JOB_FILE_LIST:   $JOB_FILE_LIST"
echo -e "\t OUT_DIR:         $OUT_DIR"
echo -e "\t STAR_VERSION:    $STAR_VERSION"
echo -e "\t BFC_OPTIONS:     $BFC_OPTIONS"

mkdir -p $OUT_DIR
mkdir -p ./sums/

for JOB_INPUT_FILE in `cat $JOB_FILE_LIST`
do
   JOB_NAME_UID=`basename $JOB_INPUT_FILE .fzd`
   echo
   echo "Submitting job for JOB_INPUT_FILE =" $JOB_INPUT_FILE \($JOB_NAME_UID\)
   echo star-submit-template -template $SOURCE_DIR/supple/job_template_reco_fzd.xml \
      -entities JOB_NAME_UID=$JOB_NAME_UID,JOB_INPUT_FILE=$JOB_INPUT_FILE,OUT_DIR=$OUT_DIR,STAR_VERSION=$STAR_VERSION,BFC_OPTIONS=$BFC_OPTIONS
   echo
done
