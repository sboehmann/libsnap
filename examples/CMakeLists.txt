project( csnap )

set( CMAKE_C_FLAGS "-std=c99 -Wall -Wextra -pedantic" )

add_executable( simple_decode simple_decode.c )
target_link_libraries( simple_decode snap )

add_executable( simple_encode simple_encode.c )
target_link_libraries( simple_encode snap )