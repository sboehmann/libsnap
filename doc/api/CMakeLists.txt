project( apidoc )

set( DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/api" )

include( MacroLogFeature )

find_package( Doxygen )
MACRO_LOG_FEATURE( DOXYGEN_FOUND "Doxygen" "Generate Doxygen based API documentation" "http://www.doxygen.org" FALSE "1.6.0" "")

if( DOXYGEN_FOUND )
  configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile ESCAPE_QUOTES IMMEDIATE @ONLY )
  add_custom_target( apidoc ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile )
  set_property( DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${DOXYGEN_OUTPUT_DIR} )

  install(CODE "execute_process(COMMAND ${CMAKE_BUILD_TOOL} apidoc)")
  install(DIRECTORY ${DOXYGEN_OUTPUT_DIR} DESTINATION ${SNAP_DOC_INSTALL_DIR}/html )

  file(GLOB image_SRCS "*.png")
  install(FILES ${image_SRCS} DESTINATION ${SNAP_DOC_INSTALL_DIR}/html/api )
endif( DOXYGEN_FOUND )
