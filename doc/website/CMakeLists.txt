project( website )

include( MacroLogFeature )

set( WEBSITE_OUTPUT_DIR "${CMAKE_BINARY_DIR}/doc/website/html" )

find_program( JEKYLL_COMMAND jekyll ) 
MACRO_LOG_FEATURE( JEKYLL_COMMAND "Jekyll" "Generate libsnap static website" "http://jekyllrb.com" FALSE "0.6.2" "")

if( JEKYLL_COMMAND )
  add_custom_target( website ${JEKYLL_COMMAND} ${CMAKE_CURRENT_SOURCE_DIR} ${WEBSITE_OUTPUT_DIR} )
  set_property( DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${WEBSITE_OUTPUT_DIR} )

  install(CODE "execute_process(COMMAND ${CMAKE_BUILD_TOOL} website)")
  install(DIRECTORY ${WEBSITE_OUTPUT_DIR} DESTINATION ${SNAP_DOC_INSTALL_DIR} )
elseif( JEKYLL_COMMAND )
  message( STATUS "Please install jekyll to generate the libsnap website" )
endif( JEKYLL_COMMAND )