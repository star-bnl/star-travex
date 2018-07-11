include(ExternalProject)

ExternalProject_Add(
	star-vertex
	PREFIX "external/"
	GIT_REPOSITORY "https://github.com/star-bnl/star-vertex.git"
	GIT_TAG "master"
	GIT_SHALLOW 1
	UPDATE_COMMAND ""
	PATCH_COMMAND ""
	# Should be a portable command but gives a warning
	# warning: jobserver unavailable: using -j1.  Add `+' to parent make rule.
	INSTALL_COMMAND ${CMAKE_COMMAND} --build . --target install
	#INSTALL_COMMAND $(MAKE) install
	CMAKE_ARGS -DSTAR_ROOT=${STAR_ROOT} -DCMAKE_INSTALL_PREFIX=./
)

set(STAR_VERTEX_INCLUDE_DIR "${CMAKE_BINARY_DIR}/external/src/star-vertex")
set(STAR_VERTEX_LIBRARIES StGenericVertexMaker StSecondaryVertexMaker StVertexRootIO StZdcVertexMaker)

set(_dir "${CMAKE_BINARY_DIR}/external/src/star-vertex-build/${STAR_ADDITIONAL_INSTALL_PREFIX}/lib")
set(_pfx "${CMAKE_SHARED_LIBRARY_PREFIX}")
set(_sfx "${CMAKE_SHARED_LIBRARY_SUFFIX}")

if(NOT ${BUILD_SHARED_LIBS})
	set(_pfx ${CMAKE_STATIC_LIBRARY_PREFIX})
	set(_sfx ${CMAKE_STATIC_LIBRARY_SUFFIX})
endif()

# Add prefix and suffix to each library
string(REGEX REPLACE "([^;]+);" "${_dir}/${_pfx}\\1${_sfx};" STAR_VERTEX_LIBRARIES "${STAR_VERTEX_LIBRARIES};")

unset(_dir)
unset(_pfx)
unset(_sfx)
