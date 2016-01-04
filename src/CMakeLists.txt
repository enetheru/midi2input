include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

add_executable( midi2input midi2input.cpp )
target_link_libraries( midi2input ${LIBS} )
