#
# Basic usage:
#
# cd <project_dir>
# mkdir build
# cd build
# cmake ../
# make

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(star-vertex-eval)


# Add to path in order to pick up the FindXXX.cmake files included in this project
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# This project depends on ROOT
find_package(ROOT)

if(NOT ROOT_FOUND)
   message(FATAL_ERROR "Fatal error: ROOT package not found")
endif()

add_compile_options(-std=c++0x)

include_directories(
	${ROOT_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
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

add_subdirectory(ext/star-vertex)

root_generate_dictionary(star-vertex-eval_Cint MuMcPrVKFV2012.h VertexRank.h VxEff.h LINKDEF VertexEvalLinkDef.h)

add_library( star-vertex-eval SHARED VertexRank.C MuMcPrVKFV2012.C VxEff.C star-vertex-eval_Cint.cxx)

install(TARGETS star-vertex-eval DESTINATION "lib")
