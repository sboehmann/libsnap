# - Find libsnap headers and library
# Once done this will define
#
#  LIBSNAP_FOUND          - system has snap library
#  LIBSNAP_INCLUDES       - the snap include directory
#  LIBSNAP_LIBRARY        - the snap library
#  LIBSNAP_STATIC_FOUND   - system has static snap library
#  LIBSNAP_STATIC_LIBRARY - the static snap library
#
# Copyright 2010 by Stefan Böhmann <kde@hilefoks.org>
# Redistribution and use is allowed according to the terms of the BSD license.

if( LIBSNAP_INCLUDES )
  # Already in cache
  set( LIBSNAP_QUIETLY TRUE )
endif( LIBSNAP_INCLUDES )

set( LIBRARY_HINTS "libsnap/ ../libsnap/" )

find_path( LIBSNAP_INCLUDES libsnap/snap.h ${CMAKE_CURRENT_SOURCE_DIR}/.. )
find_library( LIBSNAP_LIBRARY NAMES snap libsnap HINTS ${LIBRARY_HINTS} )
find_library( LIBSNAP_STATIC_LIBRARY NAMES snap.a libsnap.a HINTS ${LIBRARY_HINTS} )

include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( LIBSNAP DEFAULT_MSG LIBSNAP_LIBRARY LIBSNAP_INCLUDES )
find_package_handle_standard_args( LIBSNAP_STATIC DEFAULT_MSG LIBSNAP_STATIC_LIBRARY LIBSNAP_INCLUDES )

mark_as_advanced (LIBSNAP_LIBRARY LIBSNAP_INCLUDES)
mark_as_advanced( LIBSNAP_STATIC_LIBRARY LIBSNAP_INCLUDES )

