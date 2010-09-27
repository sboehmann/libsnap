project( snap )

set( snap_MAJOR_VERSION 0 )
set( snap_MINOR_VERSION 2 )
set( snap_PATCH_LEVEL   0 )

ENABLE_TESTING()

#############################################################################################

cmake_minimum_required( VERSION 2.8.0 FATAL_ERROR )

# -DWITH_ZLIB -DWITH_LZMA = -lz -llzma

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb3")
#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g3 -ggdb3")

set( CMAKE_COLOR_MAKEFILE ON )
set( CMAKE_VERBOSE_MAKEFILE OFF )
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON )
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

include(cmake/config-destination.cmake)
include(cmake/config-cpack.cmake)

include(MacroLogFeature)
include(MacroOptionalAddSubdirectory)
include(MacroEnsureOutOfSourceBuild)

macro_ensure_out_of_source_build( "SNAP requires an out of source build. Please create a separate build directory and run 'cmake path_to_sources [options]' there." )

set( snap_SHORT_VERSION  "${snap_MAJOR_VERSION}.${snap_MINOR_VERSION}" )
set( snap_VERSION        "${snap_SHORT_VERSION}.${snap_PATCH_LEVEL}" )
set( snap_VERSION_STRING "${snap_VERSION}" )

add_definitions("-DSNAP_MAJOR_VERSION=${snap_MAJOR_VERSION}")
add_definitions("-DSNAP_MINOR_VERSION=${snap_MINOR_VERSION}")
add_definitions("-DSNAP_PATCH_LEVEL=${snap_PATCH_LEVEL}")
add_definitions("-DSNAP_SHORT_VERSION=\"${snap_SHORT_VERSION}\"")
add_definitions("-DSNAP_VERSION=\"${snap_VERSION}\"")

## rpath handling ##
set( CMAKE_SKIP_BUILD_RPATH FALSE )                # don't skip the full RPATH for the build tree
set( CMAKE_BUILD_WITH_INSTALL_RPATH FALSE )        # when building, don't use the install RPATH already (but later on when installing)
set( CMAKE_INSTALL_RPATH ${SNAP_LIB_INSTALL_DIR} ) # the RPATH to be used when installing
set( CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE )      # add the automatically determined parts of the RPATH


include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

macro_optional_add_subdirectory( cmake )
macro_optional_add_subdirectory( libsnap )
macro_optional_add_subdirectory( doc )
macro_optional_add_subdirectory( examples )
macro_optional_add_subdirectory( libsnap++ )
macro_optional_add_subdirectory( csnap )
macro_optional_add_subdirectory( snapgauge )

macro_display_feature_log()
