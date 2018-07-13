include(ExternalProject)

ExternalProject_Add(
	travex
	PREFIX "external/"
	GIT_REPOSITORY "https://github.com/plexoos/travex.git"
	GIT_TAG "master"
	GIT_SHALLOW 1
	UPDATE_COMMAND ""
	PATCH_COMMAND ""
	INSTALL_COMMAND ""
	CMAKE_ARGS -DBOOST_ROOT=${BOOST_ROOT}
)

set(TRAVEX_INCLUDE_DIR "${CMAKE_BINARY_DIR}/external/src/travex/include")
set(TRAVEX_LIBRARIES travex)

set(_dir "${CMAKE_BINARY_DIR}/external/src/travex-build")
set(_pfx "${CMAKE_SHARED_LIBRARY_PREFIX}")
set(_sfx "${CMAKE_SHARED_LIBRARY_SUFFIX}")

if(NOT ${BUILD_SHARED_LIBS})
	set(_pfx ${CMAKE_STATIC_LIBRARY_PREFIX})
	set(_sfx ${CMAKE_STATIC_LIBRARY_SUFFIX})
endif()

# Add prefix and suffix to each library
string(REGEX REPLACE "([^;]+);" "${_dir}/${_pfx}\\1${_sfx};" TRAVEX_LIBRARIES "${TRAVEX_LIBRARIES};")

unset(_dir)
unset(_pfx)
unset(_sfx)
