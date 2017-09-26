include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

add_executable( midi2input
        midi2input.cpp
        log.h
        midi.h
        jack.h
        jack.cpp )
target_link_libraries( midi2input ${LIBS} )

if( BUILD_DOCS )
    add_custom_command( TARGET midi2input POST_BUILD
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/midi2input -h > ${CMAKE_BINARY_DIR}/doc/help.txt
        COMMENT "midi2input -h > ${CMAKE_BINARY_DIR}/doc/help.txt" )
endif()

install( TARGETS midi2input DESTINATION bin )
