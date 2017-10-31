include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/lib/fmt
    )

if( ${ALSA_FOUND} )
    set( SOURCES ${SOURCES} alsa.h alsa.cpp )
endif()

if( ${JACK_FOUND} )
    set( SOURCES ${SOURCES} jack.h jack.cpp )
endif()

if( ${WITH_X11} )
    set( SOURCES ${SOURCES} x11.h x11.cpp )
endif()

if( ${Qt5_FOUND} )
    set( SOURCES ${SOURCES} trayicon.h trayicon.cpp )
endif()

add_executable( m2i
        main.cpp
        util.h
        util.cpp
        midi.h
        midi.cpp
        lua.cpp
        lua.h
        inotify.h
        inotify.cpp
        ${SOURCES} )
target_link_libraries( m2i
    fmt
    stdc++fs
    ${LIBS} )

set_target_properties( m2i PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR} )

if( BUILD_DOCS )
    add_custom_command( TARGET m2i POST_BUILD
        COMMAND ${PROJECT_BINARY_DIR}/m2i -h > ${CMAKE_BINARY_DIR}/doc/help.txt
        COMMENT "m2i -h > ${CMAKE_BINARY_DIR}/doc/help.txt" )
endif()

install( TARGETS m2i DESTINATION bin )
