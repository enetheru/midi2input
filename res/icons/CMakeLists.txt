#generate any icons we need to here

# the existance of prefix/share/icons/hicolour is specified in the xdg spec.
add_custom_target( icons ALL
    DEPENDS m2i )

add_custom_command( TARGET icons POST_BUILD
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/hicolor/48x48/apps
    COMMAND oiiotool -i ${CMAKE_CURRENT_SOURCE_DIR}/m2i-black.png --resize 48x48 -o ${CMAKE_CURRENT_BINARY_DIR}/hicolor/48x48/apps/m2i-black.png
    COMMAND oiiotool -i ${CMAKE_CURRENT_SOURCE_DIR}/m2i-dark.png --resize 48x48 -o ${CMAKE_CURRENT_BINARY_DIR}/hicolor/48x48/apps/m2i-dark.png
    COMMAND oiiotool -i ${CMAKE_CURRENT_SOURCE_DIR}/m2i-light.png --resize 48x48 -o ${CMAKE_CURRENT_BINARY_DIR}/hicolor/48x48/apps/m2i-light.png
    COMMAND oiiotool -i ${CMAKE_CURRENT_SOURCE_DIR}/m2i-white.png --resize 48x48 -o ${CMAKE_CURRENT_BINARY_DIR}/hicolor/48x48/apps/m2i-white.png
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/hicolor/512x512/apps
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/m2i-light.png ${CMAKE_CURRENT_BINARY_DIR}/hicolor/512x512/apps
    COMMENT "Generating Icons")

install( DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/hicolor DESTINATION share/icons )
