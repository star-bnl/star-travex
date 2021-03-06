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
: ${TEMPLATE:="$SOURCE_DIR/supple/job_template_embedding_jpsi.xml"}
: ${INPUT_FILES:="/star/data05/daq/2014/152/15152001/st_zerobias_adc_15152001_raw_5000050.daq"}
: ${OUTPUT_DIR:="${HOME}/jpsi_embed/"}
: ${JOB_MAX_EVENTS:=10}
: ${BFC_MIXER_MACRO:="$SOURCE_DIR/ext/star-macros/macros/embedding/bfcMixer_Tpx.C"}
: ${VERTEX_GEN_MACRO:="$SOURCE_DIR/ext/star-macros/macros/embedding/GenerateVertex.C"}
: ${STARSIM_KUMAC:="$SOURCE_DIR/supple/starlight_Jpsi_ee.kumac"}


echo -e "Named arguments and their values:"
echo -e "\t FSET: $FSET"
echo -e "\t SOURCE_DIR: $SOURCE_DIR"
echo -e "\t TEMPLATE: $TEMPLATE"
echo -e "\t INPUT_FILES: $INPUT_FILES"
echo -e "\t OUTPUT_DIR: $OUTPUT_DIR"
echo -e "\t JOB_MAX_EVENTS: $JOB_MAX_EVENTS"
echo -e "\t BFC_MIXER_MACRO: $BFC_MIXER_MACRO"
echo -e "\t VERTEX_GEN_MACRO: $VERTEX_GEN_MACRO"


mkdir -p $OUTPUT_DIR/sums
mkdir -p $OUTPUT_DIR/log

# Copy the bfcMixer script to the current directory so it can be used by the job
cp $BFC_MIXER_MACRO ./
cp $VERTEX_GEN_MACRO ./
cp $STARSIM_KUMAC ./

star-submit-template -template ${TEMPLATE} -entities FSET=$FSET,INPUT_FILES=$INPUT_FILES,OUTPUT_DIR=$OUTPUT_DIR,JOB_MAX_EVENTS=$JOB_MAX_EVENTS
