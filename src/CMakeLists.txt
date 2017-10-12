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

add_executable( m2i
        main.cpp
        log.h
        midi.h
        midi.cpp
        lua.cpp
        lua.h
        cache_data.h
        cache_data.cpp
        ${SOURCES} )
target_link_libraries( m2i
    stdc++fs
    ${LIBS} )

set_target_properties( m2i PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR} )

if( BUILD_DOCS )
    add_custom_command( TARGET m2i POST_BUILD
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/m2i -h > ${CMAKE_BINARY_DIR}/doc/help.txt
        COMMENT "midi2input -h > ${CMAKE_BINARY_DIR}/doc/help.txt" )
endif()

install( TARGETS m2i DESTINATION bin )
