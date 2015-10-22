#!/usr/bin/env bash
#
# It is recommended to examine and set the appropriate values for the script's
# named arguments before running this script. Search for "NAMED ARGUMENTS" to
# find their input values below.

if [ -z "${1+x}" ]; then
  echo ""
  echo " Usage : $0 [FSET number, 109 or something like that]"
  echo ""
  exit 1
fi


# NAMED ARGUMENTS: Set default values for script variables

: ${FSET:="$1"}
: ${SOURCE_DIR:="${HOME}/star-vertex-eval"}
: ${TEMPLATE:="$SOURCE_DIR/supple/embed_template_P10ik.xml"}
: ${INPUT_FILES:="/star/data20/embedding/reco/2010_zerobias/daq/st_zerobias_adc_11063039_raw_2590003.daq"}
: ${OUTPUT_DIR:="/star/u/smirnovd/bnl_me/public/jpsi_sim/"}
: ${JOB_MAX_EVENTS:=10}
: ${BFC_MIXER_MACRO:="$SOURCE_DIR/ext/star-macros/macros/embedding/bfcMixer_Tpx.C"}


echo -e "Named arguments and their values:"
echo -e "\t FSET: $FSET"
echo -e "\t SOURCE_DIR: $SOURCE_DIR"
echo -e "\t TEMPLATE: $TEMPLATE"
echo -e "\t INPUT_FILES: $INPUT_FILES"
echo -e "\t OUTPUT_DIR: $OUTPUT_DIR"
echo -e "\t JOB_MAX_EVENTS: $JOB_MAX_EVENTS"
echo -e "\t BFC_MIXER_MACRO: $BFC_MIXER_MACRO"


mkdir -p $OUTPUT_DIR/sums
mkdir -p $OUTPUT_DIR/log

# Copy the bfcMixer script to the current directory so it can be used by the job
cp $BFC_MIXER_MACRO ./

star-submit-template -template ${TEMPLATE} -entities FSET=$FSET,INPUT_FILES=$INPUT_FILES,OUTPUT_DIR=$OUTPUT_DIR,JOB_MAX_EVENTS=$JOB_MAX_EVENTS
