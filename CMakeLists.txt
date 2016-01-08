cmake_minimum_required( VERSION 2.8 )

project( midi2input )

# Configuration
# =============
if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Debug CACHE STRING
        "The type of build, options are None, Debug, Release, RelWithDebInfo, MinSizeRel" FORCE )
endif()
message( "Build Type: " ${CMAKE_BUILD_TYPE} )

if( NOT BUILD_DOCS )
    set( BUILD_DOCS true CACHE BOOL
        "Whether or not to build the documentation"
        FORCE )
endif()
message( "Build Documentation: " ${BUILD_DOCS} )

# dependencies
# ============

find_package( PkgConfig )
pkg_check_modules( JACK jack )
pkg_check_modules( XTST xtst )
pkg_check_modules( X11 x11 )
pkg_check_modules( LUA lua5.2 )

include_directories(
    ${JACK_INCLUDE_DIRS}
    ${XTST_INCLUDE_DIRS}
    ${X11_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIRS}
)

set( LIBS ${LIBS}
    ${JACK_LIBRARIES}
    ${XTST_LIBRARIES}
    ${X11_LIBRARIES}
    ${LUA_LIBRARIES} )

# sub directories
# ===============
add_subdirectory( src )
add_subdirectory( doc )

# CPack
# =====
include( doc/docs.cmake )
set( CPACK_STRIP_FILES true )
set( CPACK_GENERATOR "DEB" )
set( CPACK_DEBIAN_PACKAGE_VERSION "0.1" )
set( CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libx11-6, libxtst6, libjack-jackd2-0, liblua5.2-0" )
set( CPACK_DEBIAN_PACKAGE_MAINTAINER "${DOC_AUTHORS}" )
string( REPLACE "\n" "\n " CPACK_DEBIAN_PACKAGE_DESCRIPTION ${DOC_DESCRIPTION_SHORT} "\n" ${DOC_DESCRIPTION} )
set( CPACK_DEBIAN_PACKAGE_SECTION "utils" )
include( CPack )

