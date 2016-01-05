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

set( CPACK_STRIP_FILES true )
set( CPACK_GENERATOR "DEB" )
set( CPACK_DEBIAN_PACKAGE_VERSION "0.1" )
set( CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libx11-6, libxtst6, libjack-jackd2-0, liblua5.2-0" )
set( CPACK_DEBIAN_PACKAGE_MAINTAINER "Samuel Nicholas <nicholas.samuel@gmail.com>" )
set( CPACK_DEBIAN_PACKAGE_DESCRIPTION
"Turns a midi device into a keyboard/mouse controller
 This program takes midi events from jackd and translates them to input events.
 Configuration is written in Lua." )
set( CPACK_DEBIAN_PACKAGE_SECTION "utils" )

install( FILES ${CMAKE_CURRENT_SOURCE_DIR}/LICENCE.md
        DESTINATION share/doc/midi2input
        COMPONENT applications
        RENAME copyright )

execute_process( COMMAND gzip -k9f CHANGELOG.md
		COMMAND mv CHANGELOG.md.gz ${CMAKE_BINARY_DIR}/
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

install( FILES ${CMAKE_BINARY_DIR}/CHANGELOG.md.gz
        DESTINATION share/doc/midi2input
        COMPONENT applications
        RENAME changelog.gz )

execute_process( COMMAND gzip -k9f midi2input.1
		COMMAND mv midi2input.1.gz ${CMAKE_BINARY_DIR}/
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc )

install( FILES ${CMAKE_BINARY_DIR}/midi2input.1.gz
        DESTINATION share/man/man1/
        COMPONENT applications )
include( CPack )
