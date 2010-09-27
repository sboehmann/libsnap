include( InstallRequiredSystemLibraries )

set( CPACK_PACKAGE_NAME "libsnap" )
set( CPACK_PACKAGE_VENDOR "Stefan Böhmann" )
set( CPACK_PACKAGE_CONTACT "kde@hilefoks.org" )
set( CPACK_PACKAGE_DESCRIPTION_SUMMARY "libsnap, a portable open-source S.N.A.P implementation" )

set(CPACK_PACKAGE_VERSION_MAJOR ${snap_MAJOR_VERSION} )
set(CPACK_PACKAGE_VERSION_MINOR ${snap_MINOR_VERSION} )
set(CPACK_PACKAGE_VERSION_PATCH ${snap_PATCH_LEVEL} )

#License file for the project, used by the STGZ, and NSIS generators.
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING" )

set(CPACK_GENERATOR "TBZ2;ZIP")
set(CPACK_SOURCE_GENERATOR "TBZ2;TGZ")
set(CPACK_SOURCE_STRIP_FILES ON)

set(CPACK_SOURCE_IGNORE_FILES "/\\\\.svn/")

include(CPack)