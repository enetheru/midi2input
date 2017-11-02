include( docs.cmake )

if( BUILD_DOCS )
    configure_file( m2i.1.in m2i.1 )

    add_custom_target( docs ALL
        DEPENDS m2i m2i.1.gz changelog.gz )

#./debian/copyright
    install( FILES ${CMAKE_SOURCE_DIR}/LICENCE.md
        DESTINATION share/doc/m2i
        COMPONENT applications
        RENAME copyright )

# Man Page
    add_custom_command( OUTPUT m2i.1.gz
        COMMAND gzip -9 m2i.1 )

    install( FILES ${CMAKE_CURRENT_BINARY_DIR}/m2i.1.gz
        DESTINATION share/man/man1/
        COMPONENT applications )

#./debian/changelog.gz
    add_custom_command( OUTPUT changelog.gz
        COMMAND cp ${CMAKE_SOURCE_DIR}/CHANGELOG.md ${CMAKE_CURRENT_BINARY_DIR}/changelog
        COMMAND gzip -9 changelog )

    install( FILES ${CMAKE_CURRENT_BINARY_DIR}/changelog.gz
        DESTINATION share/doc/m2i
        COMPONENT applications )

endif()
