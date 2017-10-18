cmake_minimum_required( VERSION 2.8 )

project( m2i )

# Configuration
# =============
if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Debug CACHE STRING "The type of build, options are None, Debug, Release, RelWithDebInfo, MinSizeRel" FORCE )
endif()

if( NOT DEFINED BUILD_DOCS )
    set( BUILD_DOCS false CACHE BOOL "Whether or not to build the documentation" FORCE )
endif()

if( NOT DEFINED CHECK_DOCS )
    set( CHECK_DOCS false CACHE BOOL "Check if generated documentation has differed from respository" FORCE )
endif()

# Optional Features
if( NOT DEFINED WITH_JACK )
    set( WITH_JACK false CACHE BOOL "whether to build with jack midi support" FORCE )
endif()

if( NOT DEFINED WITH_ALSA )
    set( WITH_ALSA true CACHE BOOL "whether to build with alsa midi support" FORCE )
endif()

if( NOT DEFINED WITH_XORG )
    set( WITH_XORG false CACHE BOOL "whether to build with XORG support support" FORCE )
endif()

if( NOT DEFINED WITH_QT )
    set( WITH_QT false CACHE BOOL "QT system tray support" FORCE )
endif()

# messages
message( "Configuration:" )
message( "\tCMAKE_BUILD_TYPE=" ${CMAKE_BUILD_TYPE} )
message( "\tBUILD_DOCS=" ${BUILD_DOCS} )
message( "\tCHECK_DOCS=" ${CHECK_DOCS} )
message( "\tWITH_ALSA=" ${WITH_ALSA} )
message( "\tWITH_JACK=" ${WITH_JACK} )
message( "\tWITH_XORG=" ${WITH_XORG} )
message( "\tWITH_QT=" ${WITH_QT} )

if( NOT ${WITH_ALSA} AND NOT ${WITH_JACK} )
    message( SEND_ERROR "Either -DWITH_ALSA=ON or -DWITH_JACK=ON needs to be specified.") 
endif()

# TODO configure a header file to add the preprocessor flags rather than using
# add_definitions here in cmake.add_custom_command

# Compiler
# --------
set (CMAKE_CXX_STANDARD 11)
add_definitions("-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")

# dependencies
# ============
find_package( PkgConfig )

if( ${WITH_ALSA} )
    find_package( ALSA REQUIRED )
    include_directories( ${ALSA_INCLUDE_DIRS} )
    set( LIBS ${LIBS} ${ALSA_LIBRARIES} )
    add_definitions( -DWITH_ALSA )
endif()

if( ${WITH_JACK} )
    pkg_check_modules( JACK jack )
    include_directories( ${JACK_INCLUDE_DIRS} )
    set( LIBS ${LIBS} ${JACK_LIBRARIES} )
    add_definitions( -DWITH_JACK )
endif()

if( ${WITH_XORG} )
    pkg_check_modules( XTST xtst )
    pkg_check_modules( X11 x11 )
    include_directories( ${X11_INCLUDE_DIRS} ${XTST_INCLUDE_DIRS} )
    set( LIBS ${LIBS} ${XTST_LIBRARIES} ${X11_LIBRARIES} )
    add_definitions( -DWITH_XORG )
endif()

if( ${WITH_QT} )
    find_package( Qt5 COMPONENTS Core Widgets REQUIRED )
    set( LIBS ${LIBS} Qt5::Widgets )
    set(CMAKE_AUTOMOC ON)
    add_definitions( -DWITH_QT )
endif()

pkg_check_modules( LUA lua52 )
include_directories( ${LUA_INCLUDE_DIRS} )
set( LIBS ${LIBS} ${LUA_LIBRARIES} )


include_directories( ${PROJECT_SOURCE_DIR}/include )
# sub directories
# ===============
add_subdirectory( lib/fmt EXCLUDE_FROM_ALL )
add_subdirectory( src )

if( ${BUILD_DOCS} )
    add_subdirectory( doc )
endif()

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

