include( docs.cmake )

if( BUILD_DOCS )
    configure_file( midi2input.1.in midi2input.1 )

    add_custom_target( docs ALL
        DEPENDS midi2input midi2input.1.gz changelog.gz README.md)

#./debian/copyright
    install( FILES ${CMAKE_SOURCE_DIR}/LICENCE.md
        DESTINATION share/doc/midi2input
        COMPONENT applications
        RENAME copyright )

# Man Page
    add_custom_command( OUTPUT midi2input.1.gz
        COMMAND gzip -9 midi2input.1 )

    install( FILES ${CMAKE_CURRENT_BINARY_DIR}/midi2input.1.gz
        DESTINATION share/man/man1/
        COMPONENT applications )

# README
    # configire README.md from standard strings
    configure_file( README.md.in README.md.in2 )

    # concatenate application help message into README.md
    add_custom_command( OUTPUT README.md
        COMMAND cat ${CMAKE_CURRENT_BINARY_DIR}/README.md.in2 >> README.md
        COMMAND /bin/echo -e \\\\n\\\\x60\\\\x60\\\\x60 >> README.md
        COMMAND cat ${CMAKE_CURRENT_BINARY_DIR}/help.txt >> README.md
        COMMAND /bin/echo -e \\\\x60\\\\x60\\\\x60 >> README.md )

    #check this version against the one in the base source directory and prompt the user to update it
    #add_custom_command( TARGET docs POST_BUILD
    #    COMMAND diff -q ${CMAKE_CURRENT_BINARY_DIR}/README.md ${CMAKE_SOURCE_DIR}/README.md )
    #TODO turn this into a pre-commit hook or a check by travis. maybe depend on
    # a separate target that checks things rather than all the time.. think more.

#./debian/changelog.gz
    add_custom_command( OUTPUT changelog.gz
        COMMAND cp ${CMAKE_SOURCE_DIR}/CHANGELOG.md ${CMAKE_CURRENT_BINARY_DIR}/changelog
        COMMAND gzip -9 changelog )

    install( FILES ${CMAKE_CURRENT_BINARY_DIR}/changelog.gz
        DESTINATION share/doc/midi2input
        COMPONENT applications )

endif()
