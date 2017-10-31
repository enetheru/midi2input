cmake_minimum_required( VERSION 2.8 )

project( m2i )

# Configuration
# =============
if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Release CACHE STRING "The type of build, options are None, Debug, Release, RelWithDebInfo, MinSizeRel" FORCE )
endif()

if( NOT DEFINED BUILD_DOCS )
    set( BUILD_DOCS YES CACHE BOOL "Whether or not to build the documentation" FORCE )
endif()

# Compiler
# --------
set (CMAKE_CXX_STANDARD 11)
# FIXME this should be part of configuring build types.
add_definitions("-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")

# dependencies
# ============
find_package( PkgConfig REQUIRED )

find_package( ALSA )
if( ${ALSA_FOUND} )
    include_directories( ${ALSA_INCLUDE_DIRS} )
    set( LIBS ${LIBS} ${ALSA_LIBRARIES} )
    add_definitions( -DWITH_ALSA )
endif()

pkg_check_modules( JACK jack )
if( ${JACK_FOUND} )
    include_directories( ${JACK_INCLUDE_DIRS} )
    set( LIBS ${LIBS} ${JACK_LIBRARIES} )
    add_definitions( -DWITH_JACK )
endif()

find_package( X11 )
pkg_check_modules( XTST xtst )
if( ${X11_FOUND} AND "${XTST_FOUND}" )
    set( WITH_X11 YES )
    include_directories( ${X11_INCLUDE_DIRS} ${XTST_INCLUDE_DIRS} )
    set( LIBS ${LIBS} ${XTST_LIBRARIES} ${X11_LIBRARIES} )
    add_definitions( -DWITH_XORG )
endif()

find_package( Qt5 COMPONENTS Core Widgets )
if( ${Qt5Core_FOUND} AND ${Qt5Widgets_FOUND}  )
    set( LIBS ${LIBS} Qt5::Widgets )
    set(CMAKE_AUTOMOC ON)
    add_definitions( -DWITH_QT )
endif()

find_package( Lua 5.2 EXACT REQUIRED )
include_directories( ${LUA_INCLUDE_DIRS} )
set( LIBS ${LIBS} ${LUA_LIBRARIES} )

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

include_directories( ${PROJECT_SOURCE_DIR}/include )
# sub directories
# ===============
add_subdirectory( lib/fmt EXCLUDE_FROM_ALL )
add_subdirectory( src )

if( ${BUILD_DOCS} )
    add_subdirectory( doc )
endif()

# Additional Install Directives
# =============================
install( CODE "execute_process(COMMAND xdg-icon-resource install --size 48 --novendor ${PROJECT_SOURCE_DIR}/res/icons/48/m2i.png )" )

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

