#!/usr/bin/env bash
#
# This shell script is a wrapper around two commands used to produce results for
# single hit efficiency measurements for a STAR detector layer. First, we
# reconstruct events from a *.daq file and produce a root file with a TTree.
# Second, we run `stihify` from `star-travex` over that TTree to produce the
# final results and a ROOT file with histograms.
#
# Before running this script it is recommended to examine the script's named
# arguments (see the "NAMED ARGUMENTS" section below) and modify their default
# values as needed.
#
# Once the appropriate values are set the script can be simply executed with a
# single argument specifying a STAR detector layer as:
#
# $ run_star_hify.sh pxl_1|pxl_2|ist_1|sst_1|tpc_#
#
# One can always overwrite all or some of the arguments in following way:
#
# $ INPUT_FILE=/path/to/my_file.daq STAR_TRAVEX_INSTALL_DIR=/my/existing/dir \
#   run_star_hify.sh pxl_1|pxl_2|ist_1|sst_1|tpc_#
#
# For example:
#
# $ cd my_star_travex_install_dir/
# $ STAR_TRAVEX_INSTALL_DIR=./ run_star_hify.sh tpc_7
#


# NAMED ARGUMENTS: Set default values for script variables

# A flag to chose between two running modes
: ${STAR_DEACTIVATE_DETECTOR:=true}

# Path to input file or a list of files. XXX: Need to deal with relative paths
: ${INPUT_FILE:=/scratch/smirnovd/public/random_tests/st_physics_15164004_raw_2000022.daq}

# The maximum number of events to process (assuming we start from the first event)
: ${BFC_NEVENTS:=1000000}

# These default BFC options should be enough to reconstruct real data
: ${BFC_OPTIONS:="P2014a mtd btof pxlHit istHit sstHit BEmcChkStat CorrX OSpaceZ2 OGridLeak3D -hitfilt StiHifyTreeMaker"}

# STAR_TRAVEX_INSTALL_DIR is the directory where star-travex was installed
: ${STAR_TRAVEX_INSTALL_DIR:=${HOME}/star-travex/build_install} && STAR_TRAVEX_INSTALL_DIR=`cd "$STAR_TRAVEX_INSTALL_DIR"; pwd`

# A path to the ROOT file with detector's TGeo geometry
: ${STAR_GEOMETRY_FILE:=$STAR_TRAVEX_INSTALL_DIR/y2014a.root}


# Validate user's input
if [ -d $STAR_TRAVEX_INSTALL_DIR ]; then cd $STAR_TRAVEX_INSTALL_DIR && echo "$ pwd" && pwd;
else
   echo "ERROR: Directory does not exist"
   echo -e "\t STAR_TRAVEX_INSTALL_DIR=$STAR_TRAVEX_INSTALL_DIR"
   exit 1
fi


if [ ! -e $STAR_GEOMETRY_FILE ]
then
   echo "ERROR: Geometry file does not exist"
   echo -e "\t STAR_GEOMETRY_FILE=$STAR_GEOMETRY_FILE"
   exit 1
fi


if [ -n "$1" ] && [[ $1 =~ ^([a-zA-Z]+)_([0-9]+)$ ]]
then
   STAR_DETECTOR=$1
   STAR_DETECTOR_ID=$(echo $STAR_DETECTOR | sed 's/^\(.*\)_\([0-9]\+\)$/\1/')
   STAR_DETECTOR_LAYER_ID=$(echo $STAR_DETECTOR | sed 's/^\(.*\)_\([0-9]\+\)$/\2/')
   STAR_DETECTOR_LAYER_ID_PADDED=$(printf "%02d" $STAR_DETECTOR_LAYER_ID)
else
   echo "ERROR: First parameter must be set:"
   echo "$ ${0##*/} pxl_1|pxl_2|ist_1|sst_1|tpc_#"
   exit 1
fi

