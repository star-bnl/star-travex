#!/usr/bin/env bash
#
# Use this script to submit jobs to condor.
#
# Usage:
#
# $ submit_jobs_star_hify.sh
#
# JOB_INPUT_FILE can be a text file with a list of actual files to process. In
# this case the full path must be prepended with an '@' symbol


# Set typical default values for script variables
: ${JOB_INPUT_FILE:=/star/data99/reco/AuAu200/hijing_382/b0_20/minbias/y2014a/gheisha_on/pileup_eval_Oct05/rcf15005_10_10evts.event.root}

: ${JOB_NEVENTS:=1000000}

# A flag to chose between two running modes
: ${STAR_DEACTIVATE_DETECTOR:=true}

: ${SOURCE_DIR:=${HOME}/star-travex} && SOURCE_DIR=`cd "$SOURCE_DIR"; pwd`

# STAR_TRAVEX_INSTALL_DIR is the directory where star-travex was installed
: ${STAR_TRAVEX_INSTALL_DIR:=${HOME}/star-travex/build_install} && STAR_TRAVEX_INSTALL_DIR=`cd "$STAR_TRAVEX_INSTALL_DIR"; pwd`

: ${OUT_DIR:=/path/to/some_dir} && mkdir -p $OUT_DIR && OUT_DIR=`cd "$OUT_DIR"; pwd`

#STAR_DETECTOR_LAYERS=("pxl_1" "pxl_2" "ist_1" "sst_1" "tpc_1" "tpc_2" "tpc_3" "tpc_4" "tpc_5" "tpc_6" "tpc_7" "tpc_8" "tpc_9" "tpc_10" "tpc_11" "tpc_12" "tpc_13" "tpc_14" "tpc_15" "tpc_16" "tpc_17" "tpc_18" "tpc_19" "tpc_20" "tpc_21" "tpc_22" "tpc_23" "tpc_24" "tpc_25" "tpc_26" "tpc_27" "tpc_28" "tpc_29" "tpc_30" "tpc_31" "tpc_32" "tpc_33" "tpc_34" "tpc_35" "tpc_36" "tpc_37" "tpc_38" "tpc_39" "tpc_40" "tpc_41" "tpc_42" "tpc_43" "tpc_44" "tpc_45")
STAR_DETECTOR_LAYERS=("ist_1")

# BFC options for simulation
# We use chain options similar to those used for 2014 MC reconstruction
# See http://www.star.bnl.gov/devcgi/weekDEVjobStatus.pl
#JOB_BFC_OPTIONS="fzin tpcRS y2014a AgML pxlFastSim istFastSim usexgeom FieldOn MakeEvent VFPPVnoCTB beamline Sti NoSsdIt NoSvtIt StiHftC TpcHitMover TpxClu Idst BAna l0 Tree logger genvtx tpcDB bbcSim btofsim tags emcY2 EEfs geantout evout -dstout IdTruth big StiHifyTreeMaker clearmem"

# BFC options for running over event.root files
JOB_BFC_OPTIONS="in y2014a AgML pxlFastSim istFastSim usexgeom FieldOn MakeEvent VFPPVnoCTB beamline Sti NoSsdIt NoSvtIt StiHftC TpcHitMover TpxClu Idst BAna l0 Tree logger genvtx tpcDB bbcSim btofsim tags emcY2 EEfs geantout evout -dstout IdTruth big StiHifyTreeMaker clearmem"

# BFC options for real data
#JOB_BFC_OPTIONS="P2014a mtd btof pxlHit istHit sstHit BEmcChkStat CorrX OSpaceZ2 OGridLeak3D -hitfilt StiHifyTreeMaker"

# Replace all spaces with underscores. This is needed to pass the list of
# options to the xml job template. Will not work with options containing a _
JOB_BFC_OPTIONS=`echo ${JOB_BFC_OPTIONS[*]} | sed 's/ /_/g'`


echo -e "The following variables are set:\n"
echo -e "\t JOB_INPUT_FILE:           $JOB_INPUT_FILE"
echo -e "\t JOB_NEVENTS:              $JOB_NEVENTS"
echo -e "\t SOURCE_DIR:               $SOURCE_DIR"
echo -e "\t STAR_TRAVEX_INSTALL_DIR:  $STAR_TRAVEX_INSTALL_DIR"
echo -e "\t OUT_DIR:                  $OUT_DIR"
echo -e "\t STAR_DETECTOR_LAYERS:     ${STAR_DETECTOR_LAYERS[*]}"
echo -e "\t JOB_BFC_OPTIONS:          $JOB_BFC_OPTIONS"


# After this 'trap' command print out all command before execution
trap 'echo "$ $BASH_COMMAND"' DEBUG

# Export STAR_TRAVEX_INSTALL_DIR so we can use it in .xml template
export STAR_TRAVEX_INSTALL_DIR
mkdir -p $OUT_DIR/sums/
cd $OUT_DIR


cp $SOURCE_DIR/y2014a.root $STAR_TRAVEX_INSTALL_DIR/
cp $SOURCE_DIR/supple/run_star_hify.sh $STAR_TRAVEX_INSTALL_DIR/


for STAR_DETECTOR in "${STAR_DETECTOR_LAYERS[@]}"
do

   OUT_DIR="${OUT_DIR}/$STAR_DETECTOR"
   mkdir -p $OUT_DIR

   echo
   echo "Submitting job for $STAR_DETECTOR"
   star-submit-template -template $SOURCE_DIR/supple/job_template_star_hify.xml \
      -entities JOB_INPUT_FILE=$JOB_INPUT_FILE,JOB_NEVENTS=$JOB_NEVENTS,JOB_BFC_OPTIONS=$JOB_BFC_OPTIONS,OUT_DIR=$OUT_DIR,STAR_DETECTOR=$STAR_DETECTOR,STAR_DEACTIVATE_DETECTOR=$STAR_DEACTIVATE_DETECTOR
   echo
done
