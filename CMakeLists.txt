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
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

# This project depends on ROOT
find_package(ROOT)

if(NOT ROOT_FOUND)
   message(FATAL_ERROR "Fatal error: ROOT package not found")
endif()

add_compile_options(-std=c++0x)

include_directories(
	${ROOT_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	contrib/star-base
	contrib/star-muDst
	contrib/star-stevent
	contrib/star-tables
)

add_library( star-vertex-eval SHARED VertexRank.C MuMcPrVKFV2012.C)

install(TARGETS star-vertex-eval DESTINATION "lib")
