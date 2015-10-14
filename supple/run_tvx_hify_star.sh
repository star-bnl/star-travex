#!/usr/bin/env bash
#
# It is recommended to examine and set the appropriate values for the script's
# named arguments before running this script. Search for "NAMED ARGUMENTS" to
# find their input values below.
#
# With default values the script can be simply executed as:
#
# $ run_tvx_hify_star.sh pxl_1|pxl_2|ist_1|sst_1|tpc_#
#
# or, one can overwrite selected arguments as:
#
# $ TVX_BFC_INPUT_FILE=/path/to/my_file.daq TVX_INSTALL_DIR=/my/existing/dir \
#   run_tvx_hify_star.sh pxl_1|pxl_2|ist_1|sst_1|tpc_#
#
# For example:
#
# $ cd my_tvx_install/
# $ TVX_INSTALL_DIR=./ run_tvx_hify_star.sh tpc_7
#


# NAMED ARGUMENTS: Set default values for script variables

# A flag to chose between two running modes
: ${TVX_DEACTIVATE_DETECTOR:=true}

# Path to input file or a list of files. XXX: Need to deal with relative paths
: ${TVX_BFC_INPUT_FILE:=/scratch/smirnovd/public/random_tests/st_physics_15164004_raw_2000022.daq}

# The maximum number of events to process (assuming we start from the first event)
: ${TVX_BFC_NEVENTS:=1000000}

# These default BFC options should be good for reconstructing real data
: ${TVX_BFC_OPTIONS:="P2014a mtd btof pxlHit istHit sstHit BEmcChkStat CorrX OSpaceZ2 OGridLeak3D -hitfilt StiHifyTreeMaker"}

# TVX_INSTALL_DIR is the directory where star-sti-tools was installed
: ${TVX_INSTALL_DIR:=${HOME}/travex/build_install} && TVX_INSTALL_DIR=`cd "$TVX_INSTALL_DIR"; pwd`

# A path to the ROOT file with detector's TGeo geometry
: ${TVX_STAR_GEO_FILE:=$TVX_INSTALL_DIR/y2014a.root}


# Validate user's input
if [ -d $TVX_INSTALL_DIR ]; then cd $TVX_INSTALL_DIR && echo "$ pwd" && pwd;
else
   echo "ERROR: Directory does not exist"
   echo -e "\t TVX_INSTALL_DIR=$TVX_INSTALL_DIR"
   exit 1
fi


if [ ! -e $TVX_STAR_GEO_FILE ]
then
   echo "ERROR: Geometry file does not exist"
   echo -e "\t TVX_STAR_GEO_FILE=$TVX_STAR_GEO_FILE"
   exit 1
fi


if [ -n "$1" ] && [[ $1 =~ ^([a-zA-Z]+)_([0-9]+)$ ]]
then
   TVX_DETECTOR=$1
   TVX_DETECTOR_ID=$(echo $TVX_DETECTOR | sed 's/^\(.*\)_\([0-9]\+\)$/\1/')
   TVX_DETECTOR_LAYER_ID=$(echo $TVX_DETECTOR | sed 's/^\(.*\)_\([0-9]\+\)$/\2/')
   TVX_DETECTOR_LAYER_ID_PADDED=$(printf "%02d" $TVX_DETECTOR_LAYER_ID)
else
   echo "ERROR: First parameter must be set:"
   echo "$ ${0##*/} pxl_1|pxl_2|ist_1|sst_1|tpc_#"
   exit 1
fi

