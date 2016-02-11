# Basic usage:
#
# cd <project_dir>
# mkdir build && cd build
# cmake ../
# make
# make install

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(star-sti-tools)


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

add_definitions(-D__ROOT__)


# Some external files on which this project depends
file(COPY asps/rexe/TGeant3/TGiant3.h asps/rexe/TGeant3/StarMC.h DESTINATION inc/)


include_directories(
	${Boost_INCLUDE_DIR}
	${ROOT_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}/inc
	ext/star-base
	ext/star-bfchain
	ext/star-daq
	ext/star-db-calibrations
	ext/star-emc
	ext/star-geant
	ext/star-ist
	ext/star-pxl
	ext/star-RTS
	ext/star-ssd
	ext/star-stdb
	ext/star-stevent
	ext/star-sti
	ext/star-svt
	ext/star-tables
	ext/star-tof
	ext/star-tpc
	ext/star-vertex
	ext/star-pams/global/inc   # required by star-vertex
)

add_subdirectory(ext/star-base)
add_subdirectory(ext/star-sti)
add_subdirectory(ext/star-bfchain)

root_generate_dictionary(
	StiRootIO_dict
	StiRootIO/H2D.h
	StiRootIO/H3D.h
	StiRootIO/Profile2D.h
	StiRootIO/Profile3D.h
	StiRootIO/StiTreeMaker.h
	StiRootIO/TStiEvent.h
	StiRootIO/TStiHit.h
	StiRootIO/TStiHitProxy.h
	StiRootIO/TStiKalmanTrack.h
	StiRootIO/TStiKalmanTrackNode.h
	LINKDEF StiRootIO/StiRootIOLinkDef.h
)

root_generate_dictionary(
	GeaRootIO_dict
	GeaRootIO/TGeaEvent.h
	LINKDEF GeaRootIO/GeaRootIOLinkDef.h
)

root_generate_dictionary(
	AgUStep_dict
	GeaRootIO/AgUStep.h
	LINKDEF GeaRootIO/AgUStepLinkDef.h
)

root_generate_dictionary(
	StiHify_dict
	StiHify/StiHifyEvent.h
	StiHify/StiHifyTreeMaker.h
	LINKDEF StiHify/StiHifyLinkDef.h
)

root_generate_dictionary(
	StiScan_dict
	StiScan/StiScanEvent.h
	StiScan/StiScanTreeMaker.h
	LINKDEF StiScan/StiScanLinkDef.h
)

add_library(
	StiRootIO
	SHARED
	StiRootIO/H2D.cxx
	StiRootIO/H3D.cxx
	StiRootIO/PrgOptionProcessor.cxx
	StiRootIO/Profile2D.cxx
	StiRootIO/Profile3D.cxx
	StiRootIO/StiHistContainer.cxx
	StiRootIO/StiRootFile.cxx
	StiRootIO/StiTreeMaker.cxx
	StiRootIO/StiVolumeFilter.cxx
	StiRootIO/TStiEvent.cxx
	StiRootIO/TStiHit.cxx
	StiRootIO/TStiHitProxy.cxx
	StiRootIO/TStiKalmanTrack.cxx
	StiRootIO/TStiKalmanTrackNode.cxx
	StiRootIO_dict.cxx
)

add_library( GeaRootIO SHARED GeaRootIO/TGeaEvent.cxx GeaRootIO_dict.cxx)
add_library( AgUStep SHARED GeaRootIO/AgUStep.cxx AgUStep_dict.cxx)

add_library(
	StiHify
	SHARED
	StiHify/StiHifyEvent.cxx
	StiHify/StiHifyTreeMaker.cxx
	StiHify/StiHifyHistContainer.cxx
	StiHify/StiHifyRatiosHistContainer.cxx
	StiHify/StiHifyPrgOptions.cxx
	StiHify/StiHifyRootFile.cxx
	StiHify_dict.cxx
)

add_library(
	StiScan
	SHARED
	StiScan/StiScanEvent.cxx
	StiScan/StiScanTreeMaker.cxx
	StiScan/StiScanHistContainer.cxx
	StiScan/StiScanRatiosHistContainer.cxx
	StiScan/StiScanHistsByVolume.cxx
	StiScan/StiScanPrgOptions.cxx
	StiScan/StiScanRootFile.cxx
	StiScan_dict.cxx
)


link_directories( ${Boost_LIBRARY_DIR} ${ROOT_LIBRARY_DIR} )

configure_file(src-tools/config.h.in inc/src-tools/config.h)

add_executable(sti-scan src-tools/stiscan.cc)
add_executable(sti-hify src-tools/stihify.cc)

target_link_libraries(sti-scan
	StiScan GeaRootIO StiRootIO Sti StiUtilities St_base StChain StarRoot StarClassLibrary StarMagField
	${Boost_LIBRARIES} ${ROOT_LIBRARIES} Table Geom HistPainter EG pthread
)

target_link_libraries(sti-hify
	StiHify StiScan GeaRootIO StiRootIO Sti StiUtilities St_base StChain StarRoot StarClassLibrary StarMagField
	${Boost_LIBRARIES} ${ROOT_LIBRARIES} Table Geom HistPainter EG pthread
)


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set_target_properties(sti-scan PROPERTIES LINK_FLAGS "-Wl,--warn-unresolved-symbols")
	set_target_properties(sti-hify PROPERTIES LINK_FLAGS "-Wl,--warn-unresolved-symbols")
endif()


# Installation section
install(DIRECTORY "ext/star-db-calibrations/tracker" DESTINATION "StarDb/Calibrations")
install(TARGETS StiRootIO GeaRootIO AgUStep StiHify StiScan DESTINATION "${STAR_ADDITIONAL_INSTALL_PREFIX}/lib")
install(TARGETS sti-scan sti-hify DESTINATION "${STAR_ADDITIONAL_INSTALL_PREFIX}/bin")
