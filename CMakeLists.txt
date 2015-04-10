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

# Set path to installation directory
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR} CACHE PATH "Default install prefix" FORCE)
else(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set(MY_TEMP_CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
	unset(CMAKE_INSTALL_PREFIX CACHE)
	set(CMAKE_INSTALL_PREFIX ${MY_TEMP_CMAKE_INSTALL_PREFIX})
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

option(FORCE_32BIT "compile 32-bit application" ON)
option(Boost_NO_BOOST_CMAKE "Due to bug in boost cmake files in Scientific Linux" ON)

if(FORCE_32BIT)
   SET_PROPERTY(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS OFF)
endif()

set(CMAKE_CXX_FLAGS "-std=c++0x")

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

add_subdirectory(contrib/star-sti)
add_subdirectory(contrib/star-sim)

include_directories(
	${Boost_INCLUDE_DIR}
	${ROOT_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	contrib/star-sti
	$ENV{STAR}/StRoot
	$ENV{STAR}/StRoot/St_base
	$ENV{STAR}/StRoot/Star2Root
	$ENV{STAR}/.$ENV{STAR_HOST_SYS}/include
)

add_subdirectory(StiRootIO)