case $TVX_DETECTOR in

   pxl_1)
   TVX_VOLUME_REGEX='^.*IDSM_1/PXMO_1/PXLA_[\d]+/LADR_1/PXSI_[\d]+/PLAC_1.*$'
   TVX_HIFY_OPTIONS="--z-min=-15 --z-max=15 --y-min=-2 --y-max=3"
   ;;

   pxl_2)
   TVX_VOLUME_REGEX='^.*IDSM_1/PXMO_1/PXLA_[\d]+/LADR_[234]/PXSI_[\d]+/PLAC_1.*$'
   TVX_HIFY_OPTIONS="--z-min=-15 --z-max=15 --y-min=0 --y-max=5"
   ;;

   ist_1)
   TVX_VOLUME_REGEX='^.*IDSM_1/IBMO_1/IBAM_[\d]+/IBLM_\d/IBSS_1.*$'
   TVX_HIFY_OPTIONS="--z-min=-30 --z-max=30 --y-min=-2 --y-max=8"
   ;;

   sst_1)
   TVX_VOLUME_REGEX='^.*IDSM_1/SFMO_1/SFLM_[\d]+/SFSW_[\d]+/SFSL_1/SFSD_1.*$'
   TVX_HIFY_OPTIONS="--z-min=-40 --z-max=40 --y-min=-3 --y-max=8"
   ;;

   tpc_*)
   TVX_VOLUME_REGEX='^.*Tpc/Padrow_'$TVX_DETECTOR_LAYER_ID'/Sector_[\d]+.*$'
   TVX_HIFY_OPTIONS="--z-min=-250 --z-max=250 --y-min=-50 --y-max=50"
   ;;

   *)
   echo "ERROR: Unknown detector $TVX_DETECTOR specified"
   exit 1
   ;;

esac


echo -e "Named arguments and their values:"
echo -e "\t TVX_DETECTOR: $TVX_DETECTOR"
echo -e "\t TVX_DETECTOR_ID: $TVX_DETECTOR_ID"
echo -e "\t TVX_DETECTOR_LAYER_ID: $TVX_DETECTOR_LAYER_ID (pretty: $TVX_DETECTOR_LAYER_ID_PADDED)"
echo -e "\t TVX_VOLUME_REGEX: $TVX_VOLUME_REGEX"
echo -e "\t TVX_BFC_NEVENTS: $TVX_BFC_NEVENTS"
echo -e "\t TVX_BFC_OPTIONS: $TVX_BFC_OPTIONS"
echo -e "\t TVX_DEACTIVATE_DETECTOR: $TVX_DEACTIVATE_DETECTOR"
echo -e "\t TVX_BFC_INPUT_FILE: $TVX_BFC_INPUT_FILE"
echo -e "\t TVX_INSTALL_DIR: $TVX_INSTALL_DIR"
echo -e "\t TVX_STAR_GEO_FILE: $TVX_STAR_GEO_FILE"


# After this 'trap' command print out all command before execution
trap 'echo "$ $BASH_COMMAND"' DEBUG

# Create supplementary files in the output directory
echo $TVX_VOLUME_REGEX > save_sti_detectors.txt
cat save_sti_detectors.txt

if [ "$TVX_DEACTIVATE_DETECTOR" = true ] ; then
   echo "Deactivated detector: $TVX_DETECTOR"
   cp save_sti_detectors.txt deactivate_sti_detectors.txt
   cat deactivate_sti_detectors.txt
fi

pwd && ls -la .

# Create file with a TTree
root4star -q -b -l 'bfc.C('$TVX_BFC_NEVENTS', "'$TVX_BFC_OPTIONS'", "'$TVX_BFC_INPUT_FILE'")'

# Create file with histograms
which stihify

# Do some clean up of the file name
TVX_BFC_INPUT_FILE=`echo ${TVX_BFC_INPUT_FILE} | sed 's/^@//g'`
TVX_BFC_INPUT_FILE=`echo ${TVX_BFC_INPUT_FILE} | sed 's/\.list//g'`
TVX_BFC_INPUT_FILE=${TVX_BFC_INPUT_FILE##*/}

stihify $TVX_HIFY_OPTIONS -f ${TVX_BFC_INPUT_FILE%.*}.stihify.root -c -g -o deact_${TVX_DETECTOR_ID}_${TVX_DETECTOR_LAYER_ID_PADDED}/
