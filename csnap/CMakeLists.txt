project( csnap )

set( CMAKE_C_FLAGS "-std=c99 -Wall -Wextra -pedantic" )

set( csnap_SOURCES color.c message.c util.c main.c )

add_executable( csnap ${csnap_SOURCES} )
target_link_libraries( csnap snap )

install( TARGETS csnap DESTINATION ${SNAP_BIN_INSTALL_DIR} )