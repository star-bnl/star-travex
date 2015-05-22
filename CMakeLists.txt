#
# Usage:
#
# cd <project_dir>
# mkdir build
# cd build
# cmake ../
# make
# make install


cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(star-sti-tools)

option(FORCE_32BIT "compile 32-bit application" ON)
option(Boost_NO_BOOST_CMAKE "Due to bug in boost cmake files in Scientific Linux" ON)

if(FORCE_32BIT)
   SET_PROPERTY(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS OFF)
endif()

set(CMAKE_CXX_FLAGS "-m32 -std=c++0x")

# Find boost headers and library locations
set(Boost_USE_MULTITHREADED OFF)
set(BOOST_INCLUDEDIR $ENV{OPTSTAR}/include)
set(BOOST_LIBRARYDIR $ENV{OPTSTAR}/lib)

# Add to path in order to pick up the FindXXX.cmake files included in this project
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

# This project depends on Boost
find_package( Boost 1.54.0 COMPONENTS program_options regex filesystem)

if(NOT Boost_FOUND)
   message(FATAL_ERROR "Fatal error: Boost (version >= 1.54.0) not found in standard location")
endif()

# This project depends on ROOT
find_package(ROOT)

if(NOT ROOT_FOUND)
   message(FATAL_ERROR "Fatal error: ROOT package not found")
endif()

include_directories(
	${Boost_INCLUDE_DIR}
	${ROOT_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	contrib
	contrib/star-sti
	contrib/star-stevent
	contrib/star-base
	contrib/star-base/St_base
	contrib/star-base/Star2Root
	$ENV{STAR}/.$ENV{STAR_HOST_SYS}/include
	$ENV{STAR}/StRoot #/StDetectorDbMaker
)

add_subdirectory(contrib/star-base)
add_subdirectory(contrib/star-bfchain)
add_subdirectory(contrib/star-sti)

add_subdirectory(StiRootIO)
add_subdirectory(GeaRootIO)
add_subdirectory(StiHify)
add_subdirectory(StiScan)


install(DIRECTORY contrib/star-bfchain/ DESTINATION "StRoot/StBFChain" FILES_MATCHING PATTERN "*.h" PATTERN "*.cxx" PATTERN "*.C")
install(DIRECTORY contrib/star-db-calibrations/ DESTINATION "StarDb/Calibrations")


link_directories(
	${Boost_LIBRARY_DIR}
	${ROOT_LIBRARY_DIR}
)


configure_file(src-tools/config.h.in src-tools/config.h)

add_executable(stiscan src-tools/stiscan.cc)
add_executable(stihify src-tools/stihify.cc)

target_link_libraries(stiscan
	${Boost_LIBRARIES} ${ROOT_LIBRARIES} Table Geom Eve
	St_base StChain StarRoot StarClassLibrary StiUtilities Sti StiRootIO GeaRootIO StiScan
)

target_link_libraries(stihify
	${Boost_LIBRARIES} ${ROOT_LIBRARIES} Table Geom Eve
	St_base StChain StarRoot StarClassLibrary StiUtilities Sti StiRootIO StiScan StiHify
)

if(FORCE_32BIT)
	set_target_properties(stiscan PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32 -Wl,--warn-unresolved-symbols")
	set_target_properties(stihify PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32 -Wl,--warn-unresolved-symbols")
endif()
