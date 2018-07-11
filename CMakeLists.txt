cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(star-travex LANGUAGES CXX)

# Add to path in order to pick up the FindXXX.cmake files included in this project
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# By default build shared libraries but allow the user to change if desired
OPTION(BUILD_SHARED_LIBS "Global flag to cause add_library to create shared libraries if on" ON)


find_package(ROOT COMPONENTS Cint Table HistPainter Minuit Geom Spectrum EG MathMore Eve RGL)

if( ${ROOT_CXX_FLAGS} MATCHES "-m32" )
	message(STATUS "Found -m32 option in $ROOT_CXX_FLAGS (root-config). Will add it to $CMAKE_CXX_FLAGS")
	set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS FALSE)
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32" )
	set(ENV{PKG_CONFIG_PATH} "/usr/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
endif()

find_package(STAR)
find_package(Boost 1.54.0 COMPONENTS filesystem program_options regex system)
find_package(MySQL)
find_package(LibXml2)
find_package(CURL)

set(CMAKE_CXX_STANDARD 11)

# Some external files on which this project depends
file(COPY asps/rexe/TGeant3/TGiant3.h asps/rexe/TGeant3/StarMC.h DESTINATION inc/)


include_directories(
	${Boost_INCLUDE_DIR}
	${ROOT_INCLUDE_DIR}
	${MYSQL_INCLUDE_DIRS}
	${LIBXML2_INCLUDE_DIR}
	${STAR_INCLUDE_DIRS}
	${STAR_VERTEX_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_BINARY_DIR}/inc
	ext/travex/include
)

include(cmake/ExternalProject_star-vertex.cmake)
include(cmake/ExternalProject_trackml.cmake)

add_subdirectory(ext/travex)

root_generate_dictionary(
	stirootio_dict
	stirootio/H2D.h
	stirootio/H3D.h
	stirootio/Profile2D.h
	stirootio/Profile3D.h
	stirootio/StiTreeMaker.h
	stirootio/TStiEvent.h
	stirootio/TStiHit.h
	stirootio/TStiHitProxy.h
	stirootio/TStiKalmanTrack.h
	stirootio/TStiKalmanTrackNode.h
	LINKDEF stirootio/stirootioLinkDef.h
)

root_generate_dictionary(
	gearootio_dict
	gearootio/TGeaEvent.h
	LINKDEF gearootio/LinkDef.h
)

root_generate_dictionary(
	AgUStep_dict
	GeaRootIO/AgUStep.h
	LINKDEF GeaRootIO/AgUStepLinkDef.h
)

root_generate_dictionary(
	stihify_dict
	stihify/StiHifyEvent.h
	stihify/StiHifyTreeMaker.h
	LINKDEF stihify/StiHifyLinkDef.h
)

root_generate_dictionary(
	stiscan_dict
	stiscan/StiScanEvent.h
	stiscan/StiScanTreeMaker.h
	LINKDEF stiscan/StiScanLinkDef.h
)

root_generate_dictionary(
	StMcAnalysisMaker_dict
	stihify/StMcAnalysisMaker.h
	LINKDEF stihify/StMcAnalysisMakerLinkDef.h
)

add_library(stirootiolib
	src/stirootio/H2D.cxx
	src/stirootio/H3D.cxx
	src/stirootio/PrgOptionProcessor.cxx
	src/stirootio/Profile2D.cxx
	src/stirootio/Profile3D.cxx
	src/stirootio/StiTreeMaker.cxx
	src/stirootio/StiVolumeFilter.cxx
	src/stirootio/TStiEvent.cxx
	src/stirootio/TStiHit.cxx
	src/stirootio/TStiHitProxy.cxx
	src/stirootio/TStiKalmanTrack.cxx
	src/stirootio/TStiKalmanTrackNode.cxx
	stirootio_dict.cxx
)

add_library(gearootiolib src/gearootio/TGeaEvent.cxx gearootio_dict.cxx)
add_library( AgUStep GeaRootIO/AgUStep.cxx AgUStep_dict.cxx)

add_library(
	StMcAnalysisMaker
	src/stihify/StMcAnalysisMaker.cxx
	StMcAnalysisMaker_dict.cxx
)

add_library(
	stihifylib
	src/stihify/StiHifyEvent.cxx
	src/stihify/StiHifyTreeMaker.cxx
	src/stihify/StiHifyHistContainer.cxx
	src/stihify/StiHifyAnalysisTreeMaker.cxx
	src/stihify/StiHifyRatiosHistContainer.cxx
	src/stihify/StiHifyPrgOptions.cxx
	src/stihify/StiHifyRootFile.cxx
	stihify_dict.cxx
)

add_library(
	stiscanlib
	src/stiscan/StiScanEvent.cxx
	src/stiscan/StiScanTreeMaker.cxx
	src/stiscan/StiScanHistContainer.cxx
	src/stiscan/StiScanRatiosHistContainer.cxx
	src/stiscan/StiScanHistsByVolume.cxx
	src/stiscan/StiScanPrgOptions.cxx
	src/stiscan/StiScanRootFile.cxx
	stiscan_dict.cxx
)


add_executable(stiscan src/stiscan//stiscan.cxx)
add_executable(stihify src/stihify/stihify.cxx)

target_link_libraries(stiscan
	travex
	stiscanlib gearootiolib stirootiolib
	${STAR_LIBRARIES} ${ROOT_LIBRARIES}
	${Boost_LIBRARIES}
	${CURL_LIBRARIES} ${MYSQL_LIBRARIES} ${LIBXML2_LIBRARIES}
	stiglob
)

target_link_libraries(stihify
	travex
	stihifylib stiscanlib gearootiolib stirootiolib
	${STAR_LIBRARIES} ${ROOT_LIBRARIES}
	${Boost_LIBRARIES}
	${CURL_LIBRARIES} ${MYSQL_LIBRARIES} ${LIBXML2_LIBRARIES}
	stiglob
)


add_executable(vtxhist
	src/vtxeval/vtxhist.cxx
	src/vtxeval/StarEventHistContainer.cxx
	src/vtxeval/StarVertexHistContainer.cxx
	src/vtxeval/StarVertexHftHistContainer.cxx
	src/vtxeval/DecayVertexHists.cxx
	src/vtxeval/VertexRootFile.cxx
)
target_include_directories(vtxhist PRIVATE ${STAR_VERTEX_INCLUDE_DIR})
add_dependencies(vtxhist star-vertex)
set(star_libraries_except_vertex ${STAR_LIBRARIES})
list(FILTER star_libraries_except_vertex EXCLUDE REGEX StGenericVertexMaker)
target_link_libraries(vtxhist
	travex
	${STAR_VERTEX_LIBRARIES}
	${star_libraries_except_vertex} ${ROOT_LIBRARIES}
	${Boost_LIBRARIES}
	${CURL_LIBRARIES} ${MYSQL_LIBRARIES} ${LIBXML2_LIBRARIES}
	stiglob
)


add_executable(vtxreco
	src/vtxeval/vtxreco.cxx
	src/vtxeval/VtxRecoProgramOptions.cxx
)
target_link_libraries(vtxreco
	travex
	${STAR_LIBRARIES} ${ROOT_LIBRARIES}
	${Boost_LIBRARIES}
	${CURL_LIBRARIES} ${MYSQL_LIBRARIES} ${LIBXML2_LIBRARIES}
	stiglob
)


add_executable(stargeo src/geo/stargeo.cxx)

target_link_libraries(stargeo
	${STAR_LIBRARY_StarRoot}
	${STAR_LIBRARY_StChain}
	${STAR_LIBRARY_StUtilities}
	${STAR_LIBRARY_St_base}
	${STAR_LIBRARY_StarGeometry}
	${STAR_LIBRARY_StarAgmlLib}
	${STAR_LIBRARY_StarAgmlUtil}
	${ROOT_LIBRARIES}
	${Boost_LIBRARIES}
)


add_executable(stigeo src/geo/stigeo.cxx)

target_link_libraries(stigeo
	${STAR_LIBRARIES} ${ROOT_LIBRARIES}
	${CURL_LIBRARIES} ${MYSQL_LIBRARIES} ${LIBXML2_LIBRARIES}
	stiglob
)


add_executable(evedisp
	src/evedisp/evedisp.cxx
	src/evedisp/EventReader.cxx
	src/evedisp/ProgramOptions.cxx
	src/geo/common.cxx
	src/common/root_v6.cxx
)
target_include_directories(evedisp PRIVATE ${TRACKML_INCLUDE_DIR})
add_dependencies(evedisp trackml)
target_link_libraries(evedisp
	${TRACKML_LIBRARIES}
	${STAR_LIBRARIES} ${ROOT_LIBRARIES}
	${Boost_LIBRARIES}
	${CURL_LIBRARIES} ${MYSQL_LIBRARIES} ${LIBXML2_LIBRARIES}
	stiglob
)


add_executable(tgeo2shapes
	src/geo/tgeo2shapes.cxx
	src/geo/ProgramOptions.cxx
	src/geo/common.cxx
	src/common/root_v6.cxx
)

target_link_libraries(tgeo2shapes ${ROOT_LIBRARIES} ${Boost_LIBRARIES})


add_library( stiglob src/stiglob.cxx )


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set_target_properties(stiscan PROPERTIES LINK_FLAGS "-Wl,--warn-unresolved-symbols")
	set_target_properties(stihify PROPERTIES LINK_FLAGS "-Wl,--warn-unresolved-symbols")
	set_target_properties(vtxhist PROPERTIES LINK_FLAGS "-Wl,--warn-unresolved-symbols")
	set_target_properties(vtxreco PROPERTIES LINK_FLAGS "-Wl,--warn-unresolved-symbols")
	set_target_properties(stigeo  PROPERTIES LINK_FLAGS "-Wl,--warn-unresolved-symbols")
endif()


# Installation section

install(TARGETS
	stirootiolib gearootiolib AgUStep stihifylib stiscanlib
	StMcAnalysisMaker
	DESTINATION "${STAR_ADDITIONAL_INSTALL_PREFIX}/lib" OPTIONAL)

install(TARGETS stiscan stihify vtxhist vtxreco evedisp
	DESTINATION "${STAR_ADDITIONAL_INSTALL_PREFIX}/bin" OPTIONAL)
