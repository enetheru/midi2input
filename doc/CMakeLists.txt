include( docs.cmake )

IF( BUILD_DOCS )
    configure_file( midi2input.1.in midi2input.1 )

    # I have to decide what I want to configure here.

    install( FILES ${CMAKE_SOURCE_DIR}/LICENCE.md
        DESTINATION share/doc/midi2input
        COMPONENT applications
        RENAME copyright )

        execute_process(
        COMMAND gzip -k9f CHANGELOG.md
	    COMMAND mv CHANGELOG.md.gz ${CMAKE_BINARY_DIR}/
	    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ )

    install( FILES ${CMAKE_BINARY_DIR}/CHANGELOG.md.gz
        DESTINATION share/doc/midi2input
        COMPONENT applications
        RENAME changelog.gz )

    execute_process(
        COMMAND gzip -k9f midi2input.1
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )

    install( FILES ${CMAKE_CURRENT_BINARY_DIR}/midi2input.1.gz
        DESTINATION share/man/man1/
        COMPONENT applications )

endif()