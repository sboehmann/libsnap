project( libsnap++ )

set( CMAKE_C_FLAGS "-std=c99 -Wall -Wextra -pedantic" )

set( libsnap_SOURCES     ../libsnap/snap.c ../libsnap/edm.c ../libsnap/compress.c ../libsnap/util.c )
set( libsnapxx_SOURCES   snap.cpp statistic.cpp ) 
set( libsnapxx_HEADERS   snap.h   statistic.h   )

add_library(snap++ SHARED ${libsnapxx_SOURCES} )
target_link_libraries( snap++ snap )
set_target_properties( snap++ PROPERTIES VERSION "${snap_MAJOR_VERSION}.${snap_MINOR_VERSION}.${snap_PATCH_LEVEL}" SOVERSION "${snap_MAJOR_VERSION}.${snap_MINOR_VERSION}")

# Cannot have two targets with the same name so the static version has
# '-static' appended and then the name of the output file is set separately.
add_library( snapxx-static STATIC ${libsnapxx_SOURCES} ${libsnap_SOURCES} )
#target_link_libraries( snapxx-static libsnap.a )
set_target_properties( snapxx-static PROPERTIES OUTPUT_NAME "snap++" )

install( TARGETS snap++ snapxx-static DESTINATION ${SNAP_LIB_INSTALL_DIR} )
install(FILES ${libsnapxx_HEADERS} DESTINATION ${SNAP_INCLUDE_INSTALL_DIR}/libsnap++ COMPONENT Devel )