case $STAR_DETECTOR in

   pxl_1)
   STAR_VOLUME_REGEX='^.*IDSM_1/PXMO_1/PXLA_[\d]+/LADR_1/PXSI_[\d]+/PLAC_1.*$'
   STAR_HIFY_OPTIONS="--z-min=-15 --z-max=15 --y-min=-2 --y-max=3"
   ;;

   pxl_2)
   STAR_VOLUME_REGEX='^.*IDSM_1/PXMO_1/PXLA_[\d]+/LADR_[234]/PXSI_[\d]+/PLAC_1.*$'
   STAR_HIFY_OPTIONS="--z-min=-15 --z-max=15 --y-min=0 --y-max=5"
   ;;

   ist_1)
   STAR_VOLUME_REGEX='^.*IDSM_1/IBMO_1/IBAM_[\d]+/IBLM_\d/IBSS_1.*$'
   STAR_HIFY_OPTIONS="--z-min=-30 --z-max=30 --y-min=-2 --y-max=8"
   ;;

   sst_1)
   STAR_VOLUME_REGEX='^.*IDSM_1/SFMO_1/SFLM_[\d]+/SFSW_[\d]+/SFSL_1/SFSD_1.*$'
   STAR_HIFY_OPTIONS="--z-min=-40 --z-max=40 --y-min=-3 --y-max=8"
   ;;

   tpc_*)
   STAR_VOLUME_REGEX='^.*Tpc/Padrow_'$STAR_DETECTOR_LAYER_ID'/Sector_[\d]+.*$'
   STAR_HIFY_OPTIONS="--z-min=-250 --z-max=250 --y-min=-50 --y-max=50"
   ;;

   *)
   echo "ERROR: Unknown detector $STAR_DETECTOR specified"
   exit 1
   ;;

esac


echo -e "Named arguments and their values:"
echo -e "\t STAR_DETECTOR:            $STAR_DETECTOR"
echo -e "\t STAR_DETECTOR_ID:         $STAR_DETECTOR_ID"
echo -e "\t STAR_DETECTOR_LAYER_ID:   $STAR_DETECTOR_LAYER_ID (pretty: $STAR_DETECTOR_LAYER_ID_PADDED)"
echo -e "\t STAR_VOLUME_REGEX:        $STAR_VOLUME_REGEX"
echo -e "\t BFC_NEVENTS:              $BFC_NEVENTS"
echo -e "\t BFC_OPTIONS:              $BFC_OPTIONS"
echo -e "\t STAR_DEACTIVATE_DETECTOR: $STAR_DEACTIVATE_DETECTOR"
echo -e "\t INPUT_FILE:               $INPUT_FILE"
echo -e "\t STAR_TRAVEX_INSTALL_DIR:  $STAR_TRAVEX_INSTALL_DIR"
echo -e "\t STAR_GEOMETRY_FILE:       $STAR_GEOMETRY_FILE"


# After this 'trap' command print out all command before execution
trap 'echo "$ $BASH_COMMAND"' DEBUG

# Create supplementary files in the output directory
echo $STAR_VOLUME_REGEX > save_sti_detectors.txt
cat save_sti_detectors.txt

if [ "$STAR_DEACTIVATE_DETECTOR" = true ] ; then
   echo "Deactivated detector: $STAR_DETECTOR"
   cp save_sti_detectors.txt deactivate_sti_detectors.txt
   cat deactivate_sti_detectors.txt
fi

pwd && ls -la .

# Create file with a TTree
root4star -q -b -l 'bfc.C('$BFC_NEVENTS', "'$BFC_OPTIONS'", "'$INPUT_FILE'")'

# Create file with histograms
which stihify

# Do some clean up of the file name
INPUT_FILE=`echo ${INPUT_FILE} | sed 's/^@//g'`
INPUT_FILE=`echo ${INPUT_FILE} | sed 's/\.list//g'`
INPUT_FILE=${INPUT_FILE##*/}

stihify $STAR_HIFY_OPTIONS -f ${INPUT_FILE%.*}.stihify.root -c -g -o deact_${STAR_DETECTOR_ID}_${STAR_DETECTOR_LAYER_ID_PADDED}/
