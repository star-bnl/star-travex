# Basic usage:
#
# cd <project_dir>
# mkdir build && cd build
# cmake ../
# make
# make install

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(star-travex)


# Add to path in order to pick up the FindXXX.cmake files included in this project
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/star-cmake")

# This project depends on Boost
find_package( Boost 1.54.0 REQUIRED COMPONENTS program_options regex filesystem)

# This project depends on ROOT
find_package(ROOT COMPONENTS Table HistPainter Minuit Geom) # EG needed?

if(NOT ROOT_FOUND)
   message(FATAL_ERROR "Fatal error: ROOT package not found")
endif()

# Perform some setup standard to STAR experiment environment
include(StarCommon)

# Search for mysql package after StarCommon is loaded as there some properties
# set to help choose the appropriate version
find_package(MySQL)
find_package(LibXml2)

if(NOT LIBXML2_FOUND)
	message(FATAL_ERROR "Fatal error: LibXml2 package not found")
endif()

add_definitions(-D__ROOT__)

# Remove dependency of "install" target on "all" target. This allows to
# build and install individual libraries
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)


# Some external files on which this project depends
file(COPY asps/rexe/TGeant3/TGiant3.h asps/rexe/TGeant3/StarMC.h DESTINATION inc/)


include_directories(
	${Boost_INCLUDE_DIR}
	${ROOT_INCLUDE_DIR}
	${MYSQL_INCLUDE_DIRS}
	${LIBXML2_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}/inc
	ext/travex/include
	ext/star-base
	ext/star-bfchain
	ext/star-daq
	ext/star-muDst
	ext/star-db-calibrations
	ext/star-emc
	ext/star-fgt               # required by star-muDst
	ext/star-geant
	ext/star-ist
	ext/star-pxl
	ext/star-RTS
	ext/star-ssd
	ext/star-stdb
	ext/star-stevent
	ext/star-sti
	ext/star-ca-tracker        # required by star-sti
	ext/star-svt
	ext/star-tables
	ext/star-tables/tables     # required by star-stevent
	ext/star-sim               # required by star-muDst
	ext/star-tof
	ext/star-tpc
	ext/star-ftpc              # required by star-stevent
	ext/star-vertex
	ext/star-pams/global/inc   # required by star-emc
)

add_subdirectory(ext/travex)
add_subdirectory(ext/star-vertex)
add_subdirectory(ext/star-base)
add_subdirectory(ext/star-sti)
add_subdirectory(ext/star-bfchain)
add_subdirectory(ext/star-muDst)
add_subdirectory(ext/star-pxl)
add_subdirectory(ext/star-stevent)
add_subdirectory(ext/star-emc)
add_subdirectory(ext/star-sim)
add_subdirectory(ext/star-trigger)
add_subdirectory(ext/star-tables)
add_subdirectory(ext/star-stdb)
add_subdirectory(ext/star-tof)

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

root_generate_dictionary(
	StMcAnalysisMaker_dict
	StiHify/StMcAnalysisMaker.h
	LINKDEF StiHify/StMcAnalysisMakerLinkDef.h
)

add_library(
	StiRootIO
	SHARED
	StiRootIO/H2D.cxx
	StiRootIO/H3D.cxx
	StiRootIO/PrgOptionProcessor.cxx
	StiRootIO/Profile2D.cxx
	StiRootIO/Profile3D.cxx
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
	StMcAnalysisMaker
	SHARED
	StiHify/StMcAnalysisMaker.cxx
	StMcAnalysisMaker_dict.cxx
)

add_library(
	StiHifyLib
	SHARED
	StiHify/StiHifyEvent.cxx
	StiHify/StiHifyTreeMaker.cxx
	StiHify/StiHifyHistContainer.cxx
	StiHify/StiHifyAnalysisTreeMaker.cxx
	StiHify/StiHifyRatiosHistContainer.cxx
	StiHify/StiHifyPrgOptions.cxx
	StiHify/StiHifyRootFile.cxx
	StiHify_dict.cxx
)

