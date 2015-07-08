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


# Add to path in order to pick up the FindXXX.cmake files included in this project
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

# This project depends on Boost
set(Boost_USE_MULTITHREADED OFF)
find_package( Boost 1.54.0 REQUIRED COMPONENTS program_options regex filesystem)

# This project depends on ROOT
find_package(ROOT)

if(NOT ROOT_FOUND)
   message(FATAL_ERROR "Fatal error: ROOT package not found")
endif()

add_definitions(-D__ROOT__)

# Some external files on which this project depends
file(COPY asps/rexe/TGeant3/TGiant3.h asps/rexe/TGeant3/StarMC.h DESTINATION inc/)


include_directories(
	${Boost_INCLUDE_DIR}
	${ROOT_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}/inc
	contrib/star-base
	contrib/star-bfchain
	contrib/star-daq
	contrib/star-db-calibrations
	contrib/star-emc
	contrib/star-geant
	contrib/star-hbt
	contrib/star-ist
	contrib/star-pxl
	contrib/star-RTS
	contrib/star-ssd
	contrib/star-stdb
	contrib/star-stevent
	contrib/star-sti
	contrib/star-svt
	contrib/star-tables
	contrib/star-tof
	contrib/star-tpc
	contrib/star-vertex
)

add_subdirectory(contrib/star-base)
add_subdirectory(contrib/star-bfchain)
add_subdirectory(contrib/star-sti)

add_subdirectory(StiRootIO)
add_subdirectory(GeaRootIO)
add_subdirectory(StiHify)
add_subdirectory(StiScan)


install(DIRECTORY "contrib/star-bfchain/StBFChain" DESTINATION "StRoot")
install(DIRECTORY "contrib/star-db-calibrations/tracker" DESTINATION "StarDb/Calibrations")


link_directories(
	${Boost_LIBRARY_DIR}
	${ROOT_LIBRARY_DIR}
)


configure_file(src-tools/config.h.in inc/src-tools/config.h)

add_executable(stiscan src-tools/stiscan.cc)
add_executable(stihify src-tools/stihify.cc)

target_link_libraries(stiscan
	${Boost_LIBRARIES} ${ROOT_LIBRARIES} Table Geom
	St_base StChain StarRoot StarClassLibrary StiUtilities Sti StiRootIO GeaRootIO StiScan
)

target_link_libraries(stihify
	${Boost_LIBRARIES} ${ROOT_LIBRARIES} Table Geom
	St_base StChain StarRoot StarClassLibrary StiUtilities Sti StiRootIO StiScan StiHify
)


set_target_properties(stiscan PROPERTIES LINK_FLAGS "-Wl,--warn-unresolved-symbols")
set_target_properties(stihify PROPERTIES LINK_FLAGS "-Wl,--warn-unresolved-symbols")
