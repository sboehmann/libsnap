project( libsnap )

add_subdirectory( tests )

set( CMAKE_C_FLAGS "-std=c99 -Wall -Wextra -pedantic" )

set( libsnap_SOURCES
    snap.c
    compress.c
    edm.c
    util.c
 ) 


set( libsnap_HEADERS
    global.h
    compress.h
    edm.h
    util.h
    snap.h
  )

add_library(snap SHARED ${libsnap_SOURCES})
set_target_properties( snap PROPERTIES VERSION "${snap_MAJOR_VERSION}.${snap_MINOR_VERSION}.${snap_PATCH_LEVEL}" SOVERSION "${snap_MAJOR_VERSION}.${snap_MINOR_VERSION}")

# Cannot have two targets with the same name so the static version has
# '-static' appended and then the name of the output file is set separately.
add_library(snap-static STATIC ${libsnap_SOURCES})
set_target_properties(snap-static PROPERTIES OUTPUT_NAME "snap")

install( TARGETS snap snap-static DESTINATION ${SNAP_LIB_INSTALL_DIR} )
install(FILES ${libsnap_HEADERS} DESTINATION ${SNAP_INCLUDE_INSTALL_DIR}/libsnap COMPONENT Devel )