cmake_minimum_required( VERSION 2.8 )

project( midi2input )

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

add_subdirectory( src )

set( CPACK_GENERATOR "DEB" )
set( CPACK_DEBIAN_PACKAGE_VERSION "0.1" )
set( CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libx11-6, libxtst6, libjack-jackd2-0, liblua5.2-0" )
set( CPACK_DEBIAN_PACKAGE_MAINTAINER "Samuel Nicholas" )
set( CPACK_DEBIAN_PACKAGE_DESCRIPTION "Generate input events from midi" )
set( CPACK_DEBIAN_PACKAGE_SECTION "Utilities" )
set( CPACK_DEBIAN_PACKAGE_SUGGESTS "cadence" )

include( CPack )
