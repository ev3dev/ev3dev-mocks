
configure_file(ev3dev-mocks-run.in ev3dev-mocks-run @ONLY)
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/ev3dev-mocks-run DESTINATION ${CMAKE_INSTALL_BINDIR})

find_program(PANDOC pandoc)

if(NOT PANDOC_NOTFOUND)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ev3dev-mocks-run.1
        COMMAND ${PANDOC}
            --standalone
            --variable=header:"User's Guide"
            --variable=section:1
            --to=man
            --output ${CMAKE_CURRENT_BINARY_DIR}/ev3dev-mocks-run.1
            ${CMAKE_CURRENT_SOURCE_DIR}/ev3dev-mocks-run.md
        DEPENDS
            ev3dev-mocks-run.md
    )
    add_custom_target(ev3dev-mocks-run-man ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/ev3dev-mocks-run.1)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ev3dev-mocks-run.1 DESTINATION ${CMAKE_INSTALL_MANDIR})
endif(NOT PANDOC_NOTFOUND)
