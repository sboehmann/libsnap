find_program( LCOV lcov )
find_program( LCOV_GENHTML genhtml )

if( LCOV AND LCOV_GENHTML )
  set( LCOV_FOUND true )
endif( LCOV AND LCOV_GENHTML )

MACRO_LOG_FEATURE( LCOV_FOUND "lcov" "GCC's coverage testing tool front-end" "http://ltp.sourceforge.net/coverage/lcov.php" FALSE "1.7" "")

if( LCOV_FOUND )
  add_custom_target( lcov ALL
    COMMAND mkdir -p coverage
    COMMAND ${LCOV} --quiet -b ${CMAKE_SOURCE_DIR} -d ${CMAKE_BINARY_DIR} -c -i -o coverage/check.info
    COMMAND cd ${CMAKE_BINARY_DIR} && ${CMAKE_BUILD_TOOL} test
    COMMAND ${LCOV} --quiet -b ${CMAKE_SOURCE_DIR} -d ${CMAKE_BINARY_DIR} -c -o coverage/check.info
    COMMAND ${LCOV_GENHTML} --quiet --legend -t "libsnap tests" -o coverage -p ${CMAKE_SOURCE_DIR} --html-prolog ${CMAKE_CURRENT_SOURCE_DIR}/coverage_header.html --html-epilog ${CMAKE_CURRENT_SOURCE_DIR}/coverage_footer.html -c ${CMAKE_CURRENT_SOURCE_DIR}/coverage_stylesheet.css coverage/check.info
  )

  install( DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/coverage DESTINATION ${SNAP_DOC_INSTALL_DIR}/html )
endif( LCOV_FOUND )