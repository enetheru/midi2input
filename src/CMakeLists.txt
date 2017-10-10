include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

if( WITH_ALSA )
    set( SOURCES ${SOURCES} alsa.h alsa.cpp )
endif()

if( WITH_JACK )
    set( SOURCES ${SOURCES} jack.h jack.cpp )
endif()

if( WITH_XORG )
    set( SOURCES ${SOURCES} x11.h x11.cpp )
endif()

add_executable( midi2input
        midi2input.cpp
        log.h
        midi.h
        cache_data.h
        cache_data.cpp
        ${SOURCES} )
target_link_libraries( midi2input
    stdc++fs
    ${LIBS} )

set_target_properties( midi2input PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR} )

if( BUILD_DOCS )
    add_custom_command( TARGET midi2input POST_BUILD
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/midi2input -h > ${CMAKE_BINARY_DIR}/doc/help.txt
        COMMENT "midi2input -h > ${CMAKE_BINARY_DIR}/doc/help.txt" )
endif()

install( TARGETS midi2input DESTINATION bin )
