include(ExternalProject)

ExternalProject_Add(
	trackml
	PREFIX "external/"
	GIT_REPOSITORY "https://oauth2:FysiWhxcrY8ZBShFsQy9@gitlab.com/plexoos/trackml-challenge.git"
	GIT_TAG "master"
	GIT_SHALLOW 1
	UPDATE_COMMAND ""
	PATCH_COMMAND ""
	INSTALL_COMMAND ""
)

set(TRACKML_INCLUDE_DIR "${CMAKE_BINARY_DIR}/external/src/trackml/src")
set(TRACKML_LIBRARIES tml)

set(_dir "${CMAKE_BINARY_DIR}/external/src/trackml-build")
set(_pfx "${CMAKE_SHARED_LIBRARY_PREFIX}")
set(_sfx "${CMAKE_SHARED_LIBRARY_SUFFIX}")

if(NOT ${BUILD_SHARED_LIBS})
	set(_pfx ${CMAKE_STATIC_LIBRARY_PREFIX})
	set(_sfx ${CMAKE_STATIC_LIBRARY_SUFFIX})
endif()

# Add prefix and suffix to each library
string(REGEX REPLACE "([^;]+);" "${_dir}/${_pfx}\\1${_sfx};" TRACKML_LIBRARIES "${TRACKML_LIBRARIES};")

unset(_dir)
unset(_pfx)
unset(_sfx)
