cmake_minimum_required( VERSION 3.2 FATAL_ERROR )

project( star-travex )

# Add to path in order to pick up the FindXXX.cmake files included in this project
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(ROOT COMPONENTS Table HistPainter Minuit Geom Spectrum EG MathMore)

if( ${ROOT_CXX_FLAGS} MATCHES "-m32" )
	message(STATUS "Found -m32 option in $ROOT_CXX_FLAGS (root-config). Will add it to $CMAKE_CXX_FLAGS")
	set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS FALSE)
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32" )
endif()

find_package(STAR)
find_package(Boost 1.54.0 COMPONENTS program_options filesystem)
find_package(MySQL)
find_package(LibXml2)
find_package(CURL)

set( CMAKE_CXX_STANDARD 11 )

# Some external files on which this project depends
file(COPY asps/rexe/TGeant3/TGiant3.h asps/rexe/TGeant3/StarMC.h DESTINATION inc/)


include_directories(
	${Boost_INCLUDE_DIR}
	${ROOT_INCLUDE_DIR}
	${MYSQL_INCLUDE_DIRS}
	${LIBXML2_INCLUDE_DIR}
	${STAR_INCLUDE_DIRS}
	${STAR_VERTEX_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}/inc
	ext/travex/include
)

add_subdirectory(ext/travex)

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
	VtxEval/VtxRecoProgramOptions.cxx
)



add_executable(stiscan src-tools/stiscan.cc)
add_executable(stihify src-tools/stihify.cc)

target_link_libraries(stiscan
	travex
	StiScanLib GeaRootIO StiRootIO
	${STAR_LIBRARIES} ${ROOT_LIBRARIES}
	${Boost_LIBRARIES}
	${CURL_LIBRARIES} ${MYSQL_LIBRARIES} ${LIBXML2_LIBRARIES}
)

target_link_libraries(stihify
	travex
	StiHifyLib StiScanLib GeaRootIO StiRootIO
	${STAR_LIBRARIES} ${ROOT_LIBRARIES}
	${Boost_LIBRARIES}
	${CURL_LIBRARIES} ${MYSQL_LIBRARIES} ${LIBXML2_LIBRARIES}
)


add_executable(vtxhist src-tools/vtxhist.cc)

target_link_libraries(vtxhist
	travex
	star-vertex-eval
	${STAR_LIBRARIES} ${ROOT_LIBRARIES}
	${Boost_LIBRARIES}
	${CURL_LIBRARIES} ${MYSQL_LIBRARIES} ${LIBXML2_LIBRARIES}
)


add_executable(vtxreco src-tools/vtxreco.cxx)

target_link_libraries(vtxreco
	travex
	star-vertex-eval
	${STAR_LIBRARIES} ${ROOT_LIBRARIES}
	${Boost_LIBRARIES}
	${CURL_LIBRARIES} ${MYSQL_LIBRARIES} ${LIBXML2_LIBRARIES}
)


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set_target_properties(stiscan PROPERTIES LINK_FLAGS "-Wl,--warn-unresolved-symbols")
	set_target_properties(stihify PROPERTIES LINK_FLAGS "-Wl,--warn-unresolved-symbols")
	set_target_properties(vtxhist PROPERTIES LINK_FLAGS "-Wl,--warn-unresolved-symbols")
	set_target_properties(vtxreco PROPERTIES LINK_FLAGS "-Wl,--warn-unresolved-symbols")
endif()


# Installation section

install(TARGETS
	StiRootIO GeaRootIO AgUStep StiHifyLib StiScanLib
	star-vertex-eval StMcAnalysisMaker
	DESTINATION "${STAR_ADDITIONAL_INSTALL_PREFIX}/lib" OPTIONAL)

install(TARGETS stiscan stihify vtxhist vtxreco
	DESTINATION "${STAR_ADDITIONAL_INSTALL_PREFIX}/bin" OPTIONAL)
