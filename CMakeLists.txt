# Basic usage:
#
# cd <project_dir>
# mkdir build && cd build
# cmake ../
# make

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(star-vertex-eval)


# Add to path in order to pick up the FindXXX.cmake files included in this project
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/star-cmake")

# This project depends on ROOT
find_package(ROOT)

if(NOT ROOT_FOUND)
   message(FATAL_ERROR "Fatal error: ROOT package not found")
endif()

# Perform some setup standard to STAR experiment environment
include(StarCommon)


include_directories(
	${ROOT_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	ext/travex/include
	ext/star-base
	ext/star-muDst
	ext/star-stevent
	ext/star-stdb
	ext/star-tables
	ext/star-daq
	ext/star-tof
	ext/star-emc
	ext/star-fgt
	ext/star-sti
	ext/star-bfchain
	ext/star-pams/global/inc
	ext/star-sim
)

add_subdirectory(ext/travex)
add_subdirectory(ext/star-vertex)

root_generate_dictionary(star-vertex-eval_dict MuMcPrVKFV2012.h VertexRank.h VxEff.h LINKDEF VertexEvalLinkDef.h)

add_library( star-vertex-eval SHARED VertexRank.C MuMcPrVKFV2012.C VxEff.C
	StarEventHistContainer.cxx StarVertexHistContainer.cxx star-vertex-eval_dict.cxx)


# Installation section
install(TARGETS star-vertex-eval DESTINATION "${STAR_ADDITIONAL_INSTALL_PREFIX}/lib")
