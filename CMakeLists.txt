cmake_minimum_required( VERSION 3.9 )

project( m2i )

# Configuration
# =============
if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Release CACHE STRING "The type of build, options are None, Debug, Release, RelWithDebInfo, MinSizeRel" FORCE )
endif()

if( NOT DEFINED BUILD_DOCS )
    set( BUILD_DOCS YES CACHE BOOL "Whether or not to build the documentation" FORCE )
endif()

# dependencies
# ============
find_package( Lua 5.3 EXACT REQUIRED )
find_package( ALSA REQUIRED )
find_package( X11 )
find_package( Qt5 QUIET COMPONENTS Core Widgets )
find_package( PkgConfig REQUIRED )
pkg_check_modules( XTST xtst )
pkg_check_modules( JACK jack )

if( ${X11_FOUND} AND "${XTST_FOUND}" )
    set( WITH_X11 YES )
endif()

if( ${Qt5Core_FOUND} AND ${Qt5Widgets_FOUND}  )
    set(CMAKE_AUTOMOC ON)
    add_subdirectory( res/icons )
endif()

# messages
message( "-- Configuration:" )
message( "--   CMAKE_BUILD_TYPE=" ${CMAKE_BUILD_TYPE} )
message( "--   CMAKE_INSTALL_PREFIX=" ${CMAKE_INSTALL_PREFIX} )
message( "--   BUILD_DOCS=" ${BUILD_DOCS} )
message( "--   WITH_ALSA=" ${ALSA_FOUND} )
message( "--   WITH_JACK=" ${JACK_FOUND} )
message( "--   WITH_X11=" ${WITH_X11} )
message( "--   WITH_QT=" ${Qt5_FOUND} )

if( NOT ( ${ALSA_FOUND} OR "${JACK_FOUND}" ) )
    message( SEND_ERROR "Either ALSA or Jack needs to be available.") 
endif()

# sub directories
# ===============
add_subdirectory( lib/fmt EXCLUDE_FROM_ALL )
add_subdirectory( src )

if( ${BUILD_DOCS} )
    add_subdirectory( doc )
endif()

# additional installation directives
# ==================================
install( FILES cfg/config.lua cfg/basic.lua
    DESTINATION share/m2i )
install( FILES res/applications/m2i.desktop
    DESTINATION share/applications )

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

