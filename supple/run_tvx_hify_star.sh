#!/usr/bin/env bash
#
# Usage:
#
# $ run_tvx_hify_star.sh pxl_1|pxl_2|ist_1|sst_1|tpc_#
#
# or
#
# $ TVX_HIFY_INPUT_FILE=/path/to/my_file.daq TVX_BINARY_DIR=/my/existing/dir run_tvx_hify_star.sh pxl_1|pxl_2|ist_1|sst_1|tpc_#
#

if [ -n "$1" ] && [[ $1 =~ ^([a-zA-Z]+)_([0-9]+)$ ]]
then
   TVX_DEACT_DET_LAYER=$1
   TVX_DEACT_DET_ID=$(echo $TVX_DEACT_DET_LAYER | sed 's/^\(.*\)_\([0-9]\+\)$/\1/')
   TVX_DEACT_LAYER_ID=$(echo $TVX_DEACT_DET_LAYER | sed 's/^\(.*\)_\([0-9]\+\)$/\2/')
   TVX_DEACT_LAYER_ID_PADDED=$(printf "%02d" $TVX_DEACT_LAYER_ID)
else
   echo "First parameter must be set:"
   echo "$ ${0##*/} pxl_1|pxl_2|ist_1|sst_1|tpc_#"
   exit 1
fi


# Set default values for script variables
: ${TVX_HIFY_INPUT_FILE:=/scratch/smirnovd/public/random_tests/st_physics_15164004_raw_2000022.daq}
: ${TVX_DIR:=${HOME}/star-sti-tools}
: ${TVX_STAR_GEO_FILE:=${TVX_DIR}/y2014a.root}
: ${OUTPUT_DIR:=${HOME}/deact_$TVX_DEACT_DET_LAYER}


case $TVX_DEACT_DET_LAYER in

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
   TVX_VOLUME_REGEX='^.*Tpc/Padrow_'$TVX_DEACT_LAYER_ID'/Sector_[\d]+.*$'
   TVX_HIFY_OPTIONS="--z-min=-250 --z-max=250 --y-min=-50 --y-max=50"
   ;;

   *)
   echo "ERROR: Unknown detector layer $TVX_DEACT_DET_LAYER specified"
   exit 1
   ;;

esac

echo "The following variables will be used:"
echo -e "\t TVX_DEACT_DET_LAYER: $TVX_DEACT_DET_LAYER"
echo -e "\t TVX_DEACT_DET_ID: $TVX_DEACT_DET_ID"
echo -e "\t TVX_DEACT_LAYER_ID: $TVX_DEACT_LAYER_ID"
echo -e "\t TVX_DEACT_LAYER_ID_PADDED: $TVX_DEACT_LAYER_ID_PADDED"
echo -e "\t TVX_VOLUME_REGEX: $TVX_VOLUME_REGEX"
echo -e "\t TVX_HIFY_INPUT_FILE: $TVX_HIFY_INPUT_FILE"
echo -e "\t TVX_DIR: $TVX_DIR"
echo -e "\t TVX_STAR_GEO_FILE: $TVX_STAR_GEO_FILE"
echo -e "\t OUTPUT_DIR: $OUTPUT_DIR"
echo -e "\t TVX_BINARY_DIR: $TVX_BINARY_DIR"

# Print out all command before execution
trap 'echo "$ $BASH_COMMAND"' DEBUG

pwd

if [ -n "$TVX_BINARY_DIR" ]
then
   mkdir -p $TVX_BINARY_DIR && cd $TVX_BINARY_DIR
else
   mkdir -p $OUTPUT_DIR && cd $OUTPUT_DIR
fi

pwd

cmake -D CMAKE_INSTALL_PREFIX=${OUTPUT_DIR} -D CMAKE_CXX_FLAGS="-m32" -D BOOST_ROOT=$OPTSTAR ${TVX_DIR}
make -j8 install

# Create all supplementary files in the output directory
cd $OUTPUT_DIR
which stihify
ln -s -f ${TVX_STAR_GEO_FILE}
echo $TVX_VOLUME_REGEX > deactivate_sti_detectors.txt
cp deactivate_sti_detectors.txt save_sti_detectors.txt
ls -la .
cat deactivate_sti_detectors.txt
cat save_sti_detectors.txt

# Create file with a TTree
root4star -q -b -l 'bfc.C(1, 300, "P2014a mtd btof pxlHit istHit sstHit BEmcChkStat CorrX OSpaceZ2 OGridLeak3D -hitfilt StiHifyTreeMaker", "'$TVX_HIFY_INPUT_FILE'")' &> bfc.log

# Create file with histograms
TVX_HIFY_INPUT_FILE=${TVX_HIFY_INPUT_FILE##*/}
stihify $TVX_HIFY_OPTIONS -f ${TVX_HIFY_INPUT_FILE%.*}.stihify.root -c -g -o deact_${TVX_DEACT_DET_ID}_${TVX_DEACT_LAYER_ID_PADDED}/
