# - Find CUnit headers and library
# Once done this will define
#
#  CUNIT_FOUND          - system has snap library
#  CUNIT_INCLUDES       - the snap include directory
#  CUNIT_LIBRARY        - the snap library
#
# Copyright 2010 by Stefan Böhmann <kde@hilefoks.org>
# Redistribution and use is allowed according to the terms of the BSD license.

find_path( CUNIT_INCLUDES CUnit/CUnit.h )
find_library( CUNIT_LIBRARY NAMES cunit libcunit )

include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( CUNIT DEFAULT_MSG CUNIT_LIBRARY CUNIT_INCLUDES )

mark_as_advanced (CUNIT_LIBRARY CUNIT_INCLUDES)

