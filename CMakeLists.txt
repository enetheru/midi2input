cmake_minimum_required( VERSION 2.8 )

project( midi2input )

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
#TODO make sure either jack or alsa support is enabled
#TODO add xwindows in here
if( NOT DEFINED WITH_JACK )
    set( WITH_JACK false CACHE BOOL "whether to build with jack midi support" FORCE )
endif()

if( NOT DEFINED WITH_ALSA )
    set( WITH_ALSA true CACHE BOOL "whether to build with alsa midi support" FORCE )
endif()

if( NOT DEFINED WITH_X11 )
    set( WITH_XORG false CACHE BOOL "whether to build with XORG support support" FORCE )
endif()

# messages
message( "Build Type: " ${CMAKE_BUILD_TYPE} )
message( "Build Documentation: " ${BUILD_DOCS} )
message( "Check Documentation: " ${CHECK_DOCS} )
message( "With jack: " ${WITH_JACK} )
message( "With alsa: " ${WITH_ALSA} )
message( "With xorg: " ${WITH_XORG} )

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

pkg_check_modules( XTST xtst )
pkg_check_modules( X11 x11 )
pkg_check_modules( LUA lua52 )

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${XTST_INCLUDE_DIRS}
    ${X11_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIRS}
)


set( LIBS ${LIBS}
    ${XTST_LIBRARIES}
    ${X11_LIBRARIES}
    ${LUA_LIBRARIES}
)

# sub directories
# ===============
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

