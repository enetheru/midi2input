add_executable( m2i
    main.h    main.cpp
    util.h    util.cpp
    midi.h    midi.cpp
    lua.h     lua.cpp
    inotify.h inotify.cpp )

target_include_directories( m2i PRIVATE
    ${LUA_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/include )

target_link_libraries( m2i PRIVATE
    fmt
    stdc++fs
    ${LUA_LIBRARIES} )

if( ${ALSA_FOUND} )
    target_sources( m2i PRIVATE alsa.h alsa.cpp )
    target_include_directories( m2i PRIVATE ${ALSA_INCLUDE_DIRS} )
    target_link_libraries( m2i PRIVATE ${ALSA_LIBRARIES} )
    add_definitions( -DWITH_ALSA )
endif()

if( ${JACK_FOUND} )
    target_sources( m2i PRIVATE jack.h jack.cpp )
    target_include_directories( m2i PRIVATE ${JACK_INCLUDE_DIRS} )
    target_link_libraries( m2i PRIVATE ${JACK_LIBRARIES} )
    add_definitions( -DWITH_JACK )
endif()

if( ${WITH_X11} )
    target_sources( m2i PRIVATE x11.h x11.cpp )
    target_include_directories( m2i PRIVATE ${X11_INCLUDE_DIRS} ${XTST_INCLUDE_DIRS} )
    target_link_libraries( m2i PRIVATE ${XTST_LIBRARIES} ${X11_LIBRARIES} )
    add_definitions( -DWITH_XORG )
endif()

if( ${Qt5Core_FOUND} AND ${Qt5Widgets_FOUND}  )
    target_sources( m2i PRIVATE trayicon.h trayicon.cpp )
    target_link_libraries( m2i PUBLIC Qt5::Widgets )
    add_definitions( -DWITH_QT )
endif()

target_compile_options(  m2i PRIVATE -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic )

target_compile_features( m2i PRIVATE
    cxx_range_for
    cxx_nullptr
    cxx_auto_type )

set_target_properties( m2i PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR} )

if( BUILD_DOCS )
    add_custom_command( TARGET m2i POST_BUILD
        COMMAND ${PROJECT_BINARY_DIR}/m2i -h > ${CMAKE_BINARY_DIR}/doc/help.txt
        COMMENT "m2i -h > ${CMAKE_BINARY_DIR}/doc/help.txt" )
endif()

install( TARGETS m2i DESTINATION bin )
