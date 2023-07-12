if (WIN32)
    execute_process(
            COMMAND git log -1 --format=%h
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
            OUTPUT_VARIABLE GIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    configure_file(version.rc.in version.rc @ONLY)
    set(VERSION_RC ${CMAKE_BINARY_DIR}/version.rc)
endif()