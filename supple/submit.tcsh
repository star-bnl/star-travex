#!/bin/tcsh

if ( $#argv != 1 ) then
  echo ""
  echo " Usage : $0 [FSET number, 109 or something like that]"
  echo ""
  echo ""
  exit
endif

set template = "/star/u/smirnovd/star-vertex-eval/supple/embed_template_P10ik.xml"
set INPUT_FILES = "/star/data20/embedding/reco/2010_zerobias/daq/st_zerobias_adc_11063039_raw_2590003.daq"
set OUTPUT_DIR = "/star/u/smirnovd/bnl_me/public/jpsi_sim/"
set JOB_MAX_EVENTS = 10
set BFC_MIXER_MACRO = "/star/u/smirnovd/star-vertex-eval/ext/star-macros/macros/embedding/bfcMixer_UPCAuAu200_P10ik.C"

mkdir -p $OUTPUT_DIR/sums
mkdir -p $OUTPUT_DIR/log

set FSET = "$1"

# Copy the bfcMixer script to the current directory so it can be used by the job
cp $BFC_MIXER_MACRO ./

star-submit-template -template ${template} -entities FSET=$FSET,INPUT_FILES=$INPUT_FILES,OUTPUT_DIR=$OUTPUT_DIR,JOB_MAX_EVENTS=$JOB_MAX_EVENTS
