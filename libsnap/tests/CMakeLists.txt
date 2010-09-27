find_package( CUnit )

MACRO_LOG_FEATURE( CUNIT_FOUND "CUnit" "C Unit Test Framework" "http://cunit.sourceforge.net" FALSE "2.1" "")


if( CUNIT_FOUND )
  set( CMAKE_BUILD_TYPE Debug )
  set( CMAKE_C_FLAGS "-std=c99 -g3 -ggdb3 -Wall -Wextra -pedantic -ftest-coverage -fprofile-arcs -DUNIT_TEST" )

  set( libsnap_SRCS
      ../compress.c
      ../edm.c
      ../util.c
      ../snap.c
      main.c
  )

  macro(ADD_UNIT_TEST NAME)
    add_executable( libsnap_${NAME} ${libsnap_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/${NAME}.c)
    target_link_libraries( libsnap_${NAME} ${CUNIT_LIBRARY} )
    add_test( libsnap_${NAME} ${EXECUTABLE_OUTPUT_PATH}/libsnap_${NAME} )
  endmacro(ADD_UNIT_TEST)

  ADD_UNIT_TEST( compress_test )
  ADD_UNIT_TEST( edm_test )
  ADD_UNIT_TEST( snap_test )
  ADD_UNIT_TEST( utils_test )
  ADD_UNIT_TEST( header_test )
  ADD_UNIT_TEST( encoder_test )
  ADD_UNIT_TEST( encoder_bound_test )
  ADD_UNIT_TEST( decoder_test )
  ADD_UNIT_TEST( decoder_result_bound_test )
  ADD_UNIT_TEST( decoder_response_bound_test )
  ADD_UNIT_TEST( dogfooding_test )

endif( CUNIT_FOUND )