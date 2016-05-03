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

# This project depends on Boost
find_package( Boost 1.54.0 REQUIRED COMPONENTS program_options regex filesystem)

# This project depends on ROOT
find_package(ROOT)

if(NOT ROOT_FOUND)
   message(FATAL_ERROR "Fatal error: ROOT package not found")
endif()

# Perform some setup standard to STAR experiment environment
include(StarCommon)


include_directories(
	${Boost_INCLUDE_DIR}
	${ROOT_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	ext/travex/include
	ext/star-base
	ext/star-muDst
	ext/star-stevent
	ext/star-stdb
	ext/star-tables
	ext/star-tables/tables
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

root_generate_dictionary(star-vertex-eval_dict MuMcPrVKFV2012.h VxEff.h LINKDEF VertexEvalLinkDef.h)

add_library( star-vertex-eval SHARED MuMcPrVKFV2012.C VxEff.C
	StarEventHistContainer.cxx StarVertexHistContainer.cxx StarVertexHftHistContainer.cxx VertexRootFile.cxx star-vertex-eval_dict.cxx)

link_directories( ${Boost_LIBRARY_DIR} ${ROOT_LIBRARY_DIR} )

add_executable(vtxhist vtxhist.cc)

target_link_libraries(vtxhist
	travex
	${Boost_LIBRARIES} ${ROOT_LIBRARIES} HistPainter pthread
)

# Installation section
install(TARGETS star-vertex-eval DESTINATION "${STAR_ADDITIONAL_INSTALL_PREFIX}/lib")