add_library(
	StiScanLib
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


add_library(
	star-vertex-eval
	SHARED
	VtxEval/StarEventHistContainer.cxx
	VtxEval/StarVertexHistContainer.cxx
	VtxEval/StarVertexHftHistContainer.cxx
	VtxEval/DecayVertexHists.cxx
	VtxEval/VertexRootFile.cxx
)


link_directories( ${Boost_LIBRARY_DIR} ${ROOT_LIBRARY_DIR} )

configure_file(src-tools/config.h.in inc/src-tools/config.h)

add_executable(stiscan src-tools/stiscan.cc)
add_executable(stihify src-tools/stihify.cc)

target_link_libraries(stiscan
	travex
	StiScanLib GeaRootIO StiRootIO Sti StiUtilities St_base StChain StarRoot StarClassLibrary StarMagField
	${Boost_LIBRARIES} ${ROOT_LIBRARIES} pthread
)

target_link_libraries(stihify
	travex
	StiHifyLib StiScanLib GeaRootIO StiRootIO Sti StiUtilities St_base StChain StarRoot StarClassLibrary StarMagField
	${Boost_LIBRARIES} ${ROOT_LIBRARIES} pthread
)


add_executable(vtxhist src-tools/vtxhist.cc)

target_link_libraries(vtxhist
	travex
	star-vertex-eval
	StTableUtilities
	StarRoot
	St_base
	StChain
	St_Tables
	StUtilities
	StIOMaker
	StarClassLibrary
	StTriggerDataMaker
	StEvent
	StDbLib
	StEmcUtil
	StStrangeMuDstMaker
	StBichsel
	StSecondaryVertexMaker
	StVertexRootIO
	StMuDSTMaker
)

add_executable(vtxreco src-tools/vtxreco.cxx)

target_link_libraries(vtxreco
	travex
	star-vertex-eval
	StarRoot
	StChain
	St_Tables
	StUtilities
	StarClassLibrary
	StTriggerDataMaker
	StEvent
	StEmcUtil
	StEEmcUtil
	St_Tables
	StStrangeMuDstMaker
	StBichsel
	StMuDSTMaker
	StStrangeMuDstMaker
	St_base           # Has to go last to avoid no dictionary for StObject available
	Sti
	StGenericVertexMaker
	Sti
	St_db_Maker
	StDbBroker
	StDbLib
	StBTofUtil        # required by mudst or stevent
	StTableUtilities  # required by mudst or stevent
	St_base
	StChain
	StarRoot
	StarClassLibrary
	${Boost_LIBRARIES} ${ROOT_LIBRARIES} ${MYSQL_LIBRARIES} ${LIBXML2_LIBRARIES} pthread
)


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set_target_properties(stiscan PROPERTIES LINK_FLAGS "-Wl,--warn-unresolved-symbols")
	set_target_properties(stihify PROPERTIES LINK_FLAGS "-Wl,--warn-unresolved-symbols")
	set_target_properties(vtxhist PROPERTIES LINK_FLAGS "-Wl,--warn-unresolved-symbols")
	set_target_properties(vtxreco PROPERTIES LINK_FLAGS "-Wl,--warn-unresolved-symbols")
endif()


# Installation section
install(DIRECTORY "ext/star-db-calibrations/tracker" DESTINATION "StarDb/Calibrations")

install(TARGETS
	StiRootIO GeaRootIO AgUStep StiHifyLib StiScanLib
	star-vertex-eval StMcAnalysisMaker
	DESTINATION "${STAR_ADDITIONAL_INSTALL_PREFIX}/lib" OPTIONAL)

install(TARGETS stiscan stihify vtxhist vtxreco
	DESTINATION "${STAR_ADDITIONAL_INSTALL_PREFIX}/bin" OPTIONAL)
