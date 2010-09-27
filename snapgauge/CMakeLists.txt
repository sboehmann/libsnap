project( snapgauge )

set( QT_MIN_VERSION 4.6.0 )

find_package( Qt4 REQUIRED )
find_package( KDE4 REQUIRED )

include( KDE4Defaults )
include( MacroLibrary )

add_definitions( ${QT_DEFINITIONS} ${QT_QTDBUS_DEFINITIONS} ${KDE4_DEFINITIONS} )

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_BINARY_DIR}/..
    ${QT_INCLUDES}
    ${KDE4_INCLUDES}
#    ${CMAKE_SOURCE_DIR}/libsnap
)

# The FindKDE4.cmake module sets _KDE4_PLATFORM_DEFINITIONS with
# definitions like _GNU_SOURCE that are needed on each platform.
# Put configure checks below this line.
set( CMAKE_REQUIRED_DEFINITIONS ${_KDE4_PLATFORM_DEFINITIONS} )

add_subdirectory( data )
add_subdirectory( src